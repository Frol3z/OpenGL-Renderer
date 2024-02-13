#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
	Texture(std::string path);
	Texture(
		std::string path, 
		int levelOfDetail,
		GLint wrapS,
		GLint wrapT,
		GLint minFilter,
		GLint magFilter
	);

	void Bind() const;
	void Unbind() const;

	unsigned int GetTexture() const;

private:

	enum class Format
	{
		UNKNOWN = 0,
		JPG,
		PNG
	};

	unsigned int texture;
	Format format;

private:
	void LoadFromFile(std::string path, int levelOfDetail, GLenum format);
	void CreateTexture(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	void SetFileFormat(std::string path);
};