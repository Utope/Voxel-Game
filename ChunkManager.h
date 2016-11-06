#pragma once
#include<unordered_map>
#include<vector>
#include<iterator>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include "ChunkMesh.h"
#include"Chunk.h"
#include"ChunkRenderer.h"
#include"ChunkTasks.h"
#include "glm\gtx\hash.hpp"

class ChunkManager {

	/*NOTE: The hashMap used in this class will only be changed on loadingRadius size change
	each chunks "Position" is only relative to the loading Radius
	*/

	/* TODO
		Chunk neigbors need to be updated 
		Probably should put ChunkThreading in a separate class
		Update class needs to be done
		multiple Dynamic casting can be replaced by one dynamic cast and static "checks", which is much quicker
		Many tasks don't actually implement everything they need to
			-Deletion, very basic
			-MeshingTask, needs greedy, possibly multiple types, and possibly add a "basic meshing"
			-Loading, libnoise Generation
		Decide on how RenderMethod should look
		Also getting/setting individual voxels
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		CHUNKS NEED TO BE EMPTY INITIALIZED TO TEST FOR "NONLOADEDNESS"
		Way to handle chunk state needs to be better
		Should fix this first!

	*/

public:
	ChunkManager(int loadingRadius, int threadCount);
	~ChunkManager();

	void update(glm::vec3 loadingPosition);
	void render();
	
	void setLoadingRadius(int loadingRadius);
	int getLoadingRadius();

	Chunk* getChunk(glm::ivec3 positionkey) { return m_loadedChunks.find(positionkey)->second; };

private:
	
	std::unordered_map<glm::ivec3, Chunk*> m_loadedChunks;

	//MultiThreading
	std::vector<std::thread*> m_workerThreads;
	std::atomic<bool> m_threadPoolRunning;
	
	//Cond variable for waiting on queued mutex
	std::condition_variable m_conditionalVar;

	//Queued mutex for getting and adding tasks
	std::mutex m_queuedTaskMutex;

	//Mutex for handling processed tasks
	std::mutex m_processedTaskMutex;

	/*MultiThreading_Variable
		called in 
		_ThreadHandlerFunction
		_AddTaskToLowQ
	*/
	std::queue<ChunkTask*> m_lowQ; 
	
	/*MultiThreading_Variable
		called in
		_ThreadHandlerFunction
		_AddTaskToHighQ
	*/
	std::queue<ChunkTask*> m_highQ;

	/*MultiThreading_Variable
		called in
		_ThreadHandlerFunction
		_HandleProccessedTasks
	*/
	std::queue<ChunkTask*> m_processedTasks;
	//This handles each individual thread
	void _threadHandlerFunction();

	//These are for the main function threading
	//Creates a std::vector of tasks. Size based on numProcessedTasksHandledPerFrame
	void _handleProccessedTasks();
	
	//Handles each individual task.
	//DELETES AND CLEANS UP TASK POINTERS HERE
	//MAKE EDITS IN THIS CLASS WHEN ADDING MORE CHUNKTASKS
	void _handleVectorOfProcessedTasks(std::vector<ChunkTask*>);
	
	//Adds a task to the lowQ or highQ respectivly
	//Should be called with "new ...Type of chunkTask"
	void _addTaskToHighQ(ChunkTask*);
	void _addTaskToLowQ(ChunkTask*);

	//Manager Settings

	//Number of proccessedTask we handle each frame
	//Setting it a 0 means we handle ALL tasks
	int m_numProceesedTasksHandledPerFrame;
	
	//Distance to load chunks from Update positions
	int m_loadingRadius;

};

//Constructr and deconstructor
ChunkManager::ChunkManager(int loadingRadius, int threadCount) {
	m_loadingRadius = loadingRadius;
	m_threadPoolRunning.store(true);
	for (int i = 0; i < threadCount; i++) {
		m_workerThreads.push_back(new std::thread(&ChunkManager::_threadHandlerFunction,this));
	}
}

ChunkManager::~ChunkManager() {
	//testing
	m_loadedChunks.insert({ glm::ivec3(0,0,0),new Chunk(glm::ivec3(0,0,0)) });
	m_loadedChunks.insert({ glm::ivec3(0,2,0),new Chunk(glm::ivec3(0,2,0)) });
	m_loadedChunks.insert({ glm::ivec3(0,3,0),new Chunk(glm::ivec3(0,3,0)) });
	if (!m_loadedChunks.empty()) {
		for (std::unordered_map<glm::ivec3, Chunk*>::iterator it = m_loadedChunks.begin(); it != m_loadedChunks.end(); ++it) {
			it->second->setUp(); // test
			it->second->load(); // test
			_addTaskToLowQ(new UnLoadingTask(it->second));
		}
	}

	m_threadPoolRunning.store(false);
	m_conditionalVar.notify_all();
	for (int i = 0; i < m_workerThreads.size(); i++) {
		if (m_workerThreads[i]->joinable()) {
			m_workerThreads[i]->join();
			delete m_workerThreads[i];
		}
	}

}
//Public

void ChunkManager::setLoadingRadius(int loadingRadius) {
	m_loadingRadius = loadingRadius;
}
int ChunkManager::getLoadingRadius() {
	return m_loadingRadius;
}

void ChunkManager::update(glm::vec3 loadingPosition) {
	
}

//Private




//THREADING METHODS

void ChunkManager::_addTaskToHighQ(ChunkTask* task) {
	task->preTask(); //Call preTask for eachTask added
	{
		std::unique_lock<std::mutex> lock(m_queuedTaskMutex);
		m_highQ.push(task);
	}//Automatic release of lock
	m_conditionalVar.notify_one();
}

void ChunkManager::_addTaskToLowQ(ChunkTask* task) {
	task->preTask(); // Call preTask for eachTask added
	{
		std::unique_lock<std::mutex> lock(m_queuedTaskMutex);
		std::cout << "added Task to lowQ" << std::endl;
		m_lowQ.push(task);
	}//Automatic release of lock
	m_conditionalVar.notify_one();
}

void ChunkManager::_handleProccessedTasks() {
	std::vector<ChunkTask*> tasks;
	{
		std::lock_guard<std::mutex> lock(m_processedTaskMutex);

		//If numProcessed... is zero then handle ALL tasks
		if (m_numProceesedTasksHandledPerFrame == 0) {
			while (!m_processedTasks.empty()) {
				tasks.push_back(m_processedTasks.front());
				m_processedTasks.pop();
			}
		}
		else { // If its not zero handle number of tasks given
			for (int i = 0; i < m_numProceesedTasksHandledPerFrame; i++) {
				tasks.push_back(m_processedTasks.front());
				m_processedTasks.pop();
			}
		}
	}
}

//Change multiple dynamic casts to one dynamic cast to speed it up
void ChunkManager::_handleVectorOfProcessedTasks(std::vector<ChunkTask*> tasks) {
	ChunkTask* task;
	for (int i = 0; i < tasks.size(); i++) {
		//In here we handle each individual task
		task = tasks[i];
		task->postTask(); // Call postTask for each processed task

		if (task->getTaskType() == TaskType::LOADING) {
			LoadingTask* loadingTask = dynamic_cast<LoadingTask*>(task);
			
			m_loadedChunks[loadingTask->position] = loadingTask->chunk;			
			
		}
		else if(task->getTaskType() == TaskType::MESHING) {
			MeshingTask* meshingTask = dynamic_cast<MeshingTask*>(task);
			// We send the data to each chunk for mesh processing
			//POSSIBLY CREATE ANOTHER Q FOR THIS FOR BETTER CONTROL
			m_loadedChunks[meshingTask->mapPosition]->getMesh()->updateMesh(&meshingTask->indicies,&meshingTask->positions, &meshingTask->colors, &meshingTask->normals, &meshingTask->textureCoords);
		}
		else if (task->getTaskType() == TaskType::UNLOAD) {
			UnLoadingTask* unloadingTask = dynamic_cast<UnLoadingTask*>(task);
			delete m_loadedChunks.find(unloadingTask->chunk->getPosition())->second;
			m_loadedChunks.erase(unloadingTask->chunk->getPosition());
			
		}
		else {
			assert(false && "A defined task was not called");
		}

		//And delete it
		delete task;
		task = nullptr;
	}
}

void ChunkManager::_threadHandlerFunction() {
	ChunkTask* task = nullptr;

	while (m_threadPoolRunning.load()) {
		{
			std::unique_lock<std::mutex> lock(m_queuedTaskMutex);
			while (m_threadPoolRunning.load() && m_lowQ.empty() && m_highQ.empty()) {
				m_conditionalVar.wait(lock);
			}

			if (!m_highQ.empty()) {
				task = m_highQ.front();
				m_highQ.pop();
			}
			else if (!m_lowQ.empty()) {
				task = m_lowQ.front();
				m_lowQ.pop();
			}
		}//Release m_queuedTaskMutex Lock
		if (task != nullptr) {
			task->executeTask();

			{
				std::unique_lock<std::mutex> lock(m_processedTaskMutex);
				m_processedTasks.push(task);
			}//Release m_processedTask Lock;

			task = nullptr;
		}
		
	}
}
