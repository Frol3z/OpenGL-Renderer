#include "Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Object.h"

#include <iostream>

Scene::Scene()
{
	m_Camera = new Camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
}

Scene::~Scene()
{
	delete m_Camera;
}

void Scene::Update()
{
	glm::mat4 view = m_Camera->GetViewMatrix();
	glm::mat4 projection = m_Camera->GetProjectionMatrix();

	for (auto& object : m_Objects)
	{
		object->SetViewAndProjectionMatrix(view, projection);
	}
}

void Scene::Draw() const
{
	for (auto& object : m_Objects)
	{
		object->Draw();
	}
}

void Scene::AddObject(Object* obj)
{
	m_Objects.push_back(obj);
}

void Scene::RemoveObject()
{
	m_Objects.pop_back();
}