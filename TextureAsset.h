#pragma once
#include "glm\glm.hpp"
#include "GL\glew.h"

class TextureAsset{
public:
	TextureAsset(GLuint) {};
	~TextureAsset() {};
	GLuint getTextureID();
private:
	GLuint m_textureID;
};

GLuint TextureAsset::getTextureID() {
	return m_textureID;
}