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

	/* ---------- Camera ---------- */
	ImGui::SeparatorText("Camera");

	auto& camera = scene->GetCamera();

	// Speed
	float speed = camera.GetSpeed();
	if (ImGui::SliderFloat("Speed", &speed, 0.1f, 100.0f))
		camera.SetSpeed(speed);

	// Sensitivity
	float sensitivity = camera.GetSensitivity();
	if (ImGui::SliderFloat("Sensitivity", &sensitivity, 0.001f, 5.0f))
		camera.SetSensitivity(sensitivity);

	ImGui::SeparatorText("Lights");

	/* ---------- Directional Light ---------- */
	auto& dirLight = scene->GetDirectionalLight();

	// Direction
	float direction[3] = { dirLight.GetDirection().x, dirLight.GetDirection().y, dirLight.GetDirection().z };
	if (ImGui::SliderFloat3("Direction", direction, -1.0f, 1.0f))
		dirLight.SetDirection(glm::vec3(direction[0], direction[1], direction[2]));

	float color[3] = { dirLight.GetColor().r, dirLight.GetColor().g, dirLight.GetColor().b };
	float intensity = dirLight.GetIntensity();
	const float ambStrength = 0.2f;
	const float difStrength = 0.5f;
	// Color
	if(ImGui::ColorEdit3("Color", color))
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
	}

	/* ---------- Scene ---------- */
	ImGui::SeparatorText("Scene");

	auto& objects = scene->GetObjects();
	
	// Add object button
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
	if (ImGui::Button("Add object"))
	{
		// @todo
		std::cout << "Add object" << std::endl;
	}

	// Scene tree
	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Scene"))
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

				// Delete button
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.66f, 0.2f, 0.2f, 1.0f));
				if (ImGui::Button("Remove"))
				{
					scene->RemoveObject(objects[i]);
				}
				ImGui::PopStyleColor();

				ImGui::PopID();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();

	// ImGui::ShowDemoWindow();
}

void ImGuiWindow::Render() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}