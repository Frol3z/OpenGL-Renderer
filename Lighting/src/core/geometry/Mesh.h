#pragma once

#include <string>

enum VertexLayout {
	VF = 0,
	VFNF,
	VFNFTF
};

class Mesh
{
public:
	Mesh(std::string&& name, const float* vertices, size_t vSize, VertexLayout layout, const unsigned int* indices = nullptr, size_t iSize = 0);
	~Mesh();

	void Bind() const;
	void Unbind() const;

	inline size_t GetVertsCount() const { return m_VertsCount; }
	size_t GetIndicesCount() const;
	const unsigned int* GetIndices() const;

	void SetName(const std::string& name) { m_Name = name; }
	inline const std::string& GetName() const { return m_Name; }

private:
	class VertexArray* m_VA;
	class VertexBuffer* m_VB;
	class VertexBufferLayout* m_VBL;
	class IndexBuffer* m_IB;

	std::string m_Name;

	size_t m_VertsCount;
};