#include "Game.h"
#include "Camera.h"
#include <cstdio>

Game::Game()
{
	//sets default variables for the window
	m_width = 1280;
	m_height = 720;
	m_title = "Grpahics Assessment";
}

Game::Game(int width, int height, const char* title)
{
	//sets variables for the game window
	m_width = width;
	m_height = height;
	m_title = title;
}

Game::~Game()
{

}

int Game::run()
{
	//creates new variables for function and their defaults
	bool running = true;
	bool drawing = true;
	double deltaTime = 0.0f;
	double timeOfPreviousUpdate = 0.0;

	//checks if start isn't true
	if (!start())
	{
		return -1;
	}

	//records the program as it runs
	while (running && drawing)
	{
		//Get the current time
		double timeOfCurrentUpdate = glfwGetTime();
		//find the change in time
		deltaTime = timeOfCurrentUpdate - timeOfPreviousUpdate;
		//And store current time for next loop
		timeOfPreviousUpdate = timeOfCurrentUpdate;

		running = update(deltaTime);
		drawing = draw();
	}

	//checks if end isn't true
	if (!end())
	{
		return -2;
	}

	return 0;
}

bool Game::start()
{
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

	//intitialize the GLFW
	if (!glfwInit())
	{
		return false;
	}

	//Create the window
	m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	//Make sure the window is made
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	//Make the window target
	glfwMakeContextCurrent(m_window);

	//Load the OpenGL function
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	//Print OpenGL Version Number
	int major = ogl_GetMajorVersion();
	int minor = ogl_GetMinorVersion();
	printf("GL Version: %i.%i\n", major, minor);

	//Set the Clearing Color
	glClearColor(0.05f, 0.05f, 0.025f, 1.0f);

	//Enable OpenGl's Depth Test
	glEnable(GL_DEPTH_TEST);

	//Initialize the shader
	m_shader.loadShader(aie::eShaderStage::VERTEX, "phong.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "phong.frag");
	if (!m_shader.link())
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	//Load the object mesh
	if (!m_objMesh.load("Bunny.obj"))
	{
		printf("Failed to load Object Mesh.\n");
		return false;
	}

	//Create the Box
	m_box = new Box({ 5.0f, 0.0f, 5.0f }, glm::vec3(0.0f, 0.0f, 0.0f), { 5.0f, 5.0f, 5.0f });
	m_box->start();

	//Create Identity Transform
	m_meshTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	//Initialize the Gizmos
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	//Set the Camera
	m_camera = new Camera(this);
	m_camera->setPosition({ 10, 10, 10 });
	m_camera->setYaw(-120.0f);
	m_camera->setPitch(-50.0f);

	//Set the Lights
	m_light1.setAmbient({ 0.5f, 0.5f, 0.5f });
	m_light1.setDiffuse({ 3.0f, 3.2f, 2.0f });
	m_light1.setSpecular({ 1.0f, 1.0f, 1.0f });
	m_light2.setAmbient({ 2.0f, 2.0f, 2.0f });
	m_light2.setSpecular({ 0.5f, 0.6f, 1.0f });
	m_light2.setDiffuse({ 2.0f, 2.5f, 2.0f });

	return true;
}

bool Game::update(double deltaTime)
{
	glfwPollEvents();

	//Keeps the Window open until the User closes it
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE))
	{
		return false;
	}

	//Get the Camera's Update function
	m_camera->update(deltaTime);

	//Rotate the Lights
	float time = glfwGetTime();
	m_light1.setDirection(glm::normalize(glm::vec3(glm::cos(time * 2), -1, glm::sin(time * 2))));
	m_light2.setDirection(glm::normalize(glm::vec3(glm::cos(time * 3.0f), 1, glm::sin(time * 3.0f))));

	return true;
}

bool Game::draw()
{
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

	//Clears the Window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Clears the Gizmos
	aie::Gizmos::clear();

	aie::Gizmos::addTransform(mat4(1), 4.0f);

	vec4 white(1, 1, 1, 1);
	vec4 grey(0.5f, 0.5f, 0.5f, 1);

	for (int i = 0; i < 21; ++i)
	{
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : grey);
		aie::Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : grey);
	}

	//Get the Projection and View Matrices
	mat4 projectionMatrix = m_camera->getProjectionMatrix(m_width, m_height);
	mat4 viewMatrix = m_camera->getViewMatrix();

	//Bind Shader
	m_shader.bind();

	//Bind Camera
	m_shader.bindUniform("CameraPosition", m_camera->getPosition());

	//Bind Lights
	m_shader.bindUniform("Ia", m_light1.getAmbient());
	m_shader.bindUniform("Id", m_light1.getDiffuse());
	m_shader.bindUniform("Is", m_light1.getSpecular());
	m_shader.bindUniform("LightDirection", m_light1.getDirection());
	m_shader.bindUniform("Ia", m_light2.getAmbient());
	m_shader.bindUniform("Id", m_light2.getDiffuse());
	m_shader.bindUniform("Is", m_light2.getSpecular());
	m_shader.bindUniform("LightDirection", m_light2.getDirection());

	//Bind and draw the Box
	mat4 pvm = projectionMatrix * viewMatrix * m_box->getTransform();
	m_shader.bindUniform("ProjectionViewModel", pvm);
	m_shader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_box->getTransform())));
	m_shader.bindUniform("ModelMatrix", m_box->getTransform());
	m_box->draw();

	//Draw Object Mesh
	pvm = projectionMatrix * viewMatrix * m_meshTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	m_objMesh.draw();

	aie::Gizmos::draw(projectionMatrix * viewMatrix);

	glfwSwapBuffers(m_window);

	return true;
}

bool Game::end()
{
	//Destroy the Gizmos
	aie::Gizmos::destroy();

	//Close the Window
	glfwDestroyWindow(m_window);

	//Terminate GLFW
	glfwTerminate();
	
	return true;
}
