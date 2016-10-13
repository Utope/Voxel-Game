#pragma once
#include <map>
#include "Chunk.h"
#include "ChunkRenderer.h"

class Chunk;
class ChunkManager {

public:
	ChunkManager();
	~ChunkManager();

	void update();
	void updateLoadingPosition(glm::ivec3 position);
	void updateLoadingPosition(glm::ivec3 position, int loadingRadius);
//	Chunk* getChunkAt(glm::ivec3);

	void _createChunk(ChunkRenderer renderer, glm::ivec3 chunkPosition);

private:
	
	ChunkRenderer *m_pRenderer;
	std::vector<Chunk*> m_updateNeeded;
	std::vector<Chunk*> m_loadingNeeded;
	std::vector<Chunk*> m_unLoadingNeeded;
	std::map<glm::ivec3, Chunk*> m_loadedChunks;
	glm::ivec3 m_loadingCenter;
	int m_loadingRadius;

	
};