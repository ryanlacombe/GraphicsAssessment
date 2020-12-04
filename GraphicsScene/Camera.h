#pragma once
#include "Game.h"
#include "Actor.h"
#include <glm/glm.hpp>

class Camera : public Actor
{
public:
	Camera(Game* instance) { m_instance = instance; }
	~Camera() {}

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float width, float height);

	float getYaw();
	void setYaw(float degrees);
	float getPitch();
	void setPitch(float degrees);

	bool update(double deltaTime) override;

private:

	Game* m_instance;

	//Theta
	float m_yaw = 0.0f;
	//Phi
	float m_pitch = 0.0f;

	//Speed variables
	float m_movementSpeed = 10.0f;
	float m_turnSpeed = 0.05f;

	//Mouse positions
	double m_currentMouseX = 0.0f;
	double m_currentMouseY = 0.0f;
	double m_previousMouseX = 0.0f;
	double m_previousMouseY = 0.0f;
};

