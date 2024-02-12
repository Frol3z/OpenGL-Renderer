#pragma once

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

	void AddObject(Object* obj);
	void RemoveObject(Object* obj);

	inline Camera& GetCamera() { return m_Camera; }
	inline DirectionalLight& GetDirectionalLight() { return m_DirLight; }
	inline std::vector<Object*>& GetObjects() { return m_Objects; }

private:
	Camera m_Camera;
	DirectionalLight m_DirLight;
	std::vector<Object*> m_Objects;
};