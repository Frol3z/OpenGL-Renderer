#pragma once

#include <vector>

#define WIDTH 1280
#define HEIGHT 720

class Camera;
class Object;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	void Draw() const;
	void AddObject(Object* obj);
	void RemoveObject();

	inline Camera* GetCamera() const { return m_Camera; };

private:
	Camera* m_Camera;
	std::vector<Object*> m_Objects;
};