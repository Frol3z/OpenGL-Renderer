#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometry/Mesh.h"
#include "Shader.h"
#include "Material.hpp"

class Object
{
public:
	bool isUniformScaling;
	int selectedMesh;
	int selectedMaterial;
	int selectedShader;

public:
	Object(std::string&& name, Mesh* mesh, Material* material, Shader* shader);

	virtual void Draw() const;
	
	// Setters
	void SetName(const std::string& name);
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	void SetShader(Shader* shader);

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	void SetViewAndProjectionMatrix(const glm::mat4& view, const glm::mat4& proj);
	void SetDirectionalLight(glm::vec3&& direction, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular);

	// Getters
	inline const std::string& GetName() const { return m_Name; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::vec3& GetRotation() const { return m_Rotation; }
	inline const glm::vec3 GetScale() const { return m_Scale; }

protected:
	std::string m_Name;

	Mesh* m_Mesh;
	Shader* m_Shader;
	Material* m_Material;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	// Model transform
	glm::mat4 m_TranslationTransform;
	glm::mat4 m_RotationTransform;
	glm::mat4 m_ScaleTransform;

	glm::mat4 m_ViewTransform;
	glm::mat4 m_ProjectionTransform;
};