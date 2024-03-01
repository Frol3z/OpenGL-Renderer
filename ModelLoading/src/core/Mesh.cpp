#include "Mesh.h"

#include "glad/glad.h"
#include "Shader.h"

#include <glm/glm.hpp>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned short diffuseCount = 1;
	unsigned short specularCount = 1;
	unsigned short emissiveCount = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		switch (textures[i].type)
		{
			case DIFFUSE:
			{
				shader.SetInt("material.texture_diffuse" + std::to_string(diffuseCount++), i);
				break;
			}
			case SPECULAR:
			{
				shader.SetInt("material.texture_specular" + std::to_string(specularCount++), i);
				break;
			}
			case EMISSIVE:
			{
				shader.SetInt("material.texture_emissive" + std::to_string(emissiveCount++), i);
				break;
			}
		}
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}