#pragma once
#include "VoxelType.h"

class Voxel{
public:
	Voxel();
	Voxel(bool isActive, VoxelType* voxelType);
	~Voxel();

	void setVoxelType(VoxelType* voxelType);
	void setActive(bool isActive);
	
	bool isActive() const;
	VoxelType* getVoxelType() const;

	bool compareVoxels(Voxel* other) const;
private:
	VoxelType* m_voxelType;
	bool m_isActive;

};
//Constructor and Deconstructor
Voxel::Voxel() {
	m_isActive = false;
	m_voxelType = nullptr;
}
Voxel::Voxel(bool isActive, VoxelType* voxelType) {
	m_isActive = isActive;
	m_voxelType = voxelType;
}

Voxel::~Voxel() {

}
//Public Functions
bool Voxel::compareVoxels(Voxel* other) const {
	return m_voxelType->compareVoxelTypes(other->getVoxelType());
}

void Voxel::setVoxelType(VoxelType* voxelType) {
	m_voxelType = voxelType;
}
void Voxel::setActive(bool isActive) {
	m_isActive = isActive;
}

bool Voxel::isActive() const {
	return m_isActive;
}
VoxelType* Voxel::getVoxelType() const {
	return m_voxelType;
}

