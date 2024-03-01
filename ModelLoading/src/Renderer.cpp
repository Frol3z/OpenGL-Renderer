#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
#include <sstream>
#include <memory>

#include "common/Logger.hpp"
#include "common/Timer.hpp"

#include "core/imgui/ImGuiWindow.h"
#include "core/Shader.h"
#include "core/Scene.h"

#define WINDOW_TITLE "OpenGL Renderer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Window
ImGuiWindow imGui;
GLFWwindow* window;
bool isFullscreen = false;

// Mouse input
float lastX = 0.0f;
float lastY = 0.0f;
bool isFirstMouse = true;
bool isCursorDisabled = true;

Timer& timer = Timer::Get();
Logger& logger = Logger::Get();
std::unique_ptr<Scene> scene = std::make_unique<Scene>();
Camera& camera = scene->GetCamera();

static int Init();
static void Shutdown();
static void UpdatePerformanceDisplay();
static void ProcessCameraInput();
static void ClearBuffers();

static void OnResize(GLFWwindow* window, int width, int height);
static void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
static void OnMouse(GLFWwindow* window, double xpos, double ypos);
static void OnScroll(GLFWwindow* window, double xoffset, double yoffset);
static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);

static GLFWwindow* CreateWindow();
static void SetWindowIcon(std::string path);
int main()
{
	if (!Init())
	{
		Shutdown();
		return -1;
	}
	
	Shader shader("res/shaders/default.vert", "res/shaders/default.frag");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		UpdatePerformanceDisplay();
		imGui.Update(isCursorDisabled, scene.get());
		ProcessCameraInput();

		ClearBuffers();

		scene->Draw(shader);
		imGui.Render();
		glfwSwapBuffers(window);
	}

	Shutdown();
	return 0;
}

static int Init()
{
	// Logger init
	logger.SetLevel(Logger::LEVEL_INFO);

	// GLFW init
	if (!glfwInit())
	{
		logger.Error("Failed to initialize GLFW :/)");
		return 0;
	}

	// Window creation
	window = CreateWindow();
	if (window == NULL)
	{
		logger.Error("Failed to initialize GLFW window :/");
		return 0;
	}
	SetWindowIcon("res/icon/icon.png");

	// OpenGL context
	glfwMakeContextCurrent(window);

	// GLAD init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger.Error("Failed to initialize GLAD");
		return 0;
	}

	// Viewport init
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnResize);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Input init
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, OnKeyboard);
	glfwSetCursorPosCallback(window, OnMouse);
	glfwSetScrollCallback(window, OnScroll);
	glfwSetMouseButtonCallback(window, OnMouseButton);

	imGui.Init(window);
	
	return 1;
}

static void Shutdown()
{
	imGui.Shutdown();
	glfwTerminate();
}

static void UpdatePerformanceDisplay()
{
	timer.Update(glfwGetTime());

	std::stringstream ss;
	ss << WINDOW_TITLE << " " 
	   << timer.GetMSPF() << "ms "
	   << timer.GetFPS() << "fps";

	glfwSetWindowTitle(window, ss.str().c_str());
}

static void ProcessCameraInput()
{
	if (isCursorDisabled)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(CameraMovement::FORWARD, timer.GetDeltaTime());
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(CameraMovement::BACKWARD, timer.GetDeltaTime());
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(CameraMovement::LEFT, timer.GetDeltaTime());
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(CameraMovement::RIGHT, timer.GetDeltaTime());
		}
	}
}

static void ClearBuffers()
{
	// glClearColor(0.95f, 0.73f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		case GLFW_KEY_F11:
		{
			if (!isCursorDisabled)
				break;

			if (action == GLFW_PRESS)
			{
				if (isFullscreen)
				{
					glfwSetWindowMonitor(window, NULL, 200, 200, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
					isFullscreen = false;
				}
				else
				{
					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);

					glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					isFullscreen = true;
				}
			}

			break;
		}
	}
}

static void OnMouse(GLFWwindow* window, double xpos, double ypos)
{
	if (isFirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if (isCursorDisabled)
		camera.ProcessMouseMovement(xOffset, yOffset);
}

static void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	if (isCursorDisabled)
		camera.UpdateFOV(yoffset);
}

static void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (!isCursorDisabled)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			isCursorDisabled = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			isCursorDisabled = false;
		}
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
		logger.Warning("Cannot load window icon :/");
	}

	stbi_image_free(icon[0].pixels);
}