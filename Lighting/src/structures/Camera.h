#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT
};

const float SPEED = 5.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1;
const float FOV = 45.0f;

class Camera
{
public:
	Camera(const float width, const float height, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateFOV(float offset);

private:
	void UpdateCameraVectors();

private:
	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;

	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	const float m_Width;
	const float m_Height;
	float m_FieldOfView;
	glm::mat4 m_ProjectionMatrix;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
};