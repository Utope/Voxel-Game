#pragma once
#include <vector>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "BufferDataLoader.h"

class ChunkMesh {
public:
	//init and destroy functions
	ChunkMesh() {};
	~ChunkMesh() {};
	void setUp();
	void cleanUp();

	//Drawing functions
	unsigned int getIndices();
	void bind();
	void unBind();

	void enableAllVertexAttribArrys();
	void disableAllVertexAttribArrys();

	// Position, Normals, Colors, TextureCoords
	void enableVertexAttribArrys(bool, bool, bool, bool);

	//indices, positions, colors, normals, textureCoords... Send in nullptr if you dont want to update vbo
	void updateMesh(std::vector<unsigned int>* indices, std::vector<glm::vec3>* positions, std::vector<glm::vec3>* colors, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* textureCoords);
	
private:
	GLuint m_vao;
	GLuint m_indices;
	GLuint m_vertexPositions;
	GLuint m_vertexNormals;
	GLuint m_vertexTextureCoords;
	GLuint m_vertexColors;
	unsigned int m_textureCoordsSize;
	unsigned int m_positionsSize;
	unsigned int m_indexsSize;
	unsigned int m_normalsSize;
	unsigned int m_colorsSize;

};

//Setup funcions
void ChunkMesh::setUp() {
	m_textureCoordsSize = 0;
	m_positionsSize = 0;
	m_indexsSize = 0;
	m_normalsSize = 0;
	m_colorsSize = 0;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	
	//attrib 0
	glGenBuffers(1, &m_vertexPositions);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Attrib 1
	glGenBuffers(1, &m_vertexNormals);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Attrib 2
	glGenBuffers(1, &m_vertexColors);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexColors);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Attrib 3
	glGenBuffers(1, &m_vertexTextureCoords);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexTextureCoords);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void ChunkMesh::cleanUp() {
	glDeleteBuffers(1, &m_vertexPositions);
	glDeleteBuffers(1, &m_vertexNormals);
	glDeleteBuffers(1, &m_vertexColors);
	glDeleteBuffers(1, &m_vertexTextureCoords);
	glDeleteVertexArrays(1, &m_vao);
}

//Drawing Functions
unsigned int ChunkMesh::getIndices() {
	return m_indexsSize;
}

void ChunkMesh::bind() {
	glBindVertexArray(m_vao);
}

void ChunkMesh::unBind() {
	glBindVertexArray(0);
}

void ChunkMesh::enableAllVertexAttribArrys() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

void ChunkMesh::disableAllVertexAttribArrys() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void ChunkMesh::enableVertexAttribArrys(bool positions, bool normals, bool colors, bool textureCoods) {
	if (positions){
		glEnableVertexAttribArray(0);
	}

	if (normals) {
		glEnableVertexAttribArray(1);
	}

	if (colors) {
		glEnableVertexAttribArray(2);
	}

	if (textureCoods) {
		glEnableVertexAttribArray(3);
	}
}

//Update Functions

void ChunkMesh::updateMesh(std::vector<unsigned int>* indices, std::vector<glm::vec3>* positions, std::vector<glm::vec3>* colors, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* textureCoords) {

	glBindVertexArray(m_vao);

	//Update indicies if not nullptr
	if (indices != nullptr) {
		BufferDataLoader::loadIndicies(m_indices, indices, m_indexsSize);
		m_indexsSize = indices->size();
	}

	//Update positions if not nullptr
	if (positions != nullptr) {
		BufferDataLoader::LoadDataIntoAttribArray(m_vertexPositions, positions, m_positionsSize);
		m_positionsSize = positions->size();
	}

	//Update colors if not nullptr
	if (colors != nullptr) {
		BufferDataLoader::LoadDataIntoAttribArray(m_vertexColors, colors, m_colorsSize);
		m_colorsSize = colors->size();
	}

	//Update normals if not nullptr
	if (normals != nullptr) {
		BufferDataLoader::LoadDataIntoAttribArray(m_vertexNormals, normals, m_normalsSize);
		m_normalsSize = normals->size();
	}

	//Update textureCoords if not nullptr
	if (textureCoords != nullptr) {
		BufferDataLoader::LoadDataIntoAttribArray(m_vertexColors, textureCoords, m_textureCoordsSize);
		m_textureCoordsSize = textureCoords->size();
	}

	glBindVertexArray(0);
}