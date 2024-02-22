#include "../Object.h"

class PointLight : public Object
{
public:
	PointLight(std::string&& name, Mesh* mesh, Material* material, Shader* shader) :
		Object(std::move(name), mesh, material, shader),
		m_Color(glm::vec3(1.0f)), m_Radius(50.0f),
		m_Ambient(glm::vec3(0.2f)), m_Diffuse(glm::vec3(0.5f)), m_Specular(glm::vec3(1.0f)),
		m_Intensity(1.0f)
	{
	}

	void Draw() override
	{
		m_Shader->Use();
		m_Mesh->Bind();

		// Shader uniforms
		m_Shader->SetMat4("u_model", m_TranslationTransform * m_RotationTransform * m_ScaleTransform);
		m_Shader->SetMat4("u_view", m_ViewTransform);
		m_Shader->SetMat4("u_projection", m_ProjectionTransform);
		m_Shader->SetVec3("u_Color", m_Intensity * m_Color);

		// Rendering geometry
		size_t indicesCount = m_Mesh->GetIndicesCount();
		if (indicesCount > 0)
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertsCount());

		m_Mesh->Unbind();
	}

	void AffectObject(Object* obj)
	{
		auto& pointLights = obj->GetPointLights();
		pointLights.push_back({m_Position, m_Radius, m_Ambient, m_Diffuse, m_Specular});
	}

	// Setters
	void SetColor(glm::vec3 color) { m_Color = color; }
	void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
	void SetIntensity(float intensity) { m_Intensity = intensity; }

	// Getters
	inline glm::vec3 GetColor() const { return m_Color; }
	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }
	inline float GetIntensity() const { return m_Intensity; }

private:
	glm::vec3 m_Color;
	float m_Radius;
	float m_Intensity;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};