#include "Box.h"

Box::Box(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

bool Box::start()
{
	//Load Earth texture
	if (!m_texture.load("Lizard.jpg")) 
	{
		printf("Failed to load earth_diffuse.jpg.\n");
		return false;
	}

	//Initialize the mesh
	m_mesh.intializeQuad();

	return true;
}

bool Box::draw()
{
	//Bind texture
	m_texture.bind(0);

	//Draw the box
	m_mesh.draw();

	return false;
}
