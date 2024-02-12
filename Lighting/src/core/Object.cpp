#include "Object.h"

#include <GLFW/glfw3.h>
#include <iostream>

Object::Object(std::shared_ptr<Mesh> mesh, Shader* shader, Material* material) :
	  isUniformScaling(true), m_Name("Object"),
	  m_TranslationTransform(glm::mat4(1.0f)), m_RotationTransform(glm::mat4(1.0f)), m_ScaleTransform(glm::mat4(1.0f)),
	  m_ViewTransform(glm::mat4(1.0f)), m_ProjectionTransform(glm::mat4(1.0f)),
	  m_Mesh(mesh), m_Shader(shader), m_Material(material), 
	  m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)),
	  m_Texture(nullptr), m_SpecularTexture(nullptr), m_EmissionMap(nullptr)
{
}

void Object::SetName(const std::string& name)
{
	m_Name = name;
}

void Object::SetMesh(std::shared_ptr<Mesh> mesh)
{
	m_Mesh = mesh;
}

void Object::SetPosition(glm::vec3 position)
{
	m_TranslationTransform = glm::mat4(1.0f);
	m_TranslationTransform = glm::translate(m_TranslationTransform, position);
	m_Position = position;
}

void Object::SetRotation(glm::vec3 rotation)
{
	m_RotationTransform = glm::mat4(1.0f);

	m_RotationTransform =
		glm::rotate(m_RotationTransform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(m_RotationTransform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(m_RotationTransform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Rotation = rotation;
}

void Object::SetScale(glm::vec3 scale)
{
	m_ScaleTransform = glm::mat4(1.0f);
	m_ScaleTransform = glm::scale(m_ScaleTransform, scale);
	m_Scale = scale;
}

void Object::Draw() const
{
	m_Shader->Use();
	m_Mesh->Bind();

	if (m_Texture)
	{
		glActiveTexture(GL_TEXTURE0);
		m_Texture->Use();

		m_Shader->SetInt("u_material.diffuse", 0);

		if (m_SpecularTexture)
		{
			glActiveTexture(GL_TEXTURE1);
			m_SpecularTexture->Use();

			m_Shader->SetInt("u_material.specular", 1);
		}

		if (m_EmissionMap)
		{
			glActiveTexture(GL_TEXTURE2);
			m_EmissionMap->Use();

			m_Shader->SetInt("u_material.emission", 2);
		}
	}

	// Vertex shader uniforms
	m_Shader->SetMat4("u_model", m_TranslationTransform * m_RotationTransform  * m_ScaleTransform);
	m_Shader->SetMat4("u_view", m_ViewTransform);
	m_Shader->SetMat4("u_projection", m_ProjectionTransform);

	m_Shader->SetFloat("u_material.shininess", m_Material->shininess);

	m_Shader->SetFloat("u_time", 3 * glfwGetTime());

	// Rendering geometry
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

void Object::SetViewAndProjectionMatrix(const glm::mat4& view, const glm::mat4& proj)
{
	m_ViewTransform = view;
	m_ProjectionTransform = proj;
}

void Object::SetTexture(Texture* texture)
{
	m_Texture = texture;
}

void Object::SetSpecularTexture(Texture* texture)
{
	m_SpecularTexture = texture;
}

void Object::SetEmissionMap(Texture* texture)
{
	m_EmissionMap = texture;
}

void Object::SetDirectionalLight(glm::vec3&& direction, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular)
{
	m_Shader->Use();

	m_Shader->SetVec3("u_dirLight.direction", m_ViewTransform * glm::vec4(direction, 0.0f));
	m_Shader->SetVec3("u_dirLight.ambient", ambient);
	m_Shader->SetVec3("u_dirLight.diffuse", diffuse);
	m_Shader->SetVec3("u_dirLight.specular", specular);
}