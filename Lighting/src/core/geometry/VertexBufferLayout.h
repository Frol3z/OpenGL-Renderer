#pragma once

#include <glad/glad.h>

#include <stdexcept>
#include <vector>

struct VertexBufferAttribute
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template <typename T>
	void Push(unsigned int count)
	{
		std::runtime_error();
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	const std::vector<VertexBufferAttribute>& GetAttributes() const { return m_Attributes; }
	inline unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferAttribute> m_Attributes;
	unsigned int m_Stride;
};