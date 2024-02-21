#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Scene.h"
// @todo fix header dependency between glad and glfw
#include <GLFW/glfw3.h>

class ImGuiWindow
{
public:
	ImGuiWindow() :
		m_SelectedMesh(0), m_SelectedMaterial(0), m_SelectedShader(0), m_Buffer("Unnamed")
	{
	}

public:
	void Init(GLFWwindow* window);
	void Shutdown();
	void Update(bool isCursorDisabled, Scene* scene);
	void Render() const;

private:
	void CreateCameraUI(Camera& camera);
	void CreateDirectionalLightUI(DirectionalLight& dirLight);
	void CreateObjectsUI(Scene* scene);
	void CreatePointLightsUI(Scene* scene);

	template <class T>
	bool CreateCombobox(std::vector<std::unique_ptr<T>>& vector, int* selected, std::string&& text);

private:
	int m_SelectedMesh;
	int m_SelectedMaterial;
	int m_SelectedShader;
	char m_Buffer[128];
};
