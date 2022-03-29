#pragma once

#include<string>
#include<unordered_map> 
#include"glm/gtc/matrix_transform.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int>m_UniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, const glm::vec4& v);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
	void SetUniform3f(const std::string& name, const glm::vec3& v);

	inline unsigned int GetShaderId() const { return m_RendererID; }

private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filePath);

	unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSource);

	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);

};