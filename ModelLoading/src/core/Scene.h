#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Model.h"

#define CAMERA_RES_WIDTH 1920	
#define CAMERA_RES_HEIGHT 1080

class Scene
{
public:
	Scene();

public:
	void Draw(Shader& shader);
	void AddModel(std::unique_ptr<Model> model);
	void RemoveModel(size_t toDelete);
	inline Camera& GetCamera() { return m_Camera; }
	inline std::vector<std::unique_ptr<Model>>& GetModels() { return m_Models; }

private:
	Camera m_Camera;
	std::vector<std::unique_ptr<Model>> m_Models;
};