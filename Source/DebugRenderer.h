#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "DebugShader_Positions_Colors.h"
#include "Camera.h"

class DebugRenderer {
public:

	DebugRenderer();
	~DebugRenderer();

	void addLine(glm::vec3 pointOne, glm::vec3 point2, glm::vec3 color);
	void addLine(glm::vec3 pointOne, glm::vec3 point2);
	void addCube(std::vector<glm::vec3> corners);
	void drawAxis();
	
	void render();

protected:
private:

	glm::vec3 m_defaultColor;

	GLuint m_VAO;
	GLuint m_posVBO;
	GLuint m_colorVBO;


	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_colors;

	void _createLineVBOs();
	void _deleteVBOs();
	void _clearVectors();
	
};
//Constructor and deconstructor
DebugRenderer::DebugRenderer() {
	glGenVertexArrays(1, &m_VAO);
	m_defaultColor = glm::vec3(2.0f, 2.0f, 2.0f);
};

DebugRenderer::~DebugRenderer() {
	glDeleteVertexArrays(1, &m_VAO);
};
//Setters and getters

//Public
void DebugRenderer::drawAxis() {
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
}

void DebugRenderer::addCube(std::vector<glm::vec3> corners) {
	//0,1  4,5
	//3,2  7,6
	addLine(corners[0],corners[1]);
	addLine(corners[1], corners[2]);
	addLine(corners[2], corners[3]);
	addLine(corners[3], corners[0]);

	addLine(corners[4], corners[5]);
	addLine(corners[5], corners[6]);
	addLine(corners[6], corners[7]);
	addLine(corners[7], corners[4]);

	addLine(corners[0], corners[4]);
	addLine(corners[1], corners[5]);
	addLine(corners[3], corners[7]);
	addLine(corners[2], corners[6]);

}

void DebugRenderer::addLine(glm::vec3 pointOne, glm::vec3 pointTwo) {
	m_positions.push_back(pointOne);
	m_positions.push_back(pointTwo);

	m_colors.push_back(m_defaultColor);
	m_colors.push_back(m_defaultColor);
}

void DebugRenderer::addLine(glm::vec3 pointOne, glm::vec3 pointTwo, glm::vec3 color) {
	m_positions.push_back(pointOne);
	m_positions.push_back(pointTwo);

	m_colors.push_back(color);
	m_colors.push_back(color);
}

void DebugRenderer::render() {
	if (!m_positions.empty()) {
		glBindVertexArray(m_VAO);
		_createLineVBOs();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_LINES, 0, m_positions.size());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		_deleteVBOs();
		_clearVectors();
		glBindVertexArray(0);
	}
}

//Private

void DebugRenderer::_clearVectors() {
	m_positions.clear();
	m_colors.clear();
}

void DebugRenderer::_deleteVBOs() {
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteBuffers(1, &m_posVBO);
}

void DebugRenderer::_createLineVBOs() {
	
	glGenBuffers(1, &m_posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVBO);
	glBufferData(GL_ARRAY_BUFFER,m_positions.size() * sizeof(m_positions[0]), &m_positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(m_colors[0]), &m_colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}




