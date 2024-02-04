#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, size_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline size_t GetCount() const { return m_Count; }
private:
	unsigned int m_Id;
	size_t m_Count;
};