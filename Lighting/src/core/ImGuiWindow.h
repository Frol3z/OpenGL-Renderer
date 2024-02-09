#pragma once

#include <glm/glm.hpp>

class ImGuiWindow
{
public:
	ImGuiWindow() 
		: m_ObjectPosition{0.0f, 0.0f, 0.0f}, 
		m_ObjectAmbient { 1.0f, 0.5f, 0.31f }, m_ObjectDiffuse{ 1.0f, 0.5f, 0.31f },
		m_ObjectSpecular {0.5f, 0.5f, 0.5f}, m_ObjectShininess(32.0f),
		m_LightPosition { 2.0f, 4.0f, 1.0f } 
	{};

public:
	void Init(GLFWwindow* window);
	void Shutdown();
	void Update(bool isCursorDisabled);
	void Render() const;

	inline glm::vec3 GetObjectPosition() const 
	{ 
		return glm::vec3(m_ObjectPosition[0], m_ObjectPosition[1], m_ObjectPosition[2]);
	};

	inline glm::vec3 GetObjectAmbient() const
	{
		return glm::vec3(m_ObjectAmbient[0], m_ObjectAmbient[1], m_ObjectAmbient[2]);
	};

	inline glm::vec3 GetObjectDiffuse() const
	{
		return glm::vec3(m_ObjectDiffuse[0], m_ObjectDiffuse[1], m_ObjectDiffuse[2]);
	};

	inline glm::vec3 GetObjectSpecular() const
	{
		return glm::vec3(m_ObjectSpecular[0], m_ObjectSpecular[1], m_ObjectSpecular[2]);
	};

	inline float GetObjectShininess() const
	{
		return m_ObjectShininess;
	};

	inline glm::vec3 GetLightPosition() const
	{
		return glm::vec3(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
	};

private:
	float m_ObjectPosition[3];
	float m_ObjectAmbient[3];
	float m_ObjectDiffuse[3];
	float m_ObjectSpecular[3];
	float m_ObjectShininess;

	float m_LightPosition[3];
};
