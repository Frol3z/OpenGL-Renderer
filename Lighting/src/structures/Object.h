#pragma once

class VertexArray;
class VertexBufferLayout;
class VertexBuffer;
class IndexBuffer;
class Shader;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object
{
public:
	Object(const float* verts, size_t vertsSize, const unsigned int* indices = nullptr, size_t indicesCount = 0);
	~Object();

	void Draw() const;
	void SetShader(const char* vertPath, const char* fragPath);
	void SetColor(glm::vec3 color);
	void SetLightColor(glm::vec3 color);
	void SetViewAndProjectionMatrix(glm::mat4 view, glm::mat4 proj);
	void SetPosition(glm::vec3 position);
	void SetRotation(float degree, glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
private:
	VertexArray* m_VertexArray;
	VertexBufferLayout* m_VertexBufferLayout;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

	Shader* m_Shader;
	glm::vec3 m_Color;
	glm::vec3 m_LightColor;

	glm::mat4 m_ModelMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	size_t m_NumberOfVertices;
};