#pragma once
#include <iostream>
#include <atomic>
#include "Voxel.h"
#include "ChunkRenderer.h"
#include "ChunkMesh.h"

/*
how to handle the renderer and chunkk pointers?
*/

#define CHUNK_SIZE 32

class ChunkManager;

enum BlockFace {
	RIGHT = 0, TOP = 1, BACK = 2, LEFT = 3, BOTTOM = 4, FRONT = 5
};

class Chunk {
public:
	Chunk(glm::ivec3 position);
	~Chunk();

	//Loading/Unloading functions
	void setUp();
	void cleanUp();
	void load();
	void unLoad();
	
	glm::ivec3 getPosition() { return m_position; };
	ChunkMesh* getMesh();
	void activateAll();
	void deactivateAll();
	void deactivateBlock(int x, int y, int z);
	void activateBlock(int x, int y, int z);
	
	bool isBlockFaceVisible(glm::vec3 pos, BlockFace face);
	bool isLeftFaceFull();
	bool isRightFaceFull();
	bool isFrontFaceFull();
	bool isBackFaceFull();
	bool isTopFaceFull();
	bool isBottomFaceFull();
	void greedyMeshing();

	Chunk* getTopNeighbor() { return m_top; };
	Chunk* getBottomNeighbor() { return m_bottom; };
	Chunk* getLeftNeighbor() { return m_left; };
	Chunk* getRightNeighbor() { return m_right; };
	Chunk* getFrontNeighbor() { return m_front; };
	Chunk* getBackNeighbor() { return m_back; };

private:
	glm::ivec3 m_position;
	glm::ivec3 m_offset;
	Voxel*** m_voxelArry;
	ChunkManager *m_manager;
	ChunkMesh m_mesh;

	//NeighBorChunks
	Chunk* m_top;
	Chunk* m_bottom;
	Chunk* m_left;
	Chunk* m_right;
	Chunk* m_front;
	Chunk* m_back;
	
	//Atomic flags
	std::atomic<bool> m_setUp;
	std::atomic<bool> m_needsReMeshing;
	std::atomic<bool> m_mayBeVisible;
	std::atomic<bool> m_loaded;
};

//Constructor and Deconstructor
Chunk::Chunk(glm::ivec3 position) {
	m_position = position;
	m_offset = position*CHUNK_SIZE;

	m_setUp.store(false);
	m_loaded.store(false);
	m_needsReMeshing.store(false);
	m_mayBeVisible.store(false);

	m_top = nullptr;
	m_bottom = nullptr;
	m_left = nullptr;
	m_right = nullptr;
	m_front = nullptr;
	m_back = nullptr;
};

Chunk::~Chunk() {
}

//Public

//Must be performed in main Thread!
//Not 100% sure this works or is set up right
void Chunk::setUp() {
	m_mesh.setUp();
	
	Chunk* neighbor;

	//Top Neighbor;
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(0,1,0))) != nullptr) {
		m_top = neighbor;
	}
	neighbor = nullptr;

	//Bottom Neighbor
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(0, -1, 0))) != nullptr) {
		m_bottom = neighbor;
	}
	neighbor = nullptr;

	//Left Neighbor
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(1, 0, 0))) != nullptr) {
		m_left = neighbor;
	}
	neighbor = nullptr;

	//Right Neighbor
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(-1, 0, 0))) != nullptr) {
		m_right = neighbor;
	}
	neighbor = nullptr;

	//Front Neighbor
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(0, 0, 1))) != nullptr) {
		m_front = neighbor;
	}
	neighbor = nullptr;

	//Back Neighbor
	if ((neighbor = m_manager->getChunk(m_position + glm::ivec3(0, 0, -1))) != nullptr) {
		m_back = neighbor;
	}
	neighbor = nullptr;
	m_setUp.store(true);
}

//Must be called in main Thread!
void Chunk::cleanUp() {
	m_mesh.cleanUp();
	
	//change pointers here
	m_setUp.store(false);
}

//Load and unload functions
void Chunk::load() {
	assert(!m_loaded.load() && "Error: chunk already loaded");
	assert(m_setUp.load() && "Error: chunk has not been set up");
	
	m_voxelArry = new Voxel**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_voxelArry[i] = new Voxel*[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_voxelArry[i][j] = new Voxel[CHUNK_SIZE];
		}
	}
	
	//Here we load chunk from file or generate it with libnoise

	m_loaded.store(true);
}

void Chunk::unLoad() {
	assert(m_loaded.load() && "Error: chunk is already unloaded loaded");
	assert(!m_setUp.load() && "Error: chunk is setup must call cleanUp");
	
	for (int i = 0; i <CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			delete[] m_voxelArry[i][j];
		}
		delete[] m_voxelArry[i];
	}
	delete[] m_voxelArry;

	//Unload chunk into memory possibly save it here

	m_loaded.store(false);
}

//END Load and unload Funcitons

void Chunk::deactivateBlock(int x, int y, int z) {
	m_voxelArry[x][y][z].setActive(false);
}

void Chunk::activateBlock(int x, int y, int z) {
	m_voxelArry[x][y][z].setActive(false);
}

ChunkMesh* Chunk::getMesh() {
	return &m_mesh;
}

void Chunk::activateAll() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_voxelArry[i][j][k].setActive(true);
			}
		}
	}
}

void Chunk::deactivateAll() {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_voxelArry[i][j][k].setActive(false);
			}
		}
	}
}

bool Chunk::isBlockFaceVisible(glm::vec3 pos, BlockFace face) {

	int x = (int)pos.x;
	int y = (int)pos.y;
	int z = (int)pos.z;

	if (!m_voxelArry[x][y][z].isActive()) {
		return false;
	}

	if (BlockFace::FRONT == face) {
		if (z - 1 > -1) {
			if (!m_voxelArry[x][y][z-1].isActive()) {
				return true;
			}
			return false;
		}
		else {
			return true;
			// need to check next chunk over
		}
	}
	else if (BlockFace::BOTTOM == face) {
		if (y - 1 > -1) {
			if (!m_voxelArry[x][y-1][z].isActive()) {
				return true;
			}
			return false;
		}
		else {
			return true;
			// need to check next chunk over
		}
	}
	else if (BlockFace::BACK == face) {
		if (z + 1 < CHUNK_SIZE) {
			if (!m_voxelArry[x][y][z+1].isActive()) {
				return true;
			}
			return false;
		}
		else {
			return true;
			// need to check next chunk over
		}

	}
	else if (BlockFace::LEFT == face) {
		if (x - 1 > -1) {
			if (!m_voxelArry[x-1][y][z].isActive()) {
				return true;
			}
			return false;
		}
		else {
			return true;
			// need to check next chunk over
		}

	}
	else if (BlockFace::RIGHT == face) {
		if (x + 1 < CHUNK_SIZE) {
			if (!m_voxelArry[x+1][y][z].isActive()) {
				return true;
			}
			return false;
		}
		else {
			return true;
			// need to check next chunk over
		}

	}
	else if (BlockFace::TOP == face) {
		if (y + 1 < CHUNK_SIZE) {
			if (!m_voxelArry[x][y+1][z].isActive()) {
				return true;
			}
			return false;
		}
		else {
			//need to check next chunk over
			return true;
		}
	}

	return false;
}

void Chunk::greedyMeshing() {

	//this variable will store all of the voxel data for the current plane
	int mask[CHUNK_SIZE * CHUNK_SIZE];


	BlockFace face; // stores face data

	int axis = 0; // currentPlace Axis

				  // iterates over the three axis  // why ++axis?
	for (axis = 0; axis < 3; ++axis) {
		int u = (axis + 1) % 3;
		int v = (axis + 2) % 3;

		int x[3] = { 0,0,0 };
		int q[3] = { 0,0,0 };

		q[axis] = 1;

		//Iterates through a section of the chunk at the current axis, defined by
		//x[axis]. it then check if each face of the voxel on the current axis is visile
		// setting 1 at the mask when it is true and 0 otherwise
		for (x[axis] = -1; x[axis] < CHUNK_SIZE;) {

			int n = 0; //current mask index

					   //Pretty sure this iterates through the 2d mask
					   // x[v] and x[u] are used for x/y corrds in the mask I THINK
			for (x[v] = 0; x[v] < CHUNK_SIZE; ++(x[v])) {
				for (x[u] = 0; x[u] < CHUNK_SIZE; ++(x[u]), ++n) {

					face = (BlockFace)axis;

					glm::vec3 pos = glm::vec3(x[0], x[1], x[2]);

					bool frontFaceVisible = false;
					bool backFaceVisible = false;

					if (0 <= x[axis]) {
						if (isBlockFaceVisible(pos, face)) {
							frontFaceVisible = true;
						}
					}

					face = (BlockFace)(axis + 3);
					glm::vec3 secondPos(x[0] + q[0], x[1] + q[1], x[2] + q[2]);


					if (x[axis] < CHUNK_SIZE - 1) {
						if (isBlockFaceVisible(secondPos, face)) {
							backFaceVisible = true;
						}
					}


					//Determines if we are using front face of back face
					if (frontFaceVisible == false && backFaceVisible == false) {
						mask[n] = 0;  // if they are both equal??
					}
					else if (frontFaceVisible) {
						mask[n] = 1; // if front face??
					}
					else {
						mask[n] = -1; // if back face ?? WUT
					}
				}
			}

			++x[axis];
			n = 0; // current Mask index;

				   // iterate over mask to create the mesh
				   // If mask entry is different than 0, add a face at the corresponding
				   // position. Takes into consideration the handness of the face, as well as
				   // wether it is a front of back face.

			for (int j = 0; j < CHUNK_SIZE; ++j) {
				for (int i = 0; i < CHUNK_SIZE;) {

					int c = mask[n];
					int width, height;

					// Greedy step
					//The algorithm tries first to find the width of the current quad. IT
					// iterates over the row until it hits a voxel whose type is different than the quad's.
					//When this is found, we do the same for the height, getting the biggest quad
					// of our current type as possible. This is repeated for all mask entries that are not null, and 
					//the quads never overlap

					if ((!!c)) {
						for (width = 1; c == mask[n + width] && i + width < CHUNK_SIZE; ++width) {
							//Fuck this loop man they didnt explain shit on the website
						}
						bool done = false;

						for (height = 1; height + j < CHUNK_SIZE; ++height) {
							for (int k = 0; k < width; ++k) {
								if (c != mask[n + k + height*CHUNK_SIZE]) {
									done = true;
									break;
								}
							}

							//WHY DO YOU USE BREAKS WHY
							if (done) {
								break;
							}
						}

						x[u] = i;  // sets correct indicies
						x[v] = j; // sets correct indicies

						int du[3] = { 0,0,0 }; // auxiliary vector for width
						int dv[3] = { 0,0,0 }; // auxiliary vector for height

						if (c > 0) {
							dv[v] = height;
							du[u] = width;
						}
						else {
							c = -c;
							du[v] = height;
							dv[u] = width;
						}

						glm::vec3 botLeft(x[0], x[1], x[2]);
						glm::vec3 botRight(x[0] + du[0], x[1] + du[1], x[2] + du[2]);
						glm::vec3 topLeft(x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]);
						glm::vec3 topRight(x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]);

						botLeft += m_offset;
						botRight += m_offset;
						topLeft += m_offset;
						topRight += m_offset;

						//unsigned int indicies[4];
						/*
						indicies[0] = m_meshData->vertexPositions.size();
						indicies[1] = m_meshData->vertexPositions.size() + 1;
						indicies[2] = m_meshData->vertexPositions.size() + 2;
						indicies[3] = m_meshData->vertexPositions.size() + 3;

						m_pRenderer->addVertex(m_meshData, botLeft, botLeft);
						m_pRenderer->addVertex(m_meshData, botRight, botRight);
						m_pRenderer->addVertex(m_meshData, topLeft, topLeft);
						m_pRenderer->addVertex(m_meshData, topRight, topRight);

						m_pRenderer->addTriangle(m_meshData, indicies[0], indicies[1], indicies[2]);
						m_pRenderer->addTriangle(m_meshData, indicies[2], indicies[3], indicies[0]);
						*/
						for (int l = 0; l < height; ++l) {
							for (int k = 0; k < width; ++k) {
								mask[n + k + l*CHUNK_SIZE] = 0;
							}
						}

						i += width;
						n += width;
					}
					else {
						++i;
						++n;
					}
				}
			}
		}
	}
}

//Private

