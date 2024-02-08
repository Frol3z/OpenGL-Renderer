#pragma once

#include "Object.h"

#include <vector>

class Light : public Object
{
public:
	Light(Mesh* mesh, Shader* shader, Material* material)
		: Object(mesh, shader, material)
	{};

	void Draw() const override
	{
		m_Shader->Use();
		m_Mesh->Bind();

		m_Shader->SetMat4("model", m_TranslationTransform * m_RotationTransform * m_ScaleTransform);
		m_Shader->SetMat4("view", m_ViewTransform);
		m_Shader->SetMat4("projection", m_ProjectionTransform);

		m_Shader->SetVec3("color", m_Material->ambient);

		size_t indicesCount = m_Mesh->GetIndicesCount();
		if (indicesCount > 0)
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertsCount());

		m_Mesh->Unbind();
	}

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
			object->SetLightColor(this->m_Material->ambient);
			object->SetLightPosition(this->m_Position);
		}
	}

private:
	std::vector<Object*> m_AffectedList;
};