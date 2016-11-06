#pragma once
#include <vector>
#include "GL\glew.h"
#include "RawModel.h"
#include "glm\glm.hpp"
#include "RawModel.h"
#include "ModelTexture.h"
#include "TexturedModel.h"

class Loader {
public:
	Loader() {};
	
	RawModel loadToVAO(	std::vector<glm::vec3> positions, 
						std::vector<glm::vec2> textureCoords,
						std::vector<unsigned int> indices);

	ModelTexture loadTexture(std::string fileName);
	void cleanUp();
	~Loader() {};

private:
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;
	std::vector<GLuint> textures;

	GLuint createVAO();
	void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<glm::vec2> data);
	void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<glm::vec3> data);

	void unbindVAO();
	void bindIndiciesBuffer(std::vector<unsigned int> indices);
	
};

void Loader::unbindVAO() {
	glBindVertexArray(0);
}

GLuint Loader::createVAO() {
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

RawModel Loader::loadToVAO(	std::vector<glm::vec3> positions,
							std::vector<glm::vec2> textureCoords,
							std::vector<unsigned int> indices) {
	
	GLuint vaoID = createVAO();
	bindIndiciesBuffer(indices);
	vaos.push_back(vaoID);
	storeDataInAttributeList(0,3,positions);
	storeDataInAttributeList(1, 2, textureCoords);

	unbindVAO();
	return RawModel(vaoID, indices.size());
};

void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<glm::vec3> data) {
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<glm::vec2> data) {
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndiciesBuffer(std::vector<unsigned int> indices) {
	GLuint vboID;
	vbos.push_back(vboID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

}

ModelTexture Loader::loadTexture(std::string fileName) {
	int w;
	int h;
	int comp;
	stbi_uc* image = stbi_load(fileName.c_str(), &w, &h, &comp, 4);

	if (image == nullptr)
		throw(std::string("Failed to load texture"));
	
	GLuint texture;
	glGenTextures(1, &texture);
	textures.push_back(texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	return ModelTexture(texture);
}

void Loader::cleanUp() {
	for (GLuint vao:vaos){
		glDeleteVertexArrays(1,&vao);
	}
	for (GLuint vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}
	for (GLuint texture : textures) {
		glDeleteTextures(1, &texture);
	}
}
