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
	void SetViewAndProjectionMatrix(const glm::mat4& view, const glm::mat4& proj);
	void SetPosition(glm::vec3 position);
	void SetRotation(float degree, glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	void SetTexture(Texture* texture);
	void SetSpecularTexture(Texture* texture);
	void SetEmissionMap(Texture* texture);

	void SetDirectionalLight(glm::vec3&& direction, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular);

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
	Texture* m_SpecularTexture;
	Texture* m_EmissionMap;

	glm::vec3 m_LightPosition;
	glm::vec3 m_LightAmbient;
	glm::vec3 m_LightDiffuse;
	glm::vec3 m_LightSpecular;
};