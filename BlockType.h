#pragma once
#include "GL\glew.h"
#include "glm/glm.hpp"
#include <string>

class BlockType {
public:
	BlockType();
	BlockType(std::string blockName, GLuint textureID, glm::vec3 blockColor, int blockID, bool isRegisterd);
	~BlockType();

	GLuint getTextureID() const;
	glm::vec3 getColor() const;
	std::string getBlockName() const;

	bool setTextureID(GLuint textureID);
	bool setColor(glm::vec3);
	bool setBlockName(std::string blockName);

	bool compareTypes(BlockType*) const;
	
protected:
private:

	GLuint m_textureID;
	std::string m_blockName;
	glm::vec3 m_color;
	bool m_registered;
	unsigned int m_blockID;
	
};

//Constructors and Deconstructer

BlockType::BlockType() {
	m_registered = false;
	m_blockID = 0;
	m_textureID = 0;
	m_color = glm::vec3(0.0f,0.0f,0.0f);
	m_blockName = "NOT SET";
}

BlockType::BlockType(std::string blockName, GLuint textureID, glm::vec3 blockColor, int blockID, bool isRegisterd) {
	m_registered = isRegisterd;
	m_blockID = blockID;
	m_textureID = textureID;
	m_color = blockColor;
	m_blockName = blockName;
};

BlockType::~BlockType() {

}

//Getters and Setters
GLuint BlockType::getTextureID() const { 
	return m_textureID; 
}; 

glm::vec3 BlockType::getColor() const{ 
	return m_color; 
}

std::string BlockType::getBlockName() const { 
	return m_blockName; 
}

bool BlockType::setTextureID(GLuint textureID) {
	if (!m_registered) {
		m_textureID = textureID;
		return true;
	}
	return false;
}

bool BlockType::setColor(glm::vec3 color) {
	if (!m_registered) {
		m_color = color;
		return true;
	}
}

bool BlockType::setBlockName(std::string blockName) {
	if (!m_registered) {
		m_blockName = blockName;
		return true;
	}
	return false;
}

//Public

bool BlockType::compareTypes(BlockType* other) const {
	if (this->m_blockID == other->m_blockID) {
		return true;
	}
	return false;
}

//Private