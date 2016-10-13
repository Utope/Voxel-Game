#pragma once
#include <map>
#include "BlockType.h"

class BlockTypeManager {
public:
	BlockTypeManager();
	~BlockTypeManager();

	void registerBlock(BlockType* blockType);
	void removeBlock();
	BlockType* getBlockType();
private:
	std::map<unsigned int, BlockType*> m_blockTypes;
};

// Constructor and Deconstructor
BlockTypeManager::BlockTypeManager(){}
BlockTypeManager::BlockTypeManager(){}

//Public


