#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometry/Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

class Object
{
public:
	bool isUniformScaling;

public:
	Object(std::shared_ptr<Mesh> mesh, Shader* shader, Material* material);

	virtual void Draw() const;
	
	// Setters
	void SetName(const std::string& name);
	void SetMesh(std::shared_ptr<Mesh> mesh);

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	void SetMaterial(Material* material);
	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	void SetShininess(float shininess);
	void SetViewAndProjectionMatrix(const glm::mat4& view, const glm::mat4& proj);
	void SetTexture(Texture* texture);
	void SetSpecularTexture(Texture* texture);
	void SetEmissionMap(Texture* texture);

	void SetDirectionalLight(glm::vec3&& direction, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular);

	// Getters
	inline const std::string& GetName() const { return m_Name; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::vec3& GetRotation() const { return m_Rotation; }
	inline const glm::vec3 GetScale() const { return m_Scale; }

protected:
	std::string m_Name;
	std::shared_ptr<Mesh> m_Mesh;

	glm::mat4 m_TranslationTransform;
	glm::mat4 m_RotationTransform;
	glm::mat4 m_ScaleTransform;

	glm::mat4 m_ViewTransform;
	glm::mat4 m_ProjectionTransform;

	Shader* m_Shader;
	Material* m_Material;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	Texture* m_Texture;
	Texture* m_SpecularTexture;
	Texture* m_EmissionMap;
};