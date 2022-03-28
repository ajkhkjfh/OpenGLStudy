#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_VerticesCounts;
	unsigned int m_VerticesAttributes;

public:
	VertexBuffer(const void* data, unsigned int verticesCounts,
		unsigned int verticesAttributes, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetVerticesCounts() const { return m_VerticesCounts; }
	inline unsigned int GetVerticesAttributes() const { return m_VerticesAttributes; }
};
