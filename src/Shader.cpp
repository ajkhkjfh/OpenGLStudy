#include"Shader.h"

#include <iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"Renderer.h"

Shader::Shader(const std::string& filePath)
    :m_FilePath(filePath),m_RendererID(0){
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader(){
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::UnBind() const {
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& v){
    GLCALL(glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix) {
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& v) {
    GLCALL(glUniform3f(GetUniformLocation(name), v.x, v.y, v.z));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning:uniform '" << name << "'doesn't exist!" << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderSource) {
    unsigned int shaderId = glCreateShader(shaderType);
    const char* src = shaderSource.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    //Error Handling
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (result == false) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        std::cout << "Failed to Compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}