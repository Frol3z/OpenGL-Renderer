#include "Scene.h"

#include <glm/glm.hpp>
#include <iostream>

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

void Scene::AddObject(std::unique_ptr<Object> obj)
{
	m_Objects.push_back(std::move(obj));
}

void Scene::RemoveObject(size_t toDelete)
{
	if (toDelete < m_Objects.size())
		m_Objects.erase(m_Objects.begin() + toDelete);
	else
		std::cerr << "Invalid index. Index is out of range." << std::endl;
}

void Scene::AddMesh(std::unique_ptr<Mesh> mesh)
{
	m_Meshes.push_back(std::move(mesh));
}

void Scene::RemoveMesh(size_t toDelete)
{
	if (toDelete < m_Meshes.size())
		m_Meshes.erase(m_Meshes.begin() + toDelete);
	else
		std::cerr << "Invalid index. Index is out of range." << std::endl;
}

void Scene::AddShader(std::unique_ptr<Shader> shader)
{
	m_Shaders.push_back(std::move(shader));
}

void Scene::RemoveShader(size_t toDelete)
{
	if (toDelete < m_Shaders.size())
		m_Shaders.erase(m_Shaders.begin() + toDelete);
	else
		std::cerr << "Invalid index. Index is out of range." << std::endl;
}

void Scene::AddMaterial(std::unique_ptr<Material> material)
{
	m_Materials.push_back(std::move(material));
}

void Scene::RemoveMaterial(size_t toDelete)
{
	if (toDelete < m_Materials.size())
		m_Materials.erase(m_Materials.begin() + toDelete);
	else
		std::cerr << "Invalid index. Index is out of range." << std::endl;
}

void Scene::AddTexture(std::unique_ptr<Texture> texture)
{
	m_Textures.push_back(std::move(texture));
}

void Scene::RemoveTexture(size_t toDelete)
{
	if (toDelete < m_Textures.size())
		m_Textures.erase(m_Textures.begin() + toDelete);
	else
		std::cerr << "Invalid index. Index is out of range." << std::endl;
}