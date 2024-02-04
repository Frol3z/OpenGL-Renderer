#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<VertexBufferAttribute> attribs = layout.GetAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < attribs.size(); i++)
	{
		const auto& attrib = attribs[i];
		glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalized,
			layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += attrib.count * VertexBufferAttribute::GetSizeOfType(attrib.type);
	}
	Unbind();
	vb.Unbind();
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ib)
{
	Bind();
	ib.Bind();
	Unbind();
	ib.Unbind();
}