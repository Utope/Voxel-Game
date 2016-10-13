#pragma once
#include "BlockType.h"

class Block{
public:
	Block();
	Block(bool active);
	~Block();

	bool isActive();
	void setActive(bool active);

	BlockType* getBlockType();
	void setBlockType(BlockType*);

	bool compare(Block* other);

private:
	//BlockType* blockType;
	bool m_active;
	BlockType* m_type;
};
//Constructor and Deconstructor
Block::Block() {
	m_active = false;
	//Need to set as default type
	m_type = nullptr;
}

Block::Block(bool isActive) {
	m_active = isActive;
}

Block::~Block() {

}

//Getters and setters

bool Block::isActive() {
	return m_active;
};

void Block::setActive(bool active) {
	m_active = active;
}

BlockType* Block::getBlockType() {
	return m_type;
}

void Block::setBlockType(BlockType* type) {
	m_type = type;
}

//Public

bool compare(Block* other) {
	return true;
}

//Private