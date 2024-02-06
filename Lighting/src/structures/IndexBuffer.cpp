#include "IndexBuffer.h"

#include <glad/glad.h>

#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* data, size_t size, size_t count)
	: m_Indices(data), m_Count(count)
{
	glGenBuffers(1, &m_Id);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	Unbind();
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}