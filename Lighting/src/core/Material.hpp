#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Texture.h"

class Material
{
public:
	// Default constructor
	Material(std::string&& name) :
		m_Name(name), 
		m_Ambient(glm::vec3(1.0f)), m_Diffuse(glm::vec3(1.0f)), m_Specular(glm::vec3(0.5f)), m_Shininess(32.0f),
		m_DiffuseMap(nullptr), m_SpecularMap(nullptr), m_EmissionMap(nullptr)
	{
	}

	// Simple color constructor
	Material(std::string&& name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
		m_Name(name),
		m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Shininess(shininess),
		m_DiffuseMap(nullptr), m_SpecularMap(nullptr), m_EmissionMap(nullptr)
	{
	}

	// Textured constructor
	Material(std::string&& name, Texture* diffuseMap, Texture* specularMap, float shininess) :
		m_Name(name),
		m_Ambient(glm::vec3(1.0f)), m_Diffuse(glm::vec3(1.0f)), m_Specular(glm::vec3(0.5f)), m_Shininess(shininess),
		m_DiffuseMap(diffuseMap), m_SpecularMap(specularMap), m_EmissionMap(nullptr)
	{
	}

	// Setters
	void SetName(const std::string& name) { m_Name = name; }
	void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
	void SetShininess(float shininess) { m_Shininess = shininess; }
	void SetDiffuseMap(Texture* diffuseMap) { m_DiffuseMap = diffuseMap; }
	void SetSpecularMap(Texture* specularMap) { m_SpecularMap = specularMap; }
	void SetEmissionMap(Texture* emissionMap) { m_EmissionMap = emissionMap; }

	// Getters
	const std::string& GetName() const { return m_Name; }
	const glm::vec3& GetAmbient() const { return m_Ambient; }
	const glm::vec3& GetDiffuse() const { return m_Diffuse; }
	const glm::vec3& GetSpecular() const { return m_Specular; }
	float GetShininess() const { return m_Shininess; }
	Texture* GetDiffuseMap() const { return m_DiffuseMap; }
	Texture* GetSpecularMap() const { return m_SpecularMap; }
	Texture* GetEmissionMap() const { return m_EmissionMap; }

private:
	std::string m_Name;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

	Texture* m_DiffuseMap;
	Texture* m_SpecularMap;
	Texture* m_EmissionMap;
};