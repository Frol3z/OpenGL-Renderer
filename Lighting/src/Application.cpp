#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define LOG(x) std::cout << x << std::endl

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = WIN_WIDTH / 2;
float lastY = WIN_HEIGHT / 2;
bool firstMouse = true;

float fov = 45.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE: 
		{
			if(action == GLFW_PRESS)
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

static void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
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

	camera.ProcessMouseMovement(xOffset, yOffset);
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

static void ProcessCameraInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}
}

static GLFWwindow* CreateWindow() 
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	return glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL", NULL, NULL);
}

int main(void)
{
	GLFWwindow* window;

	// GLFW initialization
	if (!glfwInit()) 
	{
		LOG("Failed to initialize GLFW");
		return -1;
	}

	// GLFW window creation
	window = CreateWindow();
	if (window == NULL)
	{
		LOG("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	// Inputs handling
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// OpenGL context creation
	glfwMakeContextCurrent(window);

	// GLAD initialization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD");
		return -1;
	}

	// Check current OpenGL version and print keyboard mapping
	LOG("OpenGL version: " << glGetString(GL_VERSION));
	LOG("Press ESC to exit");
	LOG("Press G to toggle wireframe mode");
	LOG("Press WASD and mouse to move around (scroll to zoom)");

	// Viewport configuration
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Vertices and indices
	float vertices[] = {
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

	/*
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	*/
	
	// Vertex Buffer
	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout vbl;
	vbl.Push<float>(3);

	// Index Buffer
	// IndexBuffer ib(indices, 6);
	// va.AddIndexBuffer(ib);

	// Vertex Array
	VertexArray va;
	va.AddVertexBuffer(vb, vbl);
	va.Bind();

	// Create shader program
	Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	shader.Use();

	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Computing delta time
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Updating camera
		ProcessCameraInput(window);
		
		// View and projection matrices
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)(WIN_WIDTH / WIN_HEIGHT), 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// Clear buffer
		// glClearColor(0.95f, 0.73f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Box draw call
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);

		shader.setVec3("uColor", glm::vec3(1.0f, 0.5f, 0.31f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// "Light" draw call
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::translate(model, glm::vec3(3.0f, 1.0f, -1.0f));
		shader.setMat4("model", model);

		shader.setVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}