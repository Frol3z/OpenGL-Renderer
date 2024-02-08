#pragma once

enum VertexLayout {
	VF = 0,
	VFNF
};

class Mesh
{
public:
	Mesh(const float* vertices, size_t vSize, VertexLayout layout, const unsigned int* indices = nullptr, size_t iSize = 0);
	~Mesh();

	void Bind() const;
	void Unbind() const;

	inline size_t GetVertsCount() const { return m_VertsCount; }
	size_t GetIndicesCount() const;
	const unsigned int* GetIndices() const;

private:
	class VertexArray* m_VA;
	class VertexBuffer* m_VB;
	class VertexBufferLayout* m_VBL;
	class IndexBuffer* m_IB;

	size_t m_VertsCount;
};