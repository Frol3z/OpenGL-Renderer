#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Shader.h"

class Object
{
public:
	Object(Mesh* mesh, Shader* shader, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	Object(const Object& other);

	void Draw() const;
	void SetColor(glm::vec3 color);
	void SetViewAndProjectionMatrix(glm::mat4 view, glm::mat4 proj);
	void SetPosition(glm::vec3 position);
	void SetRotation(float degree, glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	void SetLightPosition(glm::vec3 position);
	void SetLightColor(glm::vec3 color);

protected:
	glm::vec3 m_Color;
	glm::vec3 m_Position;

private:
	Mesh* m_Mesh;

	glm::mat4 m_TranslationTransform;
	glm::mat4 m_RotationTransform;
	glm::mat4 m_ScaleTransform;

	glm::mat4 m_ViewTransform;
	glm::mat4 m_ProjectionTransform;

	Shader* m_Shader;

	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColor;
};