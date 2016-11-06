#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include<vector>
#include <map>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "TextureRegion.h"

// include mipmapping some how

//Forward decleration
class TextureRegion;

enum GL_Wrapping {
	REPEAT = GL_REPEAT,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum GL_Filtering {
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum GL_FilterScale {
	TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
	TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER
};

class Texture {
public:
	Texture();
	Texture(std::string fileName);
	Texture(std::string fileName,GL_Wrapping wrapping, GL_Filtering filter);
	~Texture();

	//Set wrapping/filtering and mip mapping??

	bool load();
	void loadTextureRegions();

	void setTextureName(std::string textureName);

	bool addTextureRegion(std::string textureRegionName, glm::vec4 coords);
	void removeTextureRegion();

	std::string getTextureName();
	GLuint getTextureID();
	TextureRegion* getTextureRegion(std::string textureRegionName);

private:
	std::map<std::string, TextureRegion*> m_textureRegions;
	std::string m_fileName;
	std::string m_textureName;
	bool m_isLoaded;

	TextureRegion m_fullTexture;

	GLuint m_textureID;
	GL_Wrapping m_wrapping;
	GL_Filtering m_filtering;
	bool m_hasMipMap;
	
};
//Constructor and deconstructor

Texture::Texture() {
	m_wrapping = GL_Wrapping::CLAMP_TO_BORDER;
	m_filtering = GL_Filtering::NEAREST;
}

Texture::Texture(std::string fileName) {
	m_fileName = fileName;
	m_textureName = "NOT SET";
}

Texture::Texture(std::string fileName, GL_Wrapping wrapping, GL_Filtering filter) {
	m_fileName = fileName;
	m_wrapping = wrapping;
	m_filtering = filter;
}

Texture::~Texture() {
	std::map<std::string, TextureRegion*>::iterator it;
	for (it = m_textureRegions.begin(); it != m_textureRegions.end(); ++it) {
		delete it->second;
	}
	glDeleteTextures(1, &m_textureID);
}

//Getters and Setters
GLuint Texture::getTextureID() {
	return m_textureID;
}

std::string Texture::getTextureName() {
	return m_textureName;
}

TextureRegion* Texture::getTextureRegion(std::string textureRegionName) {
	std::map<std::string, TextureRegion*>::iterator it = m_textureRegions.find(textureRegionName);
	if (it != m_textureRegions.end()) {
		return m_textureRegions.at(textureRegionName);
	}
	else {
		return &m_fullTexture;
	}
}

void Texture::setTextureName(std::string textureName){
	m_textureName = textureName;
}

//Public

bool Texture::load() {
	int w;
	int h;
	int comp;
	stbi_uc* image = stbi_load(m_fileName.c_str(), &w, &h, &comp, 4);

	if (image == nullptr) {
		throw(std::string("Failed to load texture::  " + m_fileName));
		return false;
	}
	
	m_isLoaded = true;

	glGenTextures(1, &m_textureID);

	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapping);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filtering);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filtering);

	//Need to research this line
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	m_fullTexture.setTexture(this);
	m_fullTexture.setTextureRegionCoords(glm::vec4(0,0,w,h));
	m_fullTexture.setTextureRegionName("Texture");

	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D,0);
	return true;
}

bool Texture::addTextureRegion(std::string textureRegionName, glm::vec4 regionCoords) {
	if (m_textureRegions.count(textureRegionName) == 0) {
		TextureRegion* textureRegion = new TextureRegion;
		textureRegion->setTexture(this);
		textureRegion->setTextureRegionCoords(regionCoords);
		textureRegion->setTextureRegionName(textureRegionName);
		m_textureRegions.insert(std::pair<std::string, TextureRegion*>(textureRegionName, textureRegion));
		return true;
	}
	return false;
}
//Private