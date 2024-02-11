#pragma once

#include <glm/glm.hpp>

class DirectionalLight
{
public:
	// Constructor
	DirectionalLight() : 
		m_Direction(glm::vec3(-0.2f, -1.0f, -0.3f)),
		m_Ambient(glm::vec3(0.2f)),
		m_Diffuse(glm::vec3(0.5f)),
		m_Specular(glm::vec3(1.0f))
	{
	}

	// Setters
	void SetDirection(glm::vec3 direction) { m_Direction = direction; }
	void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) { m_Specular = specular; }

	// Getters
	inline glm::vec3 GetDirection() const { return m_Direction; }
	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }

private:
	glm::vec3 m_Direction;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};