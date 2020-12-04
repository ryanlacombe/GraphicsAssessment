#include "Box.h"

Box::Box(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

bool Box::start()
{
	//Initialize the mesh
	m_mesh.intializeQuad();

	return true;
}

bool Box::draw()
{
	//Draw the box
	m_mesh.draw();

	return false;
}
