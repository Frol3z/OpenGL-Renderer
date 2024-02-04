#pragma once

class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddIndexBuffer(const IndexBuffer& ib);
private:
	unsigned int m_Id;
};