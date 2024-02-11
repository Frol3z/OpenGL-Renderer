#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ImGuiWindow.h"

void ImGuiWindow::Init(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

	// Enable Classic ImGui style
	ImGui::StyleColorsClassic(&ImGui::GetStyle());

	// Setup Platform/Renderer backends
	// Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiWindow::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiWindow::Update(bool isCursorDisabled)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Cursor handling
	if (isCursorDisabled)
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_Always);
	else
		ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);

	// The actual ImGui window's layout
	ImGui::Begin("ImGui");

	/* ---------- Camera ---------- */
	ImGui::SeparatorText("Camera");
	ImGui::SliderFloat("Speed", &m_CameraSpeed, 0.1f, 100.0f);
	ImGui::SliderFloat("Sensitivity", &m_CameraSensitivity, 0.001f, 5.0f);

	ImGui::SeparatorText("Lights");

	/* ---------- Directional Light ---------- */
	ImGui::SliderFloat3("Directional light direction", m_DirLightDirection, -1.0f, 1.0f);
	ImGui::ColorEdit3("Directional light color", m_DirLightColor);
	ImGui::SliderFloat("Directional light ambient", &m_DirLightAmbient, 0.0f, 1.0f);
	ImGui::SliderFloat("Directional light diffuse", &m_DirLightDiffuse, 0.0f, 1.0f);

	ImGui::SeparatorText("Object");
	ImGui::SliderFloat3("O. position", m_ObjectPosition, -10.0f, 10.0f);
	ImGui::SliderFloat("O. shininess", &m_ObjectShininess, 0.0f, 256.0f);

	ImGui::End();

	// ImGui::ShowDemoWindow();
}

void ImGuiWindow::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}