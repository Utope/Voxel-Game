#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
class SpriteBillboard{
public:
	SpriteBillboard();
	~SpriteBillboard();

	void getTextureCoords();
	void getTextureID();
	void getPosition();
	void getColor();

	void renderSpriteBillboard();
private:
	GLuint textureID;

};