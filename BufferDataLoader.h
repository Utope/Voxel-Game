#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "GL\glew.h"

class BufferDataLoader {
public:

	static void loadIndicies(GLuint vbo, std::vector<unsigned int>* indices, unsigned int previousSize);
	static void loadIndicies(GLuint vbo, std::vector<unsigned short>* indices, unsigned int previousSize);

	//Floats
	static void LoadDataIntoAttribArray(GLuint vbo, std::vector<glm::vec3>* data, unsigned int previousSize);
	static void LoadDataIntoAttribArray(GLuint vbo, std::vector<glm::vec2>* data, unsigned int previousSize);
	static void LoadDataIntoAttribArray(GLuint vbo, std::vector<float>* data, unsigned int previousSize);
	
	//Ints
	
	//Shorts

	//Bytes

	//Unsigned Ints

	//Unsigned Shorts

	//Unsigned bytes

};

void BufferDataLoader::loadIndicies(GLuint vbo, std::vector<unsigned int>* indices, unsigned int previousSize) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, previousSize * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void BufferDataLoader::loadIndicies(GLuint vbo, std::vector<unsigned short>*  indices, unsigned int previousSize) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, previousSize * sizeof(unsigned short), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void BufferDataLoader::LoadDataIntoAttribArray(GLuint vbo, std::vector<glm::vec3>* data, unsigned int previousSize) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, previousSize * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferDataLoader::LoadDataIntoAttribArray(GLuint vbo, std::vector<glm::vec2>* data, unsigned int previousSize) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, previousSize * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferDataLoader::LoadDataIntoAttribArray(GLuint vbo, std::vector<float>* data, unsigned int previousSize) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, previousSize * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}