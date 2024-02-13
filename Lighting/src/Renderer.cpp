#define GLAD_INCLUDED
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
#include <sstream>
#include <memory>

#include "core/timer/Timer.h"
#include "core/Object.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "vendor/cubesphere/Cubesphere.h"
#include "core/imgui/ImGuiWindow.h"

#include "core/light/DirectionalLight.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// Preprocessors
#define WINDOW_TITLE "OpenGL Renderer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define LOG(x) std::cout << x << std::endl
#define LOG_GLM(x) LOG(glm::to_string(x).c_str())

// Global variables
ImGuiWindow imGui;
GLFWwindow* window;
bool isFullscreen = false;

Timer& timer = Timer::Get();
std::unique_ptr<Scene> scene = std::make_unique<Scene>();
Camera& camera = scene->GetCamera();

float lastX = 0.0f;
float lastY = 0.0f;
bool isFirstMouse = true;
bool isCursorDisabled = true;

static int Init();
static void Shutdown();
static int ShouldClose();
static void UpdatePerformanceDisplay();
static void ProcessCameraInput();
static void ClearBuffers();
static void CheckOpenGLErrors();
static void SceneSetup();

static void OnResize(GLFWwindow* window, int width, int height);
static void OnKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
static void OnMouse(GLFWwindow* window, double xpos, double ypos);
static void OnScroll(GLFWwindow* window, double xoffset, double yoffset);
static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);

static GLFWwindow* CreateWindow();
static void SetWindowIcon(std::string path);
static void PrintDefault();
static const float* Combine(const float* vertices, const float* normals, size_t vertCount, size_t& newSize);

int main()
{
	if (!Init())
	{
		Shutdown();
		return -1;
	}
	
	// Data setup
	SceneSetup();

	/*
		Shader* gouraudShader = new Shader("res/shaders/gouraud.vert", "res/shaders/gouraud.frag");
		Shader* phongShader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
		Shader* lightShader = new Shader("res/shaders/shader.vert", "res/shaders/light.frag");

		Texture* metallicBoxTex = new Texture("res/textures/container.png");
		Texture* metallicBoxSpecularTex = new Texture("res/textures/container_specular.png");
		Texture* emissionTex = new Texture("res/textures/container_emission_2.png");

		std::unique_ptr<Object> defaultObject = std::make_unique<Object>(cubeMesh, phongShader, defaultMaterial);
		defaultObject->SetName("Default cube");
		defaultObject->SetTexture(metallicBoxTex);
		defaultObject->SetSpecularTexture(metallicBoxSpecularTex);
		defaultObject->SetEmissionMap(emissionTex);
	*/

	glEnable(GL_DEPTH_TEST);

	while (!ShouldClose())
	{
		glfwPollEvents();

		// Timer
		timer.Update(glfwGetTime());
		UpdatePerformanceDisplay();

		// ImGui
		imGui.Update(isCursorDisabled, scene.get());
		
		// Inputs
		ProcessCameraInput();

		ClearBuffers();

		// Draw
		scene->Draw();
		CheckOpenGLErrors();
		imGui.Render();

		glfwSwapBuffers(window);
	}

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
	glfwSetMouseButtonCallback(window, OnMouseButton);

	imGui.Init(window);
	
	return 1;
}

static void Shutdown()
{
	imGui.Shutdown();
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
	if(isCursorDisabled)
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

static void CheckOpenGLErrors()
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << error << std::endl;
	}
}

static void SceneSetup()
{
	Cubesphere sphere(1.0f, 3, true);
	size_t sphereVerticesSize = 0;
	const float* sphereVertices = Combine(sphere.getVertices(), sphere.getNormals(), sphere.getVertexCount(), sphereVerticesSize);
	const float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f
	};

	// Meshes
	std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>(cubeVertices, sizeof(cubeVertices), VertexLayout::VFNFTF);
	cubeMesh->SetName("Cube");
	
	std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>(sphereVertices, sphereVerticesSize, VertexLayout::VFNF, sphere.getIndices(), sphere.getIndexSize());
	sphereMesh->SetName("Sphere");

	// Materials
	std::unique_ptr<Material> material = std::make_unique<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f), 32.0f);
	material->SetName("Default");

	// Shaders
	std::unique_ptr<Shader> shader = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
	shader->SetName("Default");
	std::unique_ptr<Shader> gouraud = std::make_unique<Shader>("res/shaders/gouraud.vert", "res/shaders/gouraud.frag");
	gouraud->SetName("Gouraud");

	// Objects
	std::unique_ptr<Object> obj1 = std::make_unique<Object>(cubeMesh.get(), material.get(), shader.get());
	obj1->SetName("Default cube");
	// std::unique_ptr<Object> obj2 = std::make_unique<Object>(sphereMesh.get(), material.get(), shader.get());

	scene->AddMesh(std::move(cubeMesh));
	scene->AddMesh(std::move(sphereMesh));
	scene->AddMaterial(std::move(material));
	scene->AddShader(std::move(shader));
	scene->AddShader(std::move(gouraud));
	scene->AddObject(std::move(obj1));
	// scene->AddObject(std::move(obj2));
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
	case GLFW_KEY_F:
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
	case GLFW_KEY_G:
	{
		if (!isCursorDisabled)
			break;

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

static void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
		LOG("Cannot load window icon :/");
	}

	stbi_image_free(icon[0].pixels);
}

static void PrintDefault()
{
	LOG("OpenGL version: " << glGetString(GL_VERSION));
	LOG("Press ESC to exit");
	LOG("Press F to toggle borderless fullscreen");
	LOG("Right click to toggle mouse visibility");
	LOG("Press G to toggle wireframe mode");
	LOG("Use WASD and mouse to move around (scroll to zoom)");
}

static const float* Combine(const float* vertices, const float* normals, size_t vertCount, size_t& newSize)
{
	// Assuming you have a buffer to store the combined data.
	float* combinedData = new float[vertCount * 6]; // 6 components per vertex (3 for vertex, 3 for normal)

	// Iterate over each vertex and append the corresponding normal.
	for (int i = 0; i < vertCount; ++i) {
		int offset = i * 6; // Each vertex occupies 6 elements in the combined array

		// Copy vertex data
		combinedData[offset] = vertices[i * 3];     // x
		combinedData[offset + 1] = vertices[i * 3 + 1]; // y
		combinedData[offset + 2] = vertices[i * 3 + 2]; // z

		// Copy normal data
		combinedData[offset + 3] = normals[i * 3];     // x
		combinedData[offset + 4] = normals[i * 3 + 1]; // y
		combinedData[offset + 5] = normals[i * 3 + 2]; // z
	}

	newSize = vertCount * 6 * sizeof(float);
	return combinedData;
}