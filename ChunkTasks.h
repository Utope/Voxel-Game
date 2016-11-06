#pragma once
#include <vector>
#include <glm\glm.hpp>
#include "Chunk.h"
//Chunk tasks load, save/unload, update mesh
enum TaskType {
	MESHING,LOADING,UNLOAD
};

class ChunkTask {
public:
	virtual void preTask() = 0;
	virtual void postTask() = 0;
	virtual TaskType getTaskType() = 0;
	//Only class that should be called from separate thread
	virtual void executeTask()=0;
};

class MeshingTask : public ChunkTask {
public:

	 //Chunk mesh goes here
	std::vector<unsigned int> indicies;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	glm::ivec3 mapPosition;

	int offset;
	//Need to copy only neccesary data
	MeshingTask(int offset, glm::ivec3 position) {
		this->offset = offset;
		mapPosition = position;
	}

	virtual void executeTask() override {
		greedy();
		std::cout << "greedy" << std::endl;
	};

	virtual TaskType getTaskType() override{
		return TaskType::MESHING;
	}

	virtual void preTask() override{

	}

	virtual void postTask() override {

	}

	void greedy() {};

};

class LoadingTask : public ChunkTask {
public:

	Chunk* chunk;
	glm::ivec3 position;

	LoadingTask(glm::ivec3 position) {
		this->position = position;
		chunk = nullptr;
	}

	virtual void executeTask() override {
		chunk = new Chunk(position);
		std::cout << "creation" << std::endl;
	};

	virtual TaskType getTaskType() override {
		return TaskType::LOADING;
	}

	virtual void preTask() override {
		//Before chunk is loaded what all do we have to do?
		chunk->setUp();
	}

	virtual void postTask() override {

	}

};

// TEst!

class UnLoadingTask : public ChunkTask {
public:
	Chunk* chunk;
	bool success;
	UnLoadingTask(Chunk* chunk) {
		this->chunk = chunk;
	}

	virtual void executeTask() override {
		chunk->unLoad();
	};

	virtual TaskType getTaskType() override {
		return TaskType::UNLOAD;
	}

	virtual void preTask() override {
		chunk->cleanUp();
		//Before chunk is Unloaded what all do we have to do?
	}

	virtual void postTask() override {
	
	}
};