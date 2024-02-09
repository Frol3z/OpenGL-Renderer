#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometry/Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

class Object
{
public:
	Object(Mesh* mesh, Shader* shader, Material* material);
	Object(const Object& other);

	virtual void Draw() const;
	void SetMaterial(Material* material);
	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	void SetShininess(float shininess);
	void SetViewAndProjectionMatrix(glm::mat4 view, glm::mat4 proj);
	void SetPosition(glm::vec3 position);
	void SetRotation(float degree, glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	void SetTexture(Texture* texture);

	void SetLightPosition(glm::vec3 position);
	void SetLightColor(glm::vec3 color);

protected:
	Mesh* m_Mesh;

	glm::mat4 m_TranslationTransform;
	glm::mat4 m_RotationTransform;
	glm::mat4 m_ScaleTransform;

	glm::mat4 m_ViewTransform;
	glm::mat4 m_ProjectionTransform;

	Shader* m_Shader;
	Material* m_Material;
	glm::vec3 m_Position;

	Texture* m_Texture;

	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColor;
};