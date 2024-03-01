#include "ImGuiWindow.h"

#include "common/Logger.hpp"
#include "common/FileDialog.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void ImGuiWindow::Init(GLFWwindow* window)
{
	m_GlfwWindow = window;

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
	ImGui_ImplGlfw_InitForOpenGL(m_GlfwWindow, true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiWindow::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiWindow::Update(bool isCursorDisabled, Scene* scene)
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

	// UI Layout
	ImGui::Begin("Editor (right-click to open)", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	
	CreateMenuBar(scene);

	if (ImGui::CollapsingHeader("Models"))
		CreateModelsUI(scene);

	ImGui::End();

	// ImGui::ShowDemoWindow();
}

void ImGuiWindow::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWindow::CreateMenuBar(Scene* scene)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import..."))
			{
				std::string path = FileDialog::Open(m_GlfwWindow, "All Files\0*.*\0\0");
				Logger::Get().Info(path);

				scene->AddModel(std::move(std::make_unique<Model>(path)));
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void ImGuiWindow::CreateModelsUI(Scene* scene)
{
	auto& models = scene->GetModels();

	// Scene tree
	for (int i = 0; i < models.size(); i++)
	{
		ImGui::PushID("Model " + i);

		if (i == 0)
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		if (ImGui::TreeNode((void*)(intptr_t)i, "Model", i))
		{

			ImGui::SeparatorText("Properties");

			// Model position
			auto modelPos = models[i]->GetPosition();
			float position[3] = { modelPos.x, modelPos.y, modelPos.z };
			if (ImGui::DragFloat3("Position", position))
				models[i]->SetPosition(glm::vec3(position[0], position[1], position[2]));

			// Model rotation
			auto modelRot = models[i]->GetRotation();
			float rotation[3] = { modelRot.x, modelRot.y, modelRot.z };
			if (ImGui::DragFloat3("Rotation", rotation, 1.0f, -180.0f, 180.0f))
				models[i]->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

			// Object scale
			bool& isUniformScaling = models[i]->isUniformScaling;
			auto modelScale = models[i]->GetScale();
			float scale[3] = { modelScale.x, modelScale.y, modelScale.z };
			if (ImGui::DragFloat3("Scale", scale, 0.1f, 0.1f, 10.0f))
			{
				if (isUniformScaling)
				{
					// Synchronizing XYZ sliders based on the one that is being modified
					if (scale[0] != modelScale.x)
					{
						scale[1] = scale[0];
						scale[2] = scale[0];
					}
					else if (scale[1] != modelScale.y)
					{
						scale[0] = scale[1];
						scale[2] = scale[1];
					}
					else if (scale[2] != modelScale.z)
					{
						scale[0] = scale[2];
						scale[1] = scale[2];
					}

					models[i]->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
				}
				else
				{
					models[i]->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
				}

			}

			ImGui::SameLine();
			ImGui::Checkbox("isUniform", &isUniformScaling);

			// Delete button
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.66f, 0.2f, 0.2f, 1.0f));
			if (ImGui::Button("Remove"))
			{
				scene->RemoveModel(i);
			}
			ImGui::PopStyleColor();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}