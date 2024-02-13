#pragma once

#include <glm/glm.hpp>
#include <string>

class Material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

public:
	Material() :
		ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(0.5f)), shininess(32.0f), m_Name("Material")
	{
	}

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : 
		ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), m_Name("Material")
	{
	}

	void SetName(const std::string& name) { m_Name = name; }
	inline const std::string& GetName() const { return m_Name; }

private:
	std::string m_Name;
};