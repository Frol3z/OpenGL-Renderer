#pragma once

#include <glm/glm.hpp>

class DirectionalLight
{
public:
	// Constructor
	DirectionalLight() : 
		m_Direction(glm::vec3(-0.2f, -1.0f, -0.3f)),
		m_Color(glm::vec3(1.0f)),
		m_Ambient(glm::vec3(0.2f)),
		m_Diffuse(glm::vec3(0.5f)),
		m_Specular(glm::vec3(1.0f)),
		m_Intensity(1.0f)
	{
	}

	// Setters
	void SetDirection(glm::vec3 direction) { m_Direction = direction; }
	void SetColor(glm::vec3 color) { m_Color = color; }
	void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
	void SetIntensity(float intensity) { m_Intensity = intensity; }

	// Getters
	inline glm::vec3 GetDirection() const { return m_Direction; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }
	inline float GetIntensity() const { return m_Intensity; }

private:
	glm::vec3 m_Direction;
	glm::vec3 m_Color;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Intensity;
};