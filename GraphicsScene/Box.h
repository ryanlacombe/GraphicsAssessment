#pragma once
#include <Texture.h>
#include "Actor.h"
#include "Mesh.h"

class Box : public Actor
{

public:
	Box(glm::vec3 position, glm::quat rotation, glm::vec3 scale = { 1.0f, 1.0f, 1.0f });

	bool start() override;
	bool draw() override;

private:

	Mesh m_mesh;
	aie::Texture m_texture;
};

