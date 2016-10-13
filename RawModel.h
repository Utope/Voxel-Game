#pragma once
#include "GL\glew.h"

class RawModel {
public:
	RawModel() {};
	RawModel(GLuint vaoID, unsigned int m_vertexCount) {
		this->m_vaoID = vaoID;
		this->m_vertexCount = m_vertexCount;
	}

	GLuint getVaoID();
	unsigned int getVertexCount();
	
	~RawModel() {};
private:
	GLuint m_vaoID;
	unsigned int m_vertexCount;
};

GLuint RawModel::getVaoID() {
	return this->m_vaoID;
}

unsigned int RawModel::getVertexCount() {
	return this->m_vertexCount;
}
