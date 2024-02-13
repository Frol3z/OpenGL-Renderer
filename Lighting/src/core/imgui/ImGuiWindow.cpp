#include "ImGuiWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

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

	// The actual ImGui window's layout
	ImGui::Begin("ImGui");

	CreateCameraUI(scene->GetCamera());
	CreateDirectionalLightUI(scene->GetDirectionalLight());
	CreateObjectsUI(scene);

	ImGui::End();

	ImGui::ShowDemoWindow();
}

void ImGuiWindow::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWindow::CreateCameraUI(Camera& camera)
{
	ImGui::SeparatorText("Camera");

	// Speed
	float speed = camera.GetSpeed();
	if (ImGui::SliderFloat("Speed", &speed, 0.1f, 100.0f))
		camera.SetSpeed(speed);

	// Sensitivity
	float sensitivity = camera.GetSensitivity();
	if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.001f, 5.0f))
		camera.SetSensitivity(sensitivity);
}

void ImGuiWindow::CreateDirectionalLightUI(DirectionalLight& dirLight)
{
	ImGui::SeparatorText("Lights");

	// Direction
	float direction[3] = { dirLight.GetDirection().x, dirLight.GetDirection().y, dirLight.GetDirection().z };
	if (ImGui::SliderFloat3("Direction", direction, -1.0f, 1.0f))
		dirLight.SetDirection(glm::vec3(direction[0], direction[1], direction[2]));

	float color[3] = { dirLight.GetColor().r, dirLight.GetColor().g, dirLight.GetColor().b };
	float intensity = dirLight.GetIntensity();
	const float ambStrength = 0.2f;
	const float difStrength = 0.5f;

	// Color
	if (ImGui::ColorEdit3("Color", color))
	{
		dirLight.SetColor(glm::vec3(color[0], color[1], color[2]));
		dirLight.SetAmbient(glm::vec3(intensity * ambStrength * color[0], intensity * ambStrength * color[1], intensity * ambStrength * color[2]));
		dirLight.SetDiffuse(glm::vec3(intensity * difStrength * color[0], intensity * difStrength * color[1], intensity * difStrength * color[2]));
		dirLight.SetSpecular(glm::vec3(color[0], color[1], color[2]));
	}

	// Intensity
	if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f))
	{
		dirLight.SetIntensity(intensity);
		dirLight.SetAmbient(glm::vec3(intensity * ambStrength * color[0], intensity * ambStrength * color[1], intensity * ambStrength * color[2]));
		dirLight.SetDiffuse(glm::vec3(intensity * difStrength * color[0], intensity * difStrength * color[1], intensity * difStrength * color[2]));
		dirLight.SetSpecular(glm::vec3(intensity * color[0], intensity * color[1], intensity * color[2]));
	}
}

void ImGuiWindow::CreateObjectsUI(Scene* scene)
{
	ImGui::SeparatorText("Scene");

	auto& objects = scene->GetObjects();

	// Add object button
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
	if (ImGui::Button("Add object"))
		ImGui::OpenPopup("New object");

	// Add object modal
	if (ImGui::BeginPopupModal("New object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
	{
		ImGui::InputText("Name", m_Buffer, sizeof(m_Buffer));

		CreateCombobox(scene->GetMeshes(), &m_SelectedMesh, "Mesh");
		CreateCombobox(scene->GetMaterials(), &m_SelectedMaterial, "Material");
		CreateCombobox(scene->GetShaders(), &m_SelectedShader, "Shader");

		// Confirm button
		if (ImGui::Button("OK"))
		{
			// Creating the new object
			std::unique_ptr<Object> newObject = std::make_unique<Object>(
				scene->GetMeshes().at(m_SelectedMesh).get(),
				scene->GetMaterials().at(m_SelectedMaterial).get(),
				scene->GetShaders().at(m_SelectedShader).get()
			);

			newObject->SetName(std::string(m_Buffer));

			scene->AddObject(std::move(newObject));

			m_SelectedMesh = 0;
			m_SelectedMaterial = 0;
			m_SelectedShader = 0;
			std::memset(m_Buffer, 0, sizeof(m_Buffer));
			strcpy_s(m_Buffer, sizeof(m_Buffer), "Unnamed");
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		// Cancel button
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	// Scene tree
	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Object list"))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (ImGui::TreeNode((void*)(intptr_t)i, objects[i]->GetName().c_str(), i))
			{
				ImGui::PushID(i);

				// Object position
				auto objectPos = objects[i]->GetPosition();
				float position[3] = { objectPos.x, objectPos.y, objectPos.z };
				if (ImGui::SliderFloat3("Position", position, -10.0f, 10.0f))
					objects[i]->SetPosition(glm::vec3(position[0], position[1], position[2]));

				// Object rotation
				auto objectRot = objects[i]->GetRotation();
				float rotation[3] = { objectRot.x, objectRot.y, objectRot.z };
				if (ImGui::SliderFloat3("Rotation", rotation, -180.0f, 180.0f))
					objects[i]->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));

				// Object scale
				bool& isUniformScaling = objects[i]->isUniformScaling;
				auto objectScale = objects[i]->GetScale();
				float scale[3] = { objectScale.x, objectScale.y, objectScale.z };
				if (ImGui::SliderFloat3("Scale", scale, 0.1f, 10.0f))
				{
					if (isUniformScaling)
					{
						// Synchronizing XYZ sliders based on the one that is being modified
						if (scale[0] != objectScale.x)
						{
							scale[1] = scale[0];
							scale[2] = scale[0];
						}
						else if (scale[1] != objectScale.y)
						{
							scale[0] = scale[1];
							scale[2] = scale[1];
						}
						else if (scale[2] != objectScale.z)
						{
							scale[0] = scale[2];
							scale[1] = scale[2];
						}

						objects[i]->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
					}
					else
					{
						objects[i]->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
					}

				}

				ImGui::SameLine();
				ImGui::Checkbox("isUniform", &isUniformScaling);

				ImGui::Separator();

				// Delete button
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.66f, 0.2f, 0.2f, 1.0f));
				if (ImGui::Button("Remove"))
				{
					scene->RemoveObject(i);
				}
				ImGui::PopStyleColor();

				ImGui::PopID();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

template <class T>
bool ImGuiWindow::CreateCombobox(std::vector<std::unique_ptr<T>>& vector, int* selected, std::string&& text)
{
	bool ret = false;

	ImGui::PushID(text.c_str());
	std::vector<const char*> names;
	for (const auto& el : vector)
	{
		names.push_back(el->GetName().c_str());
	}

	if (!names.empty())
		ret = ImGui::Combo(text.c_str(), selected, names.data(), static_cast<int>(names.size()));
	else
		ImGui::Text("No %s available", text);

	ImGui::PopID();

	return ret;
}