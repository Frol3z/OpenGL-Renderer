#include "Scene.h"

#include <glm/glm.hpp>

Scene::Scene() :
	m_Camera(Camera(CAMERA_RES_WIDTH, CAMERA_RES_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f)))
{
}

void Scene::Draw()
{
	/* 
		For every object I need to :
			- update the view and projection matrices;
			- tell it the properties of the directional light that is affecting it (may change);
			- also point and spot lights;
			- finally draw.
	*/

	for (auto& object : m_Objects)
	{
		object->SetViewAndProjectionMatrix(m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());
		object->SetDirectionalLight(m_DirLight.GetDirection(), m_DirLight.GetAmbient(), m_DirLight.GetDiffuse(), m_DirLight.GetSpecular());
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