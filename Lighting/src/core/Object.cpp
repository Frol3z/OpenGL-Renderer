#include "Object.h"

#include <GLFW/glfw3.h>

Object::Object(std::string&& name, Mesh* mesh, Material* material, Shader* shader) :
	m_Name(name), m_Mesh(mesh),
	m_Material(material), m_Shader(shader),
	m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)),
	m_TranslationTransform(glm::mat4(1.0f)), m_RotationTransform(glm::mat4(1.0f)), m_ScaleTransform(glm::mat4(1.0f)),
	m_ViewTransform(glm::mat4(1.0f)), m_ProjectionTransform(glm::mat4(1.0f)),
	isUniformScaling(true), selectedMesh(0), selectedMaterial(0), selectedShader(0)
{
}

void Object::Draw() const
{
	m_Shader->Use();
	m_Mesh->Bind();

	// Binding texture
	Texture* tex = m_Material->GetDiffuseMap();
	Texture* specTex = m_Material->GetSpecularMap();
	Texture* emisTex = m_Material->GetEmissionMap();

	m_Shader->SetBool("u_isTextured", false);

	if (tex)
	{
		glActiveTexture(GL_TEXTURE0);
		tex->Bind();
		m_Shader->SetBool("u_isTextured", true);
		m_Shader->SetInt("u_material.diffuseMap", 0);

		if (specTex)
		{
			glActiveTexture(GL_TEXTURE1);
			specTex->Bind();
			m_Shader->SetInt("u_material.specularMap", 1);
		}

		if (emisTex)
		{
			glActiveTexture(GL_TEXTURE2);
			emisTex->Bind();
			m_Shader->SetInt("u_material.emissionMap", 2);
		}
	}

	// Vertex shader uniforms
	m_Shader->SetMat4("u_model", m_TranslationTransform * m_RotationTransform * m_ScaleTransform);
	m_Shader->SetMat4("u_view", m_ViewTransform);
	m_Shader->SetMat4("u_projection", m_ProjectionTransform);

	m_Shader->SetVec3("u_material.ambient", m_Material->GetAmbient());
	m_Shader->SetVec3("u_material.diffuse", m_Material->GetDiffuse());
	m_Shader->SetVec3("u_material.specular", m_Material->GetSpecular());
	m_Shader->SetFloat("u_material.shininess", m_Material->GetShininess());

	// m_Shader->SetFloat("u_time", 3 * glfwGetTime());

	// Rendering geometry
	size_t indicesCount = m_Mesh->GetIndicesCount();
	if (indicesCount > 0)
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertsCount());

	if (tex)
		tex->Unbind();
	if (specTex)
		specTex->Unbind();
	if (emisTex)
		emisTex->Unbind();

	m_Mesh->Unbind();
}

void Object::SetName(const std::string& name)
{
	m_Name = name;
}

void Object::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
}

void Object::SetMaterial(Material* material)
{
	m_Material = material;
}

void Object::SetShader(Shader* shader)
{
	m_Shader = shader;
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

void Object::SetViewAndProjectionMatrix(const glm::mat4& view, const glm::mat4& proj)
{
	m_ViewTransform = view;
	m_ProjectionTransform = proj;
}

void Object::SetDirectionalLight(glm::vec3&& direction, glm::vec3&& ambient, glm::vec3&& diffuse, glm::vec3&& specular)
{
	m_Shader->Use();

	m_Shader->SetVec3("u_dirLight.direction", m_ViewTransform * glm::vec4(direction, 0.0f));
	m_Shader->SetVec3("u_dirLight.ambient", ambient);
	m_Shader->SetVec3("u_dirLight.diffuse", diffuse);
	m_Shader->SetVec3("u_dirLight.specular", specular);
}