#pragma once
#include <string>
#include "GL\glew.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include "TextureRegion.h"

enum VoxelFaces {
	UP_Y, DOWN_Y,
	LEFT_X, RIGHT_X,
	BACK_Z, FRONT_Z,
	NUM_FACES
};

class VoxelType {
public:
	VoxelType();
	~VoxelType();

	void setTexture(Texture* texture);
	void setColor(glm::vec3 color);
	void setVoxelTypeName(std::string blockName);
	void setFrontTextureRegion(TextureRegion* region);
	void setBackTextureRegion(TextureRegion* region);
	void setLeftTextureRegion(TextureRegion* region);
	void setRightTextureRegion(TextureRegion* region);
	void setUpTextureRegion(TextureRegion* region);
	void setDownTextureRegion(TextureRegion* region);

	void setVoxelID(unsigned int id);

	Texture* getTexture() const;
	glm::vec3 getColor() const;
	std::string getVoxelTypeName() const;
	unsigned short getVoxelTypeID() const;
	TextureRegion* getFrontTextureRegion() const;
	TextureRegion* getBackTextureRegion() const;
	TextureRegion* getLeftTextureRegion() const;
	TextureRegion* getRightTextureRegion() const;
	TextureRegion* getUpTextureRegion() const;
	TextureRegion* getDownTextureRegion() const;
	
	bool compareVoxelTypes(VoxelType*) const;
	bool isCompleteVoxelType() const;
protected:
private:
	std::string m_voxelTypeName;

	Texture* m_texture;
	std::vector<TextureRegion*> m_faceTextures;
	
	glm::vec3 m_color;
	unsigned int m_VoxelTypeID;
	
};

//Constructors and Deconstructer
VoxelType::VoxelType() {
	m_VoxelTypeID = 0;
	m_color = glm::vec3(-1.0, -1.0, -1.0);
	m_voxelTypeName = "";
	m_texture = nullptr;

	for (int i = 0; i < NUM_FACES; i++) {
		m_faceTextures.push_back(nullptr);
	}
}

VoxelType::~VoxelType() {

}

//Public functions
bool VoxelType::compareVoxelTypes(VoxelType* other) const {
	if (other->getVoxelTypeID() == this->getVoxelTypeID()) {
		return true;
	}
	else {
		return false;
	}
}
bool VoxelType::isCompleteVoxelType() const {
	//Are all face TextureRegions set?
	for (int i = 0; i < m_faceTextures.size(); i++) {
		if (m_faceTextures[i] == nullptr) {
			return false;
		}
	}

	//is texture set?
	if (m_texture == nullptr) {
		return false;
	}

	//is name set?
	if (strcmp(m_voxelTypeName.c_str(),"") == 0) {
		return false;
	}

	//is color set?
	if (m_color.x < 0.0 || m_color.y < 0.0 || m_color.z < 0.0) {
		return false;
	}

	return true;
}

void VoxelType::setTexture(Texture* texture) {
	m_texture = texture;
}
void VoxelType::setColor(glm::vec3 color) {
	m_color = color;
}
void VoxelType::setVoxelTypeName(std::string voxelTypeName) {
	m_voxelTypeName = voxelTypeName;
}
void VoxelType::setFrontTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::FRONT_Z] = region;
}
void VoxelType::setBackTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::BACK_Z] = region;
}
void VoxelType::setLeftTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::LEFT_X] = region;
}
void VoxelType::setRightTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::RIGHT_X] = region;
}
void VoxelType::setUpTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::UP_Y] = region;
}
void VoxelType::setDownTextureRegion(TextureRegion* region) {
	m_faceTextures[VoxelFaces::DOWN_Y] = region;
}
void VoxelType::setVoxelID(unsigned int id) {
	m_VoxelTypeID = id;
}

Texture* VoxelType::getTexture() const {
	return m_texture;
}
glm::vec3 VoxelType::getColor() const {
	return m_color;
}
std::string VoxelType::getVoxelTypeName() const {
	return m_voxelTypeName;
}
unsigned short VoxelType::getVoxelTypeID() const {
	return m_VoxelTypeID;
}
TextureRegion* VoxelType::getFrontTextureRegion() const {
	return m_faceTextures[VoxelFaces::FRONT_Z];
}
TextureRegion* VoxelType::getBackTextureRegion() const {
	return m_faceTextures[VoxelFaces::BACK_Z];
}
TextureRegion* VoxelType::getLeftTextureRegion() const {
	return m_faceTextures[VoxelFaces::LEFT_X];
}
TextureRegion* VoxelType::getRightTextureRegion() const {
	return m_faceTextures[VoxelFaces::RIGHT_X];
}
TextureRegion* VoxelType::getUpTextureRegion() const {
	return m_faceTextures[VoxelFaces::UP_Y];
}
TextureRegion* VoxelType::getDownTextureRegion() const {
	return m_faceTextures[VoxelFaces::DOWN_Y];
}
