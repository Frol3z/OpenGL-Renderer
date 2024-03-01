#include "Model.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include "common/Logger.hpp"

#include "Shader.h"

Model::Model(const std::string& path) :
	isUniformScaling(true),
	m_TranslationTransform(1.0f), m_RotationTransform(1.0f), m_ScaleTransform(1.0f),
	m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f)
{
	LoadFromFile(path);
}

void Model::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection)
{
	shader.Use();

	shader.SetMat4("u_ModelMat", m_TranslationTransform * m_RotationTransform * m_ScaleTransform);
	shader.SetMat4("u_ViewMat", view);
	shader.SetMat4("u_ProjectionMat", projection);

	for (unsigned int i = 0; i < m_Meshes.size(); i++)
		m_Meshes[i].Draw(shader);
}

void Model::SetPosition(glm::vec3 position)
{
	glm::mat4 m_TranslationTransform = glm::mat4(1.0f);
	m_TranslationTransform = glm::translate(m_TranslationTransform, position);

	m_Position = position;
}

void Model::SetRotation(glm::vec3 rotation)
{
	m_RotationTransform = glm::mat4(1.0f);
	m_RotationTransform =
		glm::rotate(m_RotationTransform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(m_RotationTransform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(m_RotationTransform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Rotation = rotation;
}

void Model::SetScale(glm::vec3 scale)
{
	m_ScaleTransform = glm::mat4(1.0f);
	m_ScaleTransform = glm::scale(m_ScaleTransform, scale);

	m_Scale = scale;
}

void Model::LoadFromFile(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::Get().Error(importer.GetErrorString());
		return;
	}

	m_Directory = path.substr(0, path.find_last_of('\\'));
	Logger::Get().Info("m_Directory = " + m_Directory);

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	
	// Process all node's children recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertices setup
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// Normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		// Texture coordinates
		if (mesh->HasTextureCoords(0))
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Indices setup
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Material setup
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, TextureType::EMISSIVE);
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeEnum)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str());
			texture.type = typeEnum;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::TextureFromFile(const std::string& path)
{
	std::string filename = path;
	filename = m_Directory + '/' + filename;

	Logger::Get().Info("Loading texture from " + filename);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		Logger::Get().Error("Texture failed to load at path: " + path);
		stbi_image_free(data);
	}

	return textureID;
}