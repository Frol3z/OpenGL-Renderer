#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
#include <sstream>

#include "timer/Timer.h"
#include "structures/Object.h"
#include "structures/Camera.h"
#include "structures/Scene.h"

// Preprocessors
#define WINDOW_TITLE "OpenGL Renderer"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LOG(x) std::cout << x << std::endl

// Global variables
GLFWwindow* window;
Timer& timer = Timer::Get();
Scene scene;

float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

static int Init();
static void Shutdown();
static int ShouldClose();
static void UpdatePerformanceDisplay();
static void ProcessCameraInput();

static void OnResize(GLFWwindow* window, int width, int height);
static void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
static void OnMouse(GLFWwindow* window, double xpos, double ypos);
static void OnScroll(GLFWwindow* window, double xoffset, double yoffset);

static GLFWwindow* CreateWindow();
static void SetWindowIcon(std::string path);
static void PrintDefault();

int main()
{
	if (!Init())
	{
		Shutdown();
		return -1;
	}

	const float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	};

	const unsigned int indices[] = {
		0, 1, 2
	};

	Object* obj = new Object(vertices, sizeof(vertices), indices, sizeof(indices));
	obj->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	obj->SetRotation(20, glm::vec3(1.0f, 0.0f, 0.0f));

	Object* obj1 = new Object(vertices, sizeof(vertices), indices, sizeof(indices));
	obj1->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	obj1->SetScale(glm::vec3(0.2f));

	Object* obj2 = new Object(vertices, sizeof(vertices), indices, sizeof(indices));
	obj2->SetPosition(glm::vec3(5.0f, 2.0f, 1.0f));
	obj2->SetScale(glm::vec3(5.0f));
	
	scene.AddObject(obj);
	scene.AddObject(obj1);
	scene.AddObject(obj2);

	glEnable(GL_DEPTH_TEST);

	while (!ShouldClose())
	{
		timer.Update(glfwGetTime());
		UpdatePerformanceDisplay();
		
		// Process WASD inputs
		ProcessCameraInput();
		
		scene.Update();

		glClearColor(0.95f, 0.73f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		obj2->SetRotation(glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		scene.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Free heap
	delete obj;
	delete obj1;
	delete obj2;

	Shutdown();
	return 0;
}

static int Init()
{
	// GLFW init
	if (!glfwInit())
	{
		LOG("Failed to initialize GLFW :/");
		return 0;
	}

	// Window creation
	window = CreateWindow();
	if (window == NULL)
	{
		LOG("Failed to initialize GLFW window :/");
		return 0;
	}
	SetWindowIcon("res/icon/icon.png");

	// OpenGL context
	glfwMakeContextCurrent(window);

	// GLAD init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD");
		return 0;
	}

	PrintDefault();

	// Viewport init
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnResize);

	// Input init
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, OnKeyboard);
	glfwSetCursorPosCallback(window, OnMouse);
	glfwSetScrollCallback(window, OnScroll);
	
	return 1;
}

static void Shutdown()
{
	glfwTerminate();
}

static int ShouldClose()
{
	return glfwWindowShouldClose(window);
}

static void UpdatePerformanceDisplay()
{
	std::stringstream ss;
	ss << WINDOW_TITLE << " " 
	   << timer.GetMSPF() << "ms "
	   << timer.GetFPS() << "fps";

	glfwSetWindowTitle(window, ss.str().c_str());
}

static void ProcessCameraInput()
{
	auto* camera = scene.GetCamera();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraMovement::FORWARD, timer.GetDeltaTime());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraMovement::BACKWARD, timer.GetDeltaTime());
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraMovement::LEFT, timer.GetDeltaTime());
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(CameraMovement::RIGHT, timer.GetDeltaTime());
	}
}

static GLFWwindow* CreateWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
}

static void SetWindowIcon(std::string path)
{
	GLFWimage icon[1];
	icon[0].pixels = stbi_load(path.c_str(), &icon[0].width, &icon[0].height, 0, 4);

	if (icon[0].pixels)
	{
		glfwSetWindowIcon(window, 1, icon);
	}
	else
	{
		LOG("Cannot load window icon :/");
	}

	stbi_image_free(icon[0].pixels);
}

static void PrintDefault()
{
	LOG("OpenGL version: " << glGetString(GL_VERSION));
	LOG("Press ESC to exit");
	LOG("Press G to toggle wireframe mode");
	LOG("Press UP/DOWN arrows to INCREASE/DECREASE second texture's visibility");
	LOG("Press WASD and mouse to move around (scroll to zoom)");
}

static void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		{
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			break;
		}
		case GLFW_KEY_G:
		{
			if (action == GLFW_PRESS)
			{
				// Toggle wireframe mode
				GLint polygonMode;
				glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

				if (polygonMode == GL_FILL)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					LOG("Wireframe mode: ON");
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					LOG("Wireframe mode: OFF");
				}
			}
			break;
		}
	}
}

static void OnMouse(GLFWwindow* window, double xpos, double ypos)
{
	auto* camera = scene.GetCamera();

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xOffset, yOffset);
}

static void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	auto* camera = scene.GetCamera();

	camera->UpdateFOV(yoffset);
}