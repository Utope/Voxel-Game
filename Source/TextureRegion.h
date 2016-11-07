#pragma once
#include <string>
#include "Texture.h"
#include "glm\common.hpp"

//Foreward decleration
class Texture;

class TextureRegion {
public:
	TextureRegion();
	TextureRegion(std::string regionName, Texture* texture, glm::vec4 regionCoords);
	~TextureRegion();

	Texture* getTexture() const;
	glm::vec4 getTextureRegion() const;
	std::string getTextureRegionName() const;

	void setTexture(Texture* const texture);
	void setTextureRegionCoords(glm::vec4);
	void setTextureRegionName(std::string name);
private:
	glm::vec4 m_textureRegionCoords;
	std::string m_textureRegionName;
	Texture* m_texture;
};

//Constructor and Deconstructor
TextureRegion::TextureRegion() {
	m_texture = nullptr;
}

TextureRegion::TextureRegion(std::string regionName, Texture* texture,glm::vec4 regionCoords) {
	m_textureRegionName = regionName;
	m_texture = texture;
	m_textureRegionCoords = regionCoords;
}

TextureRegion::~TextureRegion() {

}
//Getters and setters

Texture* TextureRegion::getTexture() const  {
	return m_texture;
}

glm::vec4 TextureRegion::getTextureRegion() const  {
	return m_textureRegionCoords;
}

std::string TextureRegion::getTextureRegionName() const {
	return m_textureRegionName;
}

void TextureRegion::setTexture(Texture* const texture) {
	m_texture = texture;
}

void TextureRegion::setTextureRegionName(std::string name) {
	m_textureRegionName = name;
}

void TextureRegion::setTextureRegionCoords(glm::vec4 coords) {
	m_textureRegionCoords = coords;
}
