#pragma once

#include "Object.h"

#include <vector>

class Light : public Object
{
public:
	Light(Mesh* mesh, Shader* shader, glm::vec3 color = glm::vec3(1.0f))
		: Object(mesh, shader, color)
	{};

	void AddAffected(Object* obj)
	{
		m_AffectedList.push_back(obj);
	}

	void RemoveAffected()
	{
		m_AffectedList.pop_back();
	}

	void Update()
	{
		for (auto object : m_AffectedList)
		{
			object->SetLightColor(this->m_Color);
			object->SetLightPosition(this->m_Position);
		}
	}

private:
	std::vector<Object*> m_AffectedList;
};