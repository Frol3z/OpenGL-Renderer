#pragma once

#include <vector>
#include <memory>
// @todo fix header dependency between glad and glfw
#include <GLFW/glfw3.h>

#include "core/Scene.h"

class ImGuiWindow
{
public:
	ImGuiWindow() = default;

public:
	void Init(GLFWwindow* window);
	void Shutdown();
	void Update(bool isCursorDisabled, Scene* scene);
	void Render() const;

private:
	GLFWwindow* m_GlfwWindow = nullptr;

private:
	void CreateMenuBar(Scene* scene);
	void CreateModelsUI(Scene* scene);
};
