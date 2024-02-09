#include "Texture.h"

#include <stb_image/stb_image.h>

Texture::Texture(std::string path)
{
	// create texture
	CreateTexture(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	
	// get the image format
	SetFileFormat(path);

	// based on the image format, convert to pixel data format
	GLenum glFormat = GL_NONE;
	switch (format)
	{
		case Format::JPG: glFormat = GL_RGB; break;
		case Format::PNG: glFormat = GL_RGBA; break;
		default: break; 
	}

	// loading from file
	LoadFromFile(path, 0, glFormat);
}

Texture::Texture(std::string path, int levelOfDetail, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	// create texture
	CreateTexture(wrapS, wrapT, minFilter, magFilter);

	// get the image format
	SetFileFormat(path);

	// based on the image format, convert to pixel data format
	GLenum glFormat = GL_NONE;
	switch (format)
	{
	case Format::JPG: glFormat = GL_RGB; break;
	case Format::PNG: glFormat = GL_RGBA; break;
	default: break;
	}

	// loading from file
	LoadFromFile(path, levelOfDetail, glFormat);
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int Texture::GetTexture() const
{
	return texture;
}

void Texture::LoadFromFile(std::string path, int levelOfDetail, GLenum format)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, levelOfDetail, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error: Failed to load texture!" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::CreateTexture(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// configuration
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetFileFormat(std::string path)
{
	size_t dotPosition = path.find_last_of('.');

	if (dotPosition != std::string::npos && dotPosition < path.length() - 1)
	{
		std::string extension = path.substr(dotPosition + 1);

		if (extension == "jpg")
			format = Format::JPG;
		else if (extension == "png")
			format = Format::PNG;
		else
			format = Format::UNKNOWN;
	}
	else {
		std::cout << "ERROR: Invalid texture path provided!" << std::endl;
	}
}