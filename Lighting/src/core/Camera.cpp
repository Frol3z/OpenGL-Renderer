#include "Camera.h"

Camera::Camera(unsigned int width, unsigned int height, glm::vec3 position, glm::vec3 up) :
	  m_Position(position), m_WorldUp(up), m_Front(glm::vec3(0.0f)),
	  m_Yaw(DEFAULT_YAW), m_Pitch(DEFAULT_PITCH),
	  m_Width(width), m_Height(height), m_FieldOfView(DEFAULT_FOV),
	  m_ProjectionMatrix(glm::perspective(glm::radians(DEFAULT_FOV), (float) width / height, 0.1f, 100.0f)),
	  m_Speed(5.0f), m_Sensitivity(0.1f)
{
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_Speed * deltaTime;
	switch (direction)
	{
		case FORWARD:
		{
			m_Position += m_Front * velocity;
			break;
		}
		case BACKWARD:
		{
			m_Position -= m_Front * velocity;
			break;
		}
		case LEFT:
		{
			m_Position -= m_Right * velocity;
			break;
		}
		case RIGHT:
		{
			m_Position += m_Right * velocity;
			break;
		}
	}
	// m_Position.y = 0.0f;

	UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= m_Sensitivity;
	yOffset *= m_Sensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;

		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateFOV(float offset)
{
	m_FieldOfView -= (float) offset;
	if (m_FieldOfView < 1.0f)
		m_FieldOfView = 1.0f;
	if (m_FieldOfView > 45.0f)
		m_FieldOfView = 45.0f;

	m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), (float) m_Width / m_Height, 0.1f, 100.0f);
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

const glm::mat4 Camera::GetViewMatrix() const
{
	// m_Position + m_Front = position of what the camera is looking at
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

const glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void Camera::SetSpeed(const float speed)
{
	m_Speed = speed;
}

void Camera::SetSensitivity(const float sensitivity)
{
	m_Sensitivity = sensitivity;
}