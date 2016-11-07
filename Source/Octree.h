#pragma once
#include "glm\glm.hpp"
#include <math.h>
#include "Block.h"
#include <vector>

enum NodeType {
	LEAF,INTERNAL,ROOT
};

//FRONT:: LEFT_UPPER, RIGHT_UPPER, LEFT_LOWER, RIGHT_LOWER
static glm::uvec3 FRONT_QUADRENT_1 = glm::uvec3(0, 0, 0);
static glm::uvec3 FRONT_QUADRENT_2 = glm::uvec3(2, 0, 0);
static glm::uvec3 FRONT_QUADRENT_3 = glm::uvec3(0, 2, 0);
static glm::uvec3 FRONT_QUADRENT_4 = glm::uvec3(2, 2, 0);
//BACK:: LEFT_UPPER, RIGHT_UPPER, LEFT_LOWER, RIGHT_LOWER
static glm::uvec3 BACK_QUADRENT_1 = glm::uvec3(0, 0, 2);
static glm::uvec3 BACK_QUADRENT_2 = glm::uvec3(2, 0, 2);
static glm::uvec3 BACK_QUADRENT_3 = glm::uvec3(0, 2, 2);
static glm::uvec3 BACK_QUADRENT_4 = glm::uvec3(2, 2, 2);

//DENOTES ROOT
static glm::uvec3 ROOT_POSITION = glm::uvec3(0, 0, 0);

int _pow(int base, int exp) {
	if (exp > 0) {
		return base*_pow(base,exp-1);
	}
	else {
		return 1;
	}
}

class OctreeNode {
public:
	OctreeNode(NodeType type) {
		m_children = new OctreeNode*[8];
		if (type == NodeType::LEAF) {
			m_children == nullptr;
		}
		else {
			for (int i = 0; i < 8; i++) {
				m_children[i] = nullptr;
			}
		}
	};
	~OctreeNode() {};
	Block getValue() { return m_value; }
	void setValue(Block value) { m_value = value; };
	OctreeNode** getChildrenNodes() { return m_children; };

	Block* getAverageOfChildren() {

	};
	
	void pruneTree() {
		bool sameNodes = true;
		UniqueBlock_Type* pblock = m_children[0]->getValue().getType();
		for (int i = 1; i < 8; i++) {
			if (pblock != m_children[i]->getValue().getType()) {
				sameNodes = false;
			}
		}

		if (sameNodes) {
			setValue(getAverageOfChildren());
			for (int i = 0; i < 8; i++) {
				delete m_children[i];
			}
			delete m_children;
		}

	}

protected:
private:
	Block m_value;
	OctreeNode** m_children;
};

class Octree {
public:

	Octree(unsigned int treeDimensions) {
		Block*** m_pBlocks;

		m_pBlocks = new Block**[treeDimensions];
		for (int i = 0; i < treeDimensions; i++) {

			m_pBlocks[i] = new Block*[treeDimensions];

			for (int j = 0; j < treeDimensions; j++) {
				m_pBlocks[i][j] = new Block[treeDimensions];
			}
		}
		
		
		buildTree(m_pBlocks, treeDimensions);
	};
	~Octree() {};
	
	void add(glm::uvec3 position);
	bool remove(glm::uvec3 position);
	Block* getValue(glm::uvec3 position);
	OctreeNode* getNode(glm::uvec3 position);
	void pruneTree();

	void buildTree(Block*** chunkBlocks, int arrayDimensions) {
		
		int arrayTreeHeight = std::log2(arrayDimensions) / std::log2(8);
		
		m_root = new OctreeNode(NodeType::ROOT);
		
		std::vector<OctreeNode*> parentNodes;
		std::vector<OctreeNode*> childNodes;
		std::vector<glm::uvec3> prevVectors;
		std::vector<glm::uvec3> currentVectors;

		parentNodes.push_back(m_root);
		prevVectors.push_back(ROOT_POSITION);
		//current Height level of Tree
		for (int currentHeight = 0; currentHeight <= arrayTreeHeight; currentHeight++) {
			//We keep the previous heights nodes and then loop through them to get
			//The next height levels childs
			
			if (std::pow(8, currentHeight) >= arrayDimensions) {
				for (int c = 0; c < arrayDimensions; c++) {
					std::cout << "first inner Loop" << std::endl;
					//Needs to split up array correctly
					glm::uvec3 parentVector = prevVectors[c];
					glm::uvec3 divisionVector(0,0,0);
					
					parentNodes[c]->getChildrenNodes()[0] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + FRONT_QUADRENT_1;
					glm::uvec3 vec0 = glm::uvec3(std::pow(2,divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[0]->setValue(chunkBlocks[(arrayDimensions/vec0.x)-1][(arrayDimensions/vec0.y)-1][(arrayDimensions/vec0.z)-1]);
					divisionVector = glm::vec3(0, 0, 0);

					parentNodes[c]->getChildrenNodes()[1] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + FRONT_QUADRENT_2;
					glm::uvec3 vec1 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[1]->setValue(chunkBlocks[(arrayDimensions / vec1.x) - 1][(arrayDimensions / vec1.y) - 1][(arrayDimensions / vec1.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[2] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + FRONT_QUADRENT_3;
					glm::uvec3 vec2 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[2]->setValue(chunkBlocks[(arrayDimensions / vec2.x) - 1][(arrayDimensions / vec2.y) - 1][(arrayDimensions / vec2.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[3] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + FRONT_QUADRENT_4;
					glm::uvec3 vec3 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[3]->setValue(chunkBlocks[(arrayDimensions / vec3.x) - 1][(arrayDimensions / vec3.y) - 1][(arrayDimensions / vec3.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[4] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + BACK_QUADRENT_1;
					glm::uvec3 vec4 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[4]->setValue(chunkBlocks[(arrayDimensions / vec4.x) - 1][(arrayDimensions / vec4.y) - 1][(arrayDimensions / vec4.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[5] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + BACK_QUADRENT_2;
					glm::uvec3 vec5 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[5]->setValue(chunkBlocks[(arrayDimensions / vec5.x) - 1][(arrayDimensions / vec5.y) - 1][(arrayDimensions / vec5.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[6] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + BACK_QUADRENT_3;
					glm::uvec3 vec6 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[6]->setValue(chunkBlocks[(arrayDimensions / vec6.x) - 1][(arrayDimensions / vec6.y) - 1][(arrayDimensions / vec6.z) - 1]);

					divisionVector = glm::vec3(0, 0, 0);
					parentNodes[c]->getChildrenNodes()[7] = new OctreeNode(NodeType::LEAF);
					divisionVector = parentVector + BACK_QUADRENT_4;
					glm::uvec3 vec7 = glm::uvec3(std::pow(2, divisionVector.x), std::pow(2, divisionVector.y), std::pow(2, divisionVector.z));
					parentNodes[c]->getChildrenNodes()[7]->setValue(chunkBlocks[(arrayDimensions / vec7.x) - 1][(arrayDimensions / vec7.y) - 1][(arrayDimensions / vec7.z) - 1]);

					std::cout << " PARENT NODE:: " << c << std::endl;
					for (int test = 0; test < 8; test++) {
						std::cout << "Block:: "<< c*8+test << " :: " << parentNodes[c]->getChildrenNodes()[test]->getValue().isActive() << std::endl;
					}
					std::cout << std::endl;
					std::cout << std::endl;
				}
			}
			else {
				for (int p = 0; p < (int)std::pow(8, currentHeight); p++) {
					
					//creates 8 new child node per current parent node
					for (int childPosition = 0; childPosition < 8; childPosition++) {
						childNodes.push_back(parentNodes[p]->getChildrenNodes()[childPosition] = new OctreeNode(NodeType::INTERNAL));
					}
 				
					//this is used to keep track of the division that will eventually take place on the leaf
					//nodes
					currentVectors.push_back(FRONT_QUADRENT_1 + prevVectors[p]);
					currentVectors.push_back(FRONT_QUADRENT_2 + prevVectors[p]);
					currentVectors.push_back(FRONT_QUADRENT_3 + prevVectors[p]);
					currentVectors.push_back(FRONT_QUADRENT_4 + prevVectors[p]);
					currentVectors.push_back(BACK_QUADRENT_1 + prevVectors[p]);
					currentVectors.push_back(BACK_QUADRENT_2 + prevVectors[p]);
					currentVectors.push_back(BACK_QUADRENT_3 + prevVectors[p]);
					currentVectors.push_back(BACK_QUADRENT_4 + prevVectors[p]);
					
				}
			}

		
				prevVectors = currentVectors;
				parentNodes = childNodes;
				currentVectors.clear();
				childNodes.clear();
			
		}

		//We can use parentNodes array and the prevVectors array to "prune tree" by iterating back up it
		
	}

protected:
private:

	OctreeNode* m_root;
	unsigned int m_treeDimensions;
};
