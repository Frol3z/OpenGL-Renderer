#pragma once

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void AddVertexBuffer(const class VertexBuffer& vb, const class VertexBufferLayout& layout);
	void AddIndexBuffer(const class IndexBuffer& ib);
private:
	unsigned int m_Id;
};