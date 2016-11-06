#pragma once
#include <map>
#include <string>
#include "VoxelType.h"
/*
want to add functions to query multiple different attributes of voxels
- get voxels with hardness X
- get voxels that spawn below X
 etc

 very databasyy calls
*/
class VoxelTypeManager {
public:
	VoxelTypeManager();
	~VoxelTypeManager();

	bool registerVoxel(VoxelType* voxelType);
	bool loadVoxelsFromXMLFile(std::string xmlFileName);

	bool doesVoxelExist(std::string voxelTypeName) const;
	bool doesVoxelExist(unsigned int voxelID) const;

	//Ton of these functions quearying differnet voxels
	VoxelType* getVoxelType() const;
private:
	std::map<unsigned int, VoxelType*> m_blockTypes;
};

// Constructor and Deconstructor
VoxelTypeManager::VoxelTypeManager() {

}
VoxelTypeManager::~VoxelTypeManager() {

}

//Public

bool VoxelTypeManager::registerVoxel(VoxelType* voxelType) {

}

VoxelType* VoxelTypeManager::getVoxelType() const {

}

bool VoxelTypeManager::loadVoxelsFromXMLFile(std::string xmlFileName) {

}

bool VoxelTypeManager::doesVoxelExist(std::string voxelName) const {

}
bool VoxelTypeManager::doesVoxelExist(unsigned int voxelId) const {

}





