#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "light/DirectionalLight.hpp"
#include "Object.h"

#define CAMERA_RES_WIDTH 1920	
#define CAMERA_RES_HEIGHT 1080

class Scene
{
public:
	// Constructor
	Scene();

	void Draw();

	void AddObject(std::unique_ptr<Object> obj);
	void RemoveObject(size_t toDelete);
	
	void AddMesh(std::unique_ptr<Mesh> mesh);
	void RemoveMesh(size_t toDelete);
	
	void AddShader(std::unique_ptr<Shader> shader);
	void RemoveShader(size_t toDelete);
	
	void AddMaterial(std::unique_ptr<Material> material);
	void RemoveMaterial(size_t toDelete);

	void AddTexture(std::unique_ptr<Texture> texture);
	void RemoveTexture(size_t toDelete);

	inline Camera& GetCamera() { return m_Camera; }
	inline DirectionalLight& GetDirectionalLight() { return m_DirLight; }
	inline std::vector<std::unique_ptr<Object>>& GetObjects() { return m_Objects; }
	inline std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_Meshes; }
	inline std::vector<std::unique_ptr<Shader>>& GetShaders() { return m_Shaders; }
	inline std::vector<std::unique_ptr<Material>>& GetMaterials() { return m_Materials; }
	inline std::vector<std::unique_ptr<Texture>>& GetTextures() { return m_Textures; }

private:
	Camera m_Camera;
	DirectionalLight m_DirLight;
	std::vector<std::unique_ptr<Object>> m_Objects;
	std::vector<std::unique_ptr<Mesh>> m_Meshes;
	std::vector<std::unique_ptr<Shader>> m_Shaders;
	std::vector<std::unique_ptr<Material>> m_Materials;
	std::vector<std::unique_ptr<Texture>> m_Textures;
};