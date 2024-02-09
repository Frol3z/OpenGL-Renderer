#include "Object.h"

#include <iostream>

Object::Object(Mesh* mesh, Shader* shader, Material* material)
	: m_TranslationTransform(glm::mat4(1.0f)), m_RotationTransform(glm::mat4(1.0f)), m_ScaleTransform(glm::mat4(1.0f)),
	  m_ViewTransform(glm::mat4(1.0f)), m_ProjectionTransform(glm::mat4(1.0f)),
	  m_Mesh(mesh), m_Shader(shader), m_Material(material), m_Position(glm::vec3(0.0f)), m_Texture(nullptr),
	  m_LightPosition(glm::vec3(0.0f)), m_LightColor(glm::vec3(1.0f))
{
}

Object::Object(const Object& other)
	: m_TranslationTransform(other.m_TranslationTransform),
	m_RotationTransform(other.m_RotationTransform),
	m_ScaleTransform(other.m_ScaleTransform),
	m_ViewTransform(other.m_ViewTransform),
	m_ProjectionTransform(other.m_ProjectionTransform),
	m_Mesh(other.m_Mesh),
	m_Shader(other.m_Shader),
	m_Material(other.m_Material),
	m_Position(other.m_Position),
	m_Texture(other.m_Texture),
	m_LightPosition(other.m_LightPosition),
	m_LightColor(other.m_LightColor)
{}

void Object::Draw() const
{
	m_Shader->Use();
	m_Mesh->Bind();

	if (m_Texture)
		m_Texture->Use();

	m_Shader->SetMat4("model", m_TranslationTransform * m_RotationTransform  * m_ScaleTransform);
	m_Shader->SetMat4("view", m_ViewTransform);
	m_Shader->SetMat4("projection", m_ProjectionTransform);

	m_Shader->SetVec3("material.ambient", m_Material->ambient);
	m_Shader->SetVec3("material.diffuse", m_Material->diffuse);
	m_Shader->SetVec3("material.specular", m_Material->specular);
	m_Shader->SetFloat("material.shininess", m_Material->shininess);

	m_Shader->SetVec3("light.position", m_ViewTransform * glm::vec4(m_LightPosition, 1.0f));
	m_Shader->SetVec3("light.ambient", 0.2f * 0.5f * m_LightColor);
	m_Shader->SetVec3("light.diffuse", 0.5f * m_LightColor);
	m_Shader->SetVec3("light.specular", m_LightColor);

	size_t indicesCount = m_Mesh->GetIndicesCount();
	if (indicesCount > 0)
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertsCount());

	m_Mesh->Unbind();
} 

void Object::SetMaterial(Material* material)
{
	m_Material = material;
}

void Object::SetAmbient(glm::vec3 ambient)
{
	m_Material->ambient = ambient;
}

void Object::SetDiffuse(glm::vec3 diffuse)
{
	m_Material->diffuse = diffuse;
}

void Object::SetSpecular(glm::vec3 specular)
{
	m_Material->specular = specular;
}

void Object::SetShininess(float shininess)
{
	m_Material->shininess = shininess;
}

void Object::SetViewAndProjectionMatrix(glm::mat4 view, glm::mat4 proj)
{
	m_ViewTransform = view;
	m_ProjectionTransform = proj;
}

void Object::SetPosition(glm::vec3 position)
{
	m_TranslationTransform = glm::mat4(1.0f);
	m_TranslationTransform = glm::translate(m_TranslationTransform, position);
	m_Position = position;
}

void Object::SetRotation(float degree, glm::vec3 rotation)
{
	m_RotationTransform = glm::mat4(1.0f);
	m_RotationTransform = glm::rotate(m_RotationTransform, glm::radians(degree), rotation);
}

void Object::SetScale(glm::vec3 scale)
{
	m_ScaleTransform = glm::mat4(1.0f);
	m_ScaleTransform = glm::scale(m_ScaleTransform, scale);
}

void Object::SetTexture(Texture* texture)
{
	m_Texture = texture;
}

void Object::SetLightPosition(glm::vec3 position)
{
	m_LightPosition = position;
}

void Object::SetLightColor(glm::vec3 color)
{
	m_LightColor = color;
	m_Material->specular = 0.5f * m_LightColor;
}