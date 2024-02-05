#include "Object.h"

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

Object::Object(const float* verts, size_t vertsSize, const unsigned int* indices, size_t indicesCount)
	: m_ModelMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f)),
	  m_Color(glm::vec3(1.0f)), m_LightColor(glm::vec3(1.0f))
{
	m_VertexArray = new VertexArray();

	m_VertexBuffer = new VertexBuffer(verts, vertsSize);
	m_NumberOfVertices = vertsSize / sizeof(float);

	m_VertexBufferLayout = new VertexBufferLayout();
	m_VertexBufferLayout->Push<float>(3);

	m_VertexArray->AddVertexBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

	if (indices)
	{
		m_IndexBuffer = new IndexBuffer(indices, indicesCount);
		m_VertexArray->AddIndexBuffer(*m_IndexBuffer);
	}
	else
	{
		m_IndexBuffer = nullptr;
	}

	m_Shader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
}

Object::~Object()
{
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_VertexBufferLayout;
	delete m_VertexArray;
	delete m_Shader;
}

void Object::Draw() const
{
	m_Shader->Use();
	m_VertexArray->Bind();

	m_Shader->SetMat4("model", m_ModelMatrix);
	m_Shader->SetMat4("view", m_ViewMatrix);
	m_Shader->SetMat4("projection", m_ProjectionMatrix);
	m_Shader->SetVec3("objectColor", m_Color);
	m_Shader->SetVec3("lightColor", m_LightColor);

	glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVertices);

	m_VertexArray->Unbind();
} 

void Object::SetShader(const char* vertPath, const char* fragPath)
{
	delete m_Shader;
	m_Shader = new Shader(vertPath, fragPath);
}

void Object::SetColor(glm::vec3 color)
{
	m_Color = color;
}

void Object::SetLightColor(glm::vec3 color)
{
	m_LightColor = color;
}

void Object::SetViewAndProjectionMatrix(glm::mat4 view, glm::mat4 proj)
{
	m_ViewMatrix = view;
	m_ProjectionMatrix = proj;
}

void Object::SetPosition(glm::vec3 position)
{
	m_ModelMatrix = glm::translate(m_ModelMatrix, position);
}

void Object::SetRotation(float degree, glm::vec3 rotation)
{
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(degree), rotation);
}

void Object::SetScale(glm::vec3 scale)
{
	m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
}