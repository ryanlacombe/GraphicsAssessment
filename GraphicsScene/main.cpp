#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include "Game.h"

int main() 
{
	Game* app = new Game(1920, 1080, "Graphics Assessment");

	app->run();

	delete app;

	return 0;
}