#include "Camera.h"

glm::mat4 Camera::getViewMatrix()
{
	//Convert the degrees to radians
	float yawRad = glm::radians(getYaw());
	float pitchRad = glm::radians(getPitch());

	//Calculate the Forward Vector
	glm::vec3 forward(cos(pitchRad) * cos(yawRad), sin(pitchRad), cos(pitchRad) * sin(yawRad));

	//Return the ViewMatrix
	return glm::lookAt(m_position, m_position + forward, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float width, float height)
{
	//Return the Projection
	return glm::perspective(glm::pi<float>() * 0.25f, width / height, 0.1f, 1000.0f);
}

float Camera::getYaw()
{
	return m_yaw;
}

void Camera::setYaw(float degrees)
{
	m_yaw = degrees;
}

float Camera::getPitch()
{
	return m_pitch;
}

void Camera::setPitch(float degrees)
{
	m_pitch = degrees;
}

bool Camera::update(double deltaTime)
{
	//Set the movement key variables
	int keyForward = GLFW_KEY_W;
	int keyBack = GLFW_KEY_S;
	int keyLeft = GLFW_KEY_A;
	int keyRight = GLFW_KEY_D;
	int keyUp = GLFW_KEY_E;
	int keyDown = GLFW_KEY_Q;

	//Convert the degrees to radians
	float yawRad = glm::radians(getYaw());
	float pitchRad = glm::radians(getPitch());

	//Calculate the Forwards Vector
	glm::vec3 forward(cos(pitchRad) * cos(yawRad), sin(pitchRad), cos(pitchRad) * sin(yawRad));

	//Calculate the Right Vector
	glm::vec3 right(-sin(yawRad), 0.0f, cos(yawRad));

	//Calculate the Up Vector
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	//Check the input
	if (glfwGetKey(m_instance->getWindow(), keyForward))
	{
		//Move the Camera forward
		m_position += forward * m_movementSpeed * (float)deltaTime;
	}
	if (glfwGetKey(m_instance->getWindow(), keyBack))
	{
		//Move the Camera back
		m_position -= forward * m_movementSpeed * (float)deltaTime;
	}
	if (glfwGetKey(m_instance->getWindow(), keyLeft))
	{
		//Move the Camera left
		m_position -= right * m_movementSpeed * (float)deltaTime;
	}
	if (glfwGetKey(m_instance->getWindow(), keyRight))
	{
		//Move the Camera right
		m_position += right * m_movementSpeed * (float)deltaTime;
	}
	if (glfwGetKey(m_instance->getWindow(), keyUp))
	{
		//Move the Camera Up
		m_position += up * m_movementSpeed * (float)deltaTime;
	}
	if (glfwGetKey(m_instance->getWindow(), keyDown))
	{
		//Move the Camera Down
		m_position -= up * m_movementSpeed * (float)deltaTime;
	}

	//Get the Mouse's current coordinates
	glfwGetCursorPos(m_instance->getWindow(), &m_currentMouseX, &m_currentMouseY);
	m_currentMouseX -= m_instance->getWidth() / 2;
	m_currentMouseY -= m_instance->getHeight() / 2;

	//Turn the Camera via change in Mouse Position
	if (glfwGetMouseButton(m_instance->getWindow(), GLFW_MOUSE_BUTTON_1))
	{
		setYaw(getYaw() + m_turnSpeed * (m_currentMouseX - m_previousMouseX));
		setPitch(getPitch() - m_turnSpeed * (m_currentMouseY - m_previousMouseY));
	}

	//Store the previous Mouse coordinates
	m_previousMouseX = m_currentMouseX;
	m_previousMouseY = m_currentMouseY;

	//Return
	return true;
}
