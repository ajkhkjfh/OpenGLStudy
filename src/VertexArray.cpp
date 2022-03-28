#include "VertexArray.h"
#include "Renderer.h"
#include"VertexBufferLayout.h"

VertexArray::VertexArray(){
	GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray(){
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0;i < elements.size();i++) {
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i,elements[i].count,elements[i].type,
			elements[i].normalized, layout.GetStride(), (const void*)offset));
		offset += elements[i].count*VertexBufferElement::GetSizeOfType(elements[i].type);
	}
}

void VertexArray::Bind() const{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const {
	GLCALL(glBindVertexArray(0));
}
