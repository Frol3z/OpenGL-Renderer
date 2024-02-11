#pragma once

#include <glm/glm.hpp>

class ImGuiWindow
{
public:
	ImGuiWindow() : 
		m_CameraSpeed(5.0f), m_CameraSensitivity(0.1f),
		m_DirLightDirection{ -0.2f, -1.0f, -0.3f }, m_DirLightColor{ 1.0f, 1.0f, 1.0f }, m_DirLightAmbient(0.2f), m_DirLightDiffuse(0.5f),
		m_ObjectPosition{0.0f, 0.0f, 0.0f}, 
		m_ObjectAmbient { 1.0f, 0.5f, 0.31f }, m_ObjectDiffuse{ 1.0f, 0.5f, 0.31f },
		m_ObjectSpecular {0.5f, 0.5f, 0.5f}, m_ObjectShininess(32.0f)
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



	/* ---------- Camera ---------- */
	inline const float GetCameraSpeed() const { return m_CameraSpeed; }
	inline const float GetCameraSensitivity() const { return m_CameraSensitivity; }

	/* ---------- Directional Light ---------- */
	inline glm::vec3 GetDirLightDirection() const { return glm::vec3(m_DirLightDirection[0], m_DirLightDirection[1], m_DirLightDirection[2]); }
	inline glm::vec3 GetDirLightAmbient() const { return glm::vec3(m_DirLightAmbient * m_DirLightColor[0], m_DirLightAmbient * m_DirLightColor[1], m_DirLightAmbient * m_DirLightColor[1]); }
	inline glm::vec3 GetDirLightDiffuse() const { return glm::vec3(m_DirLightDiffuse * m_DirLightColor[0], m_DirLightDiffuse * m_DirLightColor[1], m_DirLightDiffuse * m_DirLightColor[2]); }
	inline glm::vec3 GetDirLightSpecular() const { return glm::vec3(m_DirLightColor[0], m_DirLightColor[1], m_DirLightColor[2]); }

private:

	/* ---------- Camera ---------- */
	float m_CameraSpeed;
	float m_CameraSensitivity;

	/* ---------- Directional Light ---------- */
	float m_DirLightDirection[3];
	float m_DirLightColor[3];
	float m_DirLightAmbient;
	float m_DirLightDiffuse;

	float m_ObjectPosition[3];
	float m_ObjectAmbient[3];
	float m_ObjectDiffuse[3];
	float m_ObjectSpecular[3];
	float m_ObjectShininess;
};
