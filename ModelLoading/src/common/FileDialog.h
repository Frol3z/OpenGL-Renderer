#pragma once

#include <string>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class FileDialog
{
public:
	static std::string Open(GLFWwindow* window, const std::string& filter);
};