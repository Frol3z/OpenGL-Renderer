#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
#include <sstream>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "common/timer/Timer.hpp"
#include "common/logger/Logger.hpp"

#include "core/imgui/ImGuiWindow.h"

#define WINDOW_TITLE "OpenGL Renderer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// @todo remove
#define LOG_GLM(x) LOG(glm::to_string(x).c_str())

// Window
ImGuiWindow imGui;
GLFWwindow* window;
bool isFullscreen = false;
bool isCursorDisabled = true;

Timer& timer = Timer::Get();
Logger logger;

static int Init();
static void Shutdown();
static void UpdatePerformanceDisplay();
static void ClearBuffers();

static void OnResize(GLFWwindow* window, int width, int height);
static void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
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
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		timer.Update(glfwGetTime());
		UpdatePerformanceDisplay();

		imGui.Update(isCursorDisabled);

		ClearBuffers();
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
	std::stringstream ss;
	ss << WINDOW_TITLE << " " 
	   << timer.GetMSPF() << "ms "
	   << timer.GetFPS() << "fps";

	glfwSetWindowTitle(window, ss.str().c_str());
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