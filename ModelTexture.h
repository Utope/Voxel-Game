#pragma once
#include "GL\glew.h"

class ModelTexture {
public:
	ModelTexture() {};
	ModelTexture(GLuint textureID) {
		this->m_textureID = textureID;
	}
	
	GLuint getTextureID();
	
	~ModelTexture() {};

private:
	GLuint m_textureID;
};

GLuint ModelTexture::getTextureID() {
	return this->m_textureID;
};