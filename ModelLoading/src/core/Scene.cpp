#include "Scene.h"

#include <glm/glm.hpp>
#include "common/Logger.hpp"

Scene::Scene() :
	m_Camera(Camera(CAMERA_RES_WIDTH, CAMERA_RES_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f)))
{
}

void Scene::Draw(Shader& shader)
{
	for (auto& model : m_Models)
	{
		model->Draw(shader, m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());
	}
}

void Scene::AddModel(std::unique_ptr<Model> model)
{
	m_Models.push_back(std::move(model));
}

void Scene::RemoveModel(size_t toDelete)
{
	if (toDelete < m_Models.size())
		m_Models.erase(m_Models.begin() + toDelete);
	else
		Logger::Get().Error("Invalid index. Index is out of range.");
}