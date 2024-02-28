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

#define WINDOW_TITLE "OpenGL Renderer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// @todo Move them in a common folder/file
#define LOG(x) std::cout << x << std::endl
#define LOG_GLM(x) LOG(glm::to_string(x).c_str())

// Window
ImGuiWindow imGui;
GLFWwindow* window;
bool isFullscreen = false;

// Renderer
Timer& timer = Timer::Get();
std::unique_ptr<Scene> scene = std::make_unique<Scene>();
Camera& camera = scene->GetCamera();

// Mouse input
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
	
	SceneSetup();
	glEnable(GL_DEPTH_TEST);

	while (!ShouldClose())
	{
		glfwPollEvents();

		// Timer
		timer.Update(glfwGetTime());
		UpdatePerformanceDisplay();

		// UI
		imGui.Update(isCursorDisabled, scene.get());
		
		// Inputs
		ProcessCameraInput();

		// Draw
		ClearBuffers();
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
	std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>("Cube", cubeVertices, sizeof(cubeVertices), VertexLayout::VFNFTF);
	std::unique_ptr<Mesh> sphereMesh = std::make_unique<Mesh>("Sphere", sphereVertices, sphereVerticesSize, VertexLayout::VFNF, sphere.getIndices(), sphere.getIndexSize());

	// Textures
	std::unique_ptr<Texture> container = std::make_unique<Texture>("res/textures/container.png");
	std::unique_ptr<Texture> containerSpec = std::make_unique<Texture>("res/textures/container_specular.png");
	std::unique_ptr<Texture> containerEmis = std::make_unique<Texture>("res/textures/container_emission_2.png");
	std::unique_ptr<Texture> wall = std::make_unique<Texture>("res/textures/wall.jpg");
	std::unique_ptr<Texture> cobblestone = std::make_unique<Texture>("res/textures/cobblestone.jpg");

	// Non-textured materials
	std::unique_ptr<Material> defaultMat = std::make_unique<Material>("Default");
	std::unique_ptr<Material> emeraldMat = std::make_unique<Material>("Emerald", glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 128 * 0.6f);
	std::unique_ptr<Material> jadeMat = std::make_unique<Material>("Jade", glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 128 * 0.1f);
	std::unique_ptr<Material> obsidianMat = std::make_unique<Material>("Obsidian", glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 128 * 0.3f);
	std::unique_ptr<Material> pearlMat = std::make_unique<Material>("Pearl", glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 128 * 0.088f);
	std::unique_ptr<Material> rubyMat = std::make_unique<Material>("Ruby", glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 128 * 0.6f);
	std::unique_ptr<Material> turquoiseMat = std::make_unique<Material>("Turquoise", glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 128 * 0.1f);
	std::unique_ptr<Material> brassMat = std::make_unique<Material>("Brass", glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 128 * 0.21794872f);
	std::unique_ptr<Material> bronzeMat = std::make_unique<Material>("Bronze", glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 128 * 0.2f);
	std::unique_ptr<Material> chromeMat = std::make_unique<Material>("Chrome", glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 128 * 0.6f);
	std::unique_ptr<Material> copperMat = std::make_unique<Material>("Copper", glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 128 * 0.1f);
	std::unique_ptr<Material> goldMat = std::make_unique<Material>("Gold", glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 128 * 0.4f);
	std::unique_ptr<Material> silverMat = std::make_unique<Material>("Silver", glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 128 * 0.4f);
	std::unique_ptr<Material> blackPlasticMat = std::make_unique<Material>("Black Plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.50f, 0.50f, 0.50f), 128 * 0.25f);
	std::unique_ptr<Material> cyanPlasticMat = std::make_unique<Material>("Cyan Plastic", glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f), glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 128 * 0.25f);
	std::unique_ptr<Material> greenPlasticMat = std::make_unique<Material>("Green Plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 128 * 0.25f);
	std::unique_ptr<Material> redPlasticMat = std::make_unique<Material>("Red Plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 128 * 0.25f);
	std::unique_ptr<Material> whitePlasticMat = std::make_unique<Material>("White Plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.70f, 0.70f, 0.70f), 128 * 0.25f);
	std::unique_ptr<Material> yellowPlasticMat = std::make_unique<Material>("Yellow Plastic", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.60f, 0.60f, 0.50f), 128 * 0.25f);
	std::unique_ptr<Material> blackRubberMat = std::make_unique<Material>("Black Rubber", glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.4f, 0.4f, 0.4f), 128 * 0.078125f);
	std::unique_ptr<Material> cyanRubberMat = std::make_unique<Material>("Cyan Rubber", glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f), glm::vec3(0.04f, 0.7f, 0.7f), 128 * 0.078125f);
	std::unique_ptr<Material> greenRubberMat = std::make_unique<Material>("Green Rubber", glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f), glm::vec3(0.04f, 0.7f, 0.04f), 128 * 0.078125f);
	std::unique_ptr<Material> redRubberMat = std::make_unique<Material>("Red Rubber", glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f), glm::vec3(0.7f, 0.04f, 0.04f), 128 * 0.078125f);
	std::unique_ptr<Material> whiteRubberMat = std::make_unique<Material>("White Rubber", glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), 128 * 0.078125f);
	std::unique_ptr<Material> yellowRubberMat = std::make_unique<Material>("Yellow Rubber", glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.7f, 0.7f, 0.04f), 128 * 0.078125f);

	// Textures materials
	std::unique_ptr<Material> containerMat = std::make_unique<Material>("Container", container.get(), containerSpec.get(), 32.0f);
	containerMat->SetEmissionMap(containerEmis.get());
	std::unique_ptr<Material> wallMat = std::make_unique<Material>("Wall", wall.get(), nullptr, 32.0f);
	std::unique_ptr<Material> cobblestoneMat = std::make_unique<Material>("Cobblestone", cobblestone.get(), nullptr, 32.0f);

	// Shaders
	std::unique_ptr<Shader> shader = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
	shader->SetName("Default");
	std::unique_ptr<Shader> gouraud = std::make_unique<Shader>("res/shaders/gouraud.vert", "res/shaders/gouraud.frag");
	gouraud->SetName("Gouraud (non-textured only)");
	std::unique_ptr<Shader> flat = std::make_unique<Shader>("res/shaders/flat.vert", "res/shaders/flat.frag");
	flat->SetName("Flat (non-textured only)");
	std::unique_ptr<Shader> gooch = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/gooch.frag");
	gooch->SetName("Gooch (currently not working)");
	std::unique_ptr<Shader> pointLightShader = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/point_light.frag");
	pointLightShader->SetName("Point light (to remove)");

	// Point lights
	std::unique_ptr<PointLight> pl1 = std::make_unique<PointLight>("Red point light", sphereMesh.get(), defaultMat.get(), pointLightShader.get());
	pl1->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	pl1->SetScale(glm::vec3(0.2));
	pl1->SetColor(glm::vec3(0.7f, 0.0f, 0.0f));
	pl1->SetAmbient(0.2f * glm::vec3(0.7f, 0.0f, 0.0f));
	pl1->SetDiffuse(0.5f * glm::vec3(0.7f, 0.0f, 0.0f));
	pl1->SetSpecular(glm::vec3(0.7f, 0.0f, 0.0f));

	std::unique_ptr<PointLight> pl2 = std::make_unique<PointLight>("Green point light", sphereMesh.get(), defaultMat.get(), pointLightShader.get());
	pl2->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	pl2->SetScale(glm::vec3(0.2));
	pl2->SetColor(glm::vec3(0.0f, 0.7f, 0.0f));
	pl2->SetAmbient(0.2f * glm::vec3(0.0f, 0.7f, 0.0f));
	pl2->SetDiffuse(0.5f * glm::vec3(0.0f, 0.7f, 0.0f));
	pl2->SetSpecular(glm::vec3(0.0f, 0.7f, 0.0f));

	std::unique_ptr<PointLight> pl3 = std::make_unique<PointLight>("Blue point light", sphereMesh.get(), defaultMat.get(), pointLightShader.get());
	pl3->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pl3->SetScale(glm::vec3(0.2));
	pl3->SetColor(glm::vec3(0.0f, 0.0f, 0.7f));
	pl3->SetAmbient(0.2f * glm::vec3(0.0f, 0.0f, 0.7f));
	pl3->SetDiffuse(0.5f * glm::vec3(0.0f, 0.0f, 0.7f));
	pl3->SetSpecular(glm::vec3(0.0f, 0.0f, 0.7f));

	// Objects
	std::unique_ptr<Object> obj1 = std::make_unique<Object>("Default cube", cubeMesh.get(), defaultMat.get(), shader.get());
	std::unique_ptr<Object> obj2 = std::make_unique<Object>("Default floor", cubeMesh.get(), defaultMat.get(), shader.get());
	obj2->SetScale(glm::vec3(10.0f, 0.1f, 10.0f));
	obj2->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	scene->AddMesh(std::move(cubeMesh));
	scene->AddMesh(std::move(sphereMesh));

	scene->AddTexture(std::move(container));
	scene->AddTexture(std::move(containerSpec));
	scene->AddTexture(std::move(containerEmis));
	scene->AddTexture(std::move(wall));
	scene->AddTexture(std::move(cobblestone));

	scene->AddMaterial(std::move(defaultMat));
	scene->AddMaterial(std::move(emeraldMat));
	scene->AddMaterial(std::move(jadeMat));
	scene->AddMaterial(std::move(obsidianMat));
	scene->AddMaterial(std::move(pearlMat));
	scene->AddMaterial(std::move(rubyMat));
	scene->AddMaterial(std::move(turquoiseMat));
	scene->AddMaterial(std::move(brassMat));
	scene->AddMaterial(std::move(bronzeMat));
	scene->AddMaterial(std::move(chromeMat));
	scene->AddMaterial(std::move(copperMat));
	scene->AddMaterial(std::move(goldMat));
	scene->AddMaterial(std::move(silverMat));
	scene->AddMaterial(std::move(blackPlasticMat));
	scene->AddMaterial(std::move(cyanPlasticMat));
	scene->AddMaterial(std::move(greenPlasticMat));
	scene->AddMaterial(std::move(redPlasticMat));
	scene->AddMaterial(std::move(whitePlasticMat));
	scene->AddMaterial(std::move(yellowPlasticMat));
	scene->AddMaterial(std::move(blackRubberMat));
	scene->AddMaterial(std::move(cyanRubberMat));
	scene->AddMaterial(std::move(greenRubberMat));
	scene->AddMaterial(std::move(redRubberMat));
	scene->AddMaterial(std::move(whiteRubberMat));
	scene->AddMaterial(std::move(yellowRubberMat));

	scene->AddMaterial(std::move(containerMat));
	scene->AddMaterial(std::move(wallMat));
	scene->AddMaterial(std::move(cobblestoneMat));

	scene->AddShader(std::move(shader));
	scene->AddShader(std::move(gouraud));
	scene->AddShader(std::move(flat));
	scene->AddShader(std::move(gooch));
	scene->AddShader(std::move(pointLightShader));

	scene->AddPointLight(std::move(pl1));
	scene->AddPointLight(std::move(pl2));
	scene->AddPointLight(std::move(pl3));

	scene->AddObject(std::move(obj1));
	scene->AddObject(std::move(obj2));
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
	case GLFW_KEY_F:
	{
		if (!isCursorDisabled)
			break;

		if (action == GLFW_PRESS)
		{
			scene->ToggleFlashlight();
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
	LOG("Press F11 to toggle borderless fullscreen");
	LOG("Right click to toggle mouse visibility");
	LOG("Press G to toggle wireframe mode");
	LOG("Press F to toggle flashlight");
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