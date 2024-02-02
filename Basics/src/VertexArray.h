#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>

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