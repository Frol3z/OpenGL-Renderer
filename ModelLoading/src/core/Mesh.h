#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader;

enum TextureType
{
	DIFFUSE = 0,
	SPECULAR,
	EMISSIVE
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	TextureType type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	
	void Draw(Shader& shader);

private:
	unsigned int m_VBO, m_EBO, m_VAO;

private:
	void SetupMesh();
};
