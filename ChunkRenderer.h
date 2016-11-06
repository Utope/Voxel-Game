#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "DebugShader_Positions_Colors.h"
#include <vector>
#include <iostream>
#include "GLSL_ShaderProgram.h"

struct MeshData{
	GLuint meshID_VAO;
	GLuint position_VBO;
	GLuint color_VBO;
	GLuint indices_VBO;
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexColors;
	std::vector<unsigned int> vertexIndices;
};

struct TextureMesh_Data {
	GLuint meshID_VAO;
	GLuint position_VBO;
	GLuint color_VBO;
	GLuint indices_VBO;
	GLuint textureCoords_VBO;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexColors;
	std::vector<unsigned int> vertexIndices;
};

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	MeshData* createMesh();
	void deleteMesh(MeshData* pmeshData);

	void startEditingMesh(MeshData* pmeshID);
	void addTriangle(MeshData* pmeshID, unsigned int vert1, unsigned int vert2, unsigned int vert3);
	void addVertex(MeshData* pmeshID, glm::vec3 position, glm::vec3 color);
	void stopEditingMesh(MeshData* pmeshID);

	void render();

private:
	std::vector<MeshData*> m_meshData;
	GLuint currentlyBound_VAO;
	void flushMeshData(MeshData* pmeshID);
	void clearMeshDataBuffers(MeshData* pmeshID);
};
//Constructor and deconstructor
ChunkRenderer::ChunkRenderer() {};

ChunkRenderer::~ChunkRenderer() {
	for (MeshData *meshData : m_meshData) {
		meshData->vertexPositions.clear();
		meshData->vertexColors.clear();
		meshData->vertexIndices.clear();

		glDeleteBuffers(1, &meshData->position_VBO);
		glDeleteBuffers(1, &meshData->color_VBO);
		glDeleteBuffers(1, &meshData->indices_VBO);
		glDeleteVertexArrays(1, &meshData->meshID_VAO);

		delete meshData;
	}
}

//Public

//Private
MeshData* ChunkRenderer::createMesh() {
	MeshData* mesh = new MeshData;
	m_meshData.push_back(mesh);
	glGenVertexArrays(1, &mesh->meshID_VAO);
	glBindVertexArray(mesh->meshID_VAO);
	
	//Create indices VBO
	glGenBuffers(1, &mesh->indices_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_VBO);

	//Create positions VBO
	glGenBuffers(1, &mesh->position_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->position_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Create Color VBO
	glGenBuffers(1, &mesh->color_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->color_VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	return mesh;
}

void ChunkRenderer::startEditingMesh(MeshData* pmeshData) {
	glBindVertexArray(pmeshData->meshID_VAO);
	currentlyBound_VAO = pmeshData->meshID_VAO;
	pmeshData->vertexIndices.clear();
}

void ChunkRenderer::stopEditingMesh(MeshData* pmeshData) {
	if (!pmeshData->vertexIndices.empty()) {
		flushMeshData(pmeshData);
	}
	glBindVertexArray(0);
	currentlyBound_VAO = 0;
}

void ChunkRenderer::deleteMesh(MeshData* pmeshData) {
	pmeshData->vertexPositions.clear();
	pmeshData->vertexColors.clear();
	pmeshData->vertexIndices.clear();
	
	glDeleteBuffers(1, &pmeshData->position_VBO);
	glDeleteBuffers(1, &pmeshData->color_VBO);
	glDeleteBuffers(1, &pmeshData->indices_VBO);
	glDeleteVertexArrays(1, &pmeshData->meshID_VAO);

	//Delete from m_meshData
}

void ChunkRenderer::flushMeshData(MeshData* pmeshData) {
	
	//Update Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmeshData->indices_VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pmeshData->vertexIndices.size() * sizeof(unsigned int), &pmeshData->vertexIndices[0], GL_STATIC_DRAW);

	//Update Positions
	glBindBuffer(GL_ARRAY_BUFFER, pmeshData->position_VBO);
	glBufferData(GL_ARRAY_BUFFER, pmeshData->vertexPositions.size() * sizeof(glm::vec3), &pmeshData->vertexPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update Colors
	glBindBuffer(GL_ARRAY_BUFFER, pmeshData->color_VBO);
	glBufferData(GL_ARRAY_BUFFER, pmeshData->vertexColors.size() * sizeof(glm::vec3), &pmeshData->vertexColors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	clearMeshDataBuffers(pmeshData);
}

void ChunkRenderer::clearMeshDataBuffers(MeshData* pmeshID) {
	pmeshID->vertexPositions.clear();
	pmeshID->vertexColors.clear();
}

void ChunkRenderer::addVertex(MeshData* pMeshData, glm::vec3 position, glm::vec3 color) {
	if (currentlyBound_VAO == pMeshData->meshID_VAO) {
		pMeshData->vertexPositions.push_back(position);
		pMeshData->vertexColors.push_back(color);
	}
}

void ChunkRenderer::addTriangle(MeshData* pMeshData, unsigned int vert1, unsigned int vert2, unsigned int vert3) {
	if (currentlyBound_VAO == pMeshData->meshID_VAO) {
		pMeshData->vertexIndices.push_back(vert1);
		pMeshData->vertexIndices.push_back(vert2);
		pMeshData->vertexIndices.push_back(vert3);
	}
}

void ChunkRenderer::render() {
	for (MeshData *meshData : m_meshData) {
		glBindVertexArray(meshData->meshID_VAO);
		glDrawElements(GL_TRIANGLES, meshData->vertexIndices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

