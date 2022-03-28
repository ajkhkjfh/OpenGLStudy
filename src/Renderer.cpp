#include"Renderer.h"
#include<iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* funcition, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGl Error] (" << error << "): " << funcition << " "
            << file << ":" << line << std::endl;
        return false;
    };
    return true;
}

void Renderer::Clear() const {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::DrawIndices(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArrays(const VertexArray& va, const Shader& shader, const VertexBuffer& vb)const {
    shader.Bind();
    va.Bind();
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, vb.GetVerticesCounts()));
}
