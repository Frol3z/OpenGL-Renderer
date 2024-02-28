#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, size_t size, size_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline size_t GetCount() const { return m_Count; }
	inline const unsigned int* GetIndices() const { return m_Indices; }
private:
	unsigned int m_Id;
	size_t m_Count;
	const unsigned int* m_Indices;
};