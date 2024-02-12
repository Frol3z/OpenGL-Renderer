#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Scene.h"
// @todo fix header dependency between glad and glfw
#include <GLFW/glfw3.h>

class ImGuiWindow
{
public:
	ImGuiWindow() = default;

public:
	void Init(GLFWwindow* window);
	void Shutdown();
	void Update(bool isCursorDisabled, Scene* scene);
	void Render() const;
};
