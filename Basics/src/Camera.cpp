#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	  m_Yaw(YAW),
	  m_Pitch(PITCH),
	  m_MovementSpeed(SPEED),
	  m_MouseSensitivity(SENSITIVITY)
{
	m_Position = position;
	m_WorldUp = up;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	// m_Position + m_Front = position of what the camera is looking at
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	
	/*
		glm::mat4 translation(1.0f);
		translation[3] = glm::vec4(-m_Position, 1.0f);
		glm::mat4 rotation(1.0f);
		rotation[0] = glm::vec4(m_Right.x, m_Up.x, -m_Front.x, 0.0f);
		rotation[1] = glm::vec4(m_Right.y, m_Up.y, -m_Front.y, 0.0f);
		rotation[2] = glm::vec4(m_Right.z, m_Up.z, -m_Front.z, 0.0f);
		return rotation * translation;
	*/
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
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
	xOffset *= SENSITIVITY;
	yOffset *= SENSITIVITY;

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