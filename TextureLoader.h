#pragma once
#include <map>
#include "TextureAsset.h"
#include "stb_image.h"

class TextureManager {
public:
	TextureManager() {};
	~TextureManager() {};
	TextureAsset& loadTexture(std::string textureName, std::string fileName);
	TextureAsset& getTexture(std::string textureName);
private:
	std::map<std::string, TextureAsset> m_textures;
};

TextureAsset& TextureManager::loadTexture(std::string textureName, std::string fileName) {
	int w;
	int h;
	int comp;
	stbi_uc* image = stbi_load(fileName.c_str(), &w, &h, &comp, 4);

	if (image == nullptr)
		throw(std::string("Failed to load texture"));

	GLuint texture;
	glGenTextures(1, &texture);
		
	m_textures.insert(std::pair<std::string, TextureAsset>(textureName, TextureAsset(texture)));
		
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	return m_textures.at(textureName);
}

TextureAsset& TextureManager::getTexture(std::string textureName) {
	return m_textures.at(textureName);
}