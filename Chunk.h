#pragma once
#include "Block.h"
#include "ChunkManager.h"
#include "ChunkRenderer.h"
#include <iostream>

#define CHUNK_SIZE 16

struct VoxelFace {
	bool transparent;
	BlockType* blocktype;
	int side;

	VoxelFace* equals(VoxelFace* other) {
		if (this->transparent == other->transparent && this->blocktype == other->blocktype) {
			return other;
		}
		return nullptr;
	}
};

enum BlockFace {
	SOUTH,NORTH,EAST,WEST,TOP,BOTTOM
};

struct BlockFaceTesting {
	bool X_FACE1 = false;
	bool X_FACE2 = false;
	bool Y_FACE1 = false;
	bool Y_FACE2 = false;
	bool Z_FACE1 = false;
	bool Z_FACE2 = false;

	void resetFaces() {
		X_FACE1 = false;
		X_FACE2 = false;
		Y_FACE1 = false;
		Y_FACE2 = false;
		Z_FACE1 = false;
		Z_FACE2 = false;
	}
};

class Chunk {
public:
	Chunk(ChunkRenderer& renderer , glm::ivec3 position) {
		this->m_position = position;
		m_pRenderer = &renderer;
		m_meshData = m_pRenderer->createMesh();

		m_pBlocks = new Block**[CHUNK_SIZE];
		for (int i = 0; i < CHUNK_SIZE;i++) {
			
			m_pBlocks[i] = new Block*[CHUNK_SIZE];
				
			for (int j = 0; j < CHUNK_SIZE; j++) {
				m_pBlocks[i][j] = new Block[CHUNK_SIZE];
			}
		}

	};
	~Chunk() {
		for (int i = 0; i < CHUNK_SIZE; ++i)
		{
			for (int j = 0; j < CHUNK_SIZE; ++j)
			{
				delete[] m_pBlocks[i][j];
			}

			delete[] m_pBlocks[i];
		}
		delete[] m_pBlocks;
	}

	void updateMesh();
	void activateAll();
	void deactivateAll();
	void deactivateBlock(int x, int y, int z);
	void activateBlock(int x, int y, int z);
	void replaceBlock(int x, int y, int z, BlockType* type);
	void createBlockMesh(int x, int y, int z, std::vector<bool>& activeFace);
	bool isBlockVisible(int x, int y, int z, std::vector<bool>& visibility);
	void updateVisibleFaceStruct(int x, int y, int z, BlockFaceTesting& faces);
	
	void addQuad(glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool backFace);
	void greedyMeshing();
	VoxelFace* getVoxelFace(int x, int y, int z, int side);
private:
	MeshData* m_meshData;
	ChunkRenderer* m_pRenderer;
	glm::ivec3 m_position;

	Block*** m_pBlocks;
};

void Chunk::deactivateBlock(int x, int y, int z) {
	m_pBlocks[x][y][z].setActive(false);
}

void Chunk::activateBlock(int x, int y, int z) {
	m_pBlocks[x][y][z].setActive(true);
}

void Chunk::updateMesh() {
	m_pRenderer->startEditingMesh(m_meshData);
	greedyMeshing();
	/*
	std::vector<bool> visibility({ true, true, true,true,true,true,true });
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				visibility[0] = true;
				visibility[1] = true;
				visibility[2] = true;
				visibility[3] = true;
				visibility[4] = true;
				visibility[5] = true;
				bool visible = isBlockVisible(x, y, z, visibility);
				if (m_pBlocks[x][y][z].isActive() && visible)
				{
					createBlockMesh(x, y, z,visibility);
					// Don't create triangle data for inactive blocks
				}
				
			}
		}
	}

	*/
	m_pRenderer->stopEditingMesh(m_meshData);
	
		
}
/*
bool Chunk::isBlockVisible(int x, int y, int z, std::vector<bool>& visibility) {
	short counter = 0;
		
		if (x != 0) {
			if (m_pBlocks[x - 1][y][z].isActive()) {
				visibility[BlockFace::LEFT] = false;
				++counter;
			}
		}
		if (x != CHUNK_SIZE - 1) {
			if (m_pBlocks[x + 1][y][z].isActive()) {
				visibility[BlockFace::RIGHT] = false;
				++counter;
			}
		}
		if (y < CHUNK_SIZE - 1) {
			if (m_pBlocks[x][y + 1][z].isActive()) {
				visibility[BlockFace::UP] = false;
				++counter;
			}
		}
		if (y != 0) {
			if (m_pBlocks[x][y - 1][z].isActive()) {
				visibility[BlockFace::DOWN] = false;
				++counter;
			}
		}
		if (z != 0) {
			if (m_pBlocks[x][y][z - 1].isActive()) {
				visibility[BlockFace::BACK] = false;
				++counter;
			}
		}
		if (z < CHUNK_SIZE - 1) {
			if (m_pBlocks[x][y][z + 1].isActive()) {
				visibility[BlockFace::FRONT] = false;
				++counter;
			}
		}

		if (counter == 6) {
			return false;
		}
		
	return true;
}
*/
/*
void Chunk::createBlockMesh(int x, int y, int z, std::vector<bool>& activeFace) {
	
	unsigned int indicies[8];
	
	indicies[0] = m_meshData->vertexPositions.size();
	indicies[1] = m_meshData->vertexPositions.size() + 1;
	indicies[2] = m_meshData->vertexPositions.size() + 2;
	indicies[3] = m_meshData->vertexPositions.size() + 3;
	indicies[4] = m_meshData->vertexPositions.size() + 4;
	indicies[5] = m_meshData->vertexPositions.size() + 5;
	indicies[6] = m_meshData->vertexPositions.size() + 6;
	indicies[7] = m_meshData->vertexPositions.size() + 7;

	int chunkOffestX = m_position.x*CHUNK_SIZE;
	int chunkOffestY = m_position.y*CHUNK_SIZE;
	int chunkOffestZ = m_position.z*CHUNK_SIZE;
	
	m_pRenderer->addVertex(m_meshData, glm::vec3(x-0.5f + chunkOffestX, y-0.5f + chunkOffestY, z+0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f / z)); //V0
	m_pRenderer->addVertex(m_meshData, glm::vec3(x+0.5f + chunkOffestX, y-0.5f + chunkOffestY, z+0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f/ z)); //V1
	m_pRenderer->addVertex(m_meshData, glm::vec3(x+0.5f + chunkOffestX, y+0.5f + chunkOffestY, z+0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f / z)); //V2
	m_pRenderer->addVertex(m_meshData, glm::vec3(x-0.5f + chunkOffestX, y+0.5f + chunkOffestY, z+0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f / z)); //V3
	m_pRenderer->addVertex(m_meshData, glm::vec3(x+0.5f + chunkOffestX, y-0.5f + chunkOffestY, z-0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f / z)); //V4
	m_pRenderer->addVertex(m_meshData, glm::vec3(x-0.5f + chunkOffestX, y-0.5f + chunkOffestY, z-0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f / z)); //V5
	m_pRenderer->addVertex(m_meshData, glm::vec3(x-0.5f + chunkOffestX, y+0.5f + chunkOffestY, z-0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f/ z)); //V6
	m_pRenderer->addVertex(m_meshData, glm::vec3(x+0.5f + chunkOffestX, y+0.5f + chunkOffestY, z-0.5f + chunkOffestZ), glm::vec3(1.0f / x, 1.0f / y, 1.0f)); //V7
	
	//Front
	if (activeFace[BlockFace::FRONT]) {
		m_pRenderer->addTriangle(m_meshData, indicies[0], indicies[1], indicies[2]);
		m_pRenderer->addTriangle(m_meshData, indicies[0], indicies[2], indicies[3]);
	}
	//Back
	if (activeFace[BlockFace::BACK]) {
		m_pRenderer->addTriangle(m_meshData, indicies[4], indicies[5], indicies[6]);
		m_pRenderer->addTriangle(m_meshData, indicies[4], indicies[6], indicies[7]);
	}
	//Left
	if (activeFace[BlockFace::LEFT]) {
		m_pRenderer->addTriangle(m_meshData, indicies[5], indicies[0], indicies[3]);
		m_pRenderer->addTriangle(m_meshData, indicies[5], indicies[3], indicies[6]);
	}
	//Right
	if (activeFace[BlockFace::RIGHT]) {
		m_pRenderer->addTriangle(m_meshData, indicies[1], indicies[4], indicies[7]);
		m_pRenderer->addTriangle(m_meshData, indicies[1], indicies[7], indicies[2]);
	}
	//Top
	if (activeFace[BlockFace::UP]) {
		m_pRenderer->addTriangle(m_meshData, indicies[3], indicies[2], indicies[7]);
		m_pRenderer->addTriangle(m_meshData, indicies[3], indicies[7], indicies[6]);
	}
	//Bottom
	if (activeFace[BlockFace::DOWN]) {
		m_pRenderer->addTriangle(m_meshData, indicies[5], indicies[4], indicies[1]);
		m_pRenderer->addTriangle(m_meshData, indicies[5], indicies[1], indicies[0]);
	}
}
*/
void Chunk::activateAll() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_pBlocks[i][j][k].setActive(true);
			}
		}
	}
}

void Chunk::deactivateAll() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_pBlocks[i][j][k].setActive(false);
			}
		}
	}
}

void Chunk::updateVisibleFaceStruct(int x, int y, int z, BlockFaceTesting& facesVisible) {

	if (x != 0) {
		if (!m_pBlocks[x - 1][y][z].isActive()) {
			facesVisible.X_FACE1 = true;
		}
	}
	if (x != CHUNK_SIZE - 1) {
		if (!m_pBlocks[x + 1][y][z].isActive()) {
			facesVisible.X_FACE2 = true;
		}
	}
	if (y < CHUNK_SIZE - 1) {
		if (m_pBlocks[x][y - 1][z].isActive()) {
			facesVisible.Y_FACE1 = true;
		}
	}
	if (y != 0) {
		if (m_pBlocks[x][y - 1][z].isActive()) {
			facesVisible.Y_FACE2 = true;
		}
	}
	if (z != 0) {
		if (m_pBlocks[x][y][z - 1].isActive()) {
			facesVisible.Z_FACE1 = true;
		}
	}
	if (z < CHUNK_SIZE - 1) {
		if (m_pBlocks[x][y][z + 1].isActive()) {
			facesVisible.Z_FACE2 = true;
		}
	}
}

void Chunk::greedyMeshing() {
	int SOUTH = 0;
	int NORTH = 1;
	int EAST = 2;
	int WEST = 3;
	int TOP = 4;
	int BOTTOM = 5;
	
	int i, j, k, l, w, h, u, v, n, side = 0;
	
	int x[3] = { 0, 0, 0 };
	int q[3] = { 0, 0, 0 };
	int du[3] = { 0, 0, 0 };
	int dv[3] = { 0, 0, 0 };
	
	VoxelFace* mask[CHUNK_SIZE * CHUNK_SIZE];

	VoxelFace* voxelFace;
	VoxelFace* 	voxelFace1;
	
	for (bool backFace = true, b = false; b != backFace; backFace = backFace&& b, b = !b) {
		
		for (int d = 0; d < 3; d++) {
			u = (d + 1) % 3;
			v = (d + 2) % 3;

			x[0] = 0;
			x[1] = 0;
			x[2] = 0;

			q[0] = 0;
			q[1] = 0;
			q[2] = 0;
			q[d] = 1;

			//variable = condition ? value_if_true : value_if_false
			if (d == 0) {
				side = backFace ? WEST : EAST;
			}else if (d==1) {
				side = backFace ? BOTTOM : TOP;
			}else if (d==2) {
				side = backFace ? SOUTH : NORTH;
			}

			for (x[d] = -1; x[d] < CHUNK_SIZE;) {
				n = 0;

				for (x[v] = 0; x[v] < CHUNK_SIZE; x[v]++) {
					for (x[u] = 0; x[u] < CHUNK_SIZE; x[u]++) {
						
						voxelFace =  (x[d] >= 0) ? getVoxelFace(x[0], x[1], x[2], side) : nullptr;
						voxelFace1 = (x[d] < CHUNK_SIZE - 1) ? getVoxelFace(x[0] + q[0], x[1] + q[1], x[2] + q[2], side) : nullptr;
						
						mask[n++] = ((voxelFace != nullptr && voxelFace1 != nullptr) && voxelFace->equals(voxelFace1)) ? nullptr : backFace ? voxelFace1 : voxelFace1;
					
					}
				}

				x[d]++;

				n = 0;

				for (j = 0; j < CHUNK_SIZE; j++) {
					for (i = 0; i < CHUNK_SIZE; i++) {
						if (mask[n] != nullptr) {
							for (w = 1; i + w < CHUNK_SIZE && mask[n + w] != nullptr && mask[n + w]->equals(mask[n]); w++) {

								bool done = false;

								for (h = 1; j + h < CHUNK_SIZE; h++) {
									for (k = 0; k < w; k++) {
										
										if (mask[n + k + h*CHUNK_SIZE] == nullptr || !mask[n + k + h*CHUNK_SIZE]->equals(mask[n])) {
											done = true;
											break;
										}

										if (done) {
											break;
										}
									}

									if (!mask[n]->transparent) {
										x[u] = i;
										x[v] = j;

										du[0] = 0;
										du[1] = 0;
										du[2] = 0;
										du[u] = w;

										dv[0] = 0;
										dv[1] = 0;
										dv[2] = 0;
										dv[v] = h;

										glm::vec3 bottomLeft = glm::vec3(x[0], x[1], x[2]);
										glm::vec3 topLeft = glm::vec3(x[0] + du[0], x[1]+du[1], x[2]+du[2]);
										glm::vec3 topRight = glm::vec3(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]);
										glm::vec3 bottomRight = glm::vec3(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]);
										int width = w;
										int height = h;
										mask[n]; // voxel face
										backFace;

										addQuad(bottomLeft, topLeft, topRight, bottomRight, backFace);
									}
								}
							}
						}
					}
				}
			}

				
		}
	}
	
}

VoxelFace* Chunk::getVoxelFace(int x, int y, int z, int side) {
	VoxelFace* voxelFace = new VoxelFace;
	voxelFace->side = side;
	voxelFace->transparent = false;
	voxelFace->blocktype = m_pBlocks[x][y][z].getBlockType();
	return voxelFace;
}

void Chunk::addQuad(glm::vec3 botLeft, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, bool backFace) {
	
	
	unsigned int indicies[4];

	indicies[0] = m_meshData->vertexPositions.size();
	indicies[1] = m_meshData->vertexPositions.size() + 1;
	indicies[2] = m_meshData->vertexPositions.size() + 2;
	indicies[3] = m_meshData->vertexPositions.size() + 3;

	m_pRenderer->addVertex(m_meshData, botLeft, glm::vec3(0.0f,0.0f,0.0f));
	m_pRenderer->addVertex(m_meshData, topLeft, glm::vec3(0.0f, 0.0f, 0.0f));
	m_pRenderer->addVertex(m_meshData, topRight, glm::vec3(0.0f, 0.0f, 0.0f));
	m_pRenderer->addVertex(m_meshData, botRight, glm::vec3(0.0f, 0.0f, 0.0f));
	
	if (backFace) {
		m_pRenderer->addTriangle(m_meshData, indicies[2],indicies[0],indicies[1]);
		m_pRenderer->addTriangle(m_meshData, indicies[1], indicies[3], indicies[2]);
	}
	else {
		m_pRenderer->addTriangle(m_meshData, indicies[2], indicies[3], indicies[1]);
		m_pRenderer->addTriangle(m_meshData, indicies[1], indicies[0], indicies[2]);
	}

}

