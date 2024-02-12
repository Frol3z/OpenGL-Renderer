#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT
};

// Looking down the negative Z axis by default
static const float DEFAULT_YAW = -90.0f;
static const float DEFAULT_PITCH = 0.0f;
static const float DEFAULT_FOV = 45.0f;

class Camera
{
public:
	// Constructor
	Camera(unsigned int width, unsigned int height, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	// Inputs
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateFOV(float offset);

	// Matrix Getters
	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;

	// Camera Settings Setters
	void SetSpeed(const float speed);
	void SetSensitivity(const float sensitivity);
	inline const float GetSpeed() const { return m_Speed; }
	inline const float GetSensitivity() const { return m_Sensitivity; }

private:
	void UpdateCameraVectors();

private:
	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;

	// Camera coordinates system vectors
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	// Projection
	unsigned int m_Width;
	unsigned int m_Height;
	float m_FieldOfView;
	glm::mat4 m_ProjectionMatrix;

	float m_Yaw;
	float m_Pitch;

	// Camera settings
	float m_Speed;
	float m_Sensitivity;
};