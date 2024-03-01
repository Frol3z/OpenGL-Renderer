#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "Mesh.h"

class Shader;

class Model
{
public:
	bool isUniformScaling;

public:
	Model(const std::string& path);

public:
	void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection);

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetRotation() const { return m_Rotation; }
	inline glm::vec3 GetScale() const { return m_Scale; }

private:
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_TranslationTransform;
	glm::mat4 m_RotationTransform;
	glm::mat4 m_ScaleTransform;

private:
	void LoadFromFile(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeEnum);
	unsigned int TextureFromFile(const std::string& path);
};