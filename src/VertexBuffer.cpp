#include "VertexBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int verticesCounts, 
	unsigned int verticesAttributes, unsigned int size)
	:m_VerticesCounts(verticesCounts),m_VerticesAttributes(verticesAttributes){
	GLCALL(glGenBuffers(1, &m_RendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, m_VerticesCounts * m_VerticesAttributes * size,
		data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer(){
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

