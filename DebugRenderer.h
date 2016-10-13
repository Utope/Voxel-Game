#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "DebugShader_Positions_Colors.h"
#include "Camera.h"

class DebugRenderer {
public:

	DebugRenderer() {
		glGenVertexArrays(1, &m_lineVAO);
		m_defaultColor = glm::vec3(2.0f, 2.0f, 2.0f);
	};

	~DebugRenderer() {
		glDeleteVertexArrays(1, &m_lineVAO);
	};

	void addLine(glm::vec3 pointOne, glm::vec3 point2, glm::vec3 color);
	void addLine(glm::vec3 pointOne, glm::vec3 point2);
	void drawAxis();
	
	void render();

protected:
private:

	glm::vec3 m_defaultColor;

	GLuint m_lineVAO;
	GLuint m_posVBO;
	GLuint m_colorVBO;

	std::vector<glm::vec3> m_linePositions;
	std::vector<glm::vec3> m_lineColors;

	void createLineVBOs();
	void deleteVBOs();
	void clearVectors();
	
};

void DebugRenderer::drawAxis() {
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
}

void DebugRenderer::addLine(glm::vec3 pointOne ,glm::vec3 pointTwo) {
	m_linePositions.push_back(pointOne);
	m_linePositions.push_back(pointTwo);
	m_lineColors.push_back(m_defaultColor);
	m_lineColors.push_back(m_defaultColor);
}

void DebugRenderer::addLine(glm::vec3 pointOne, glm::vec3 pointTwo, glm::vec3 color) {
	m_linePositions.push_back(pointOne);
	m_linePositions.push_back(pointTwo);
	m_lineColors.push_back(color);
	m_lineColors.push_back(color);
}

void DebugRenderer::render() {
	if (!m_linePositions.empty()) {
		glBindVertexArray(m_lineVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		createLineVBOs();
		glDrawArrays(GL_LINES, 0, m_linePositions.size());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		deleteVBOs();
		clearVectors();
		glBindVertexArray(0);
	}
}

void DebugRenderer::clearVectors() {
	m_linePositions.clear();
	m_lineColors.clear();
}

void DebugRenderer::deleteVBOs() {
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteBuffers(1, &m_posVBO);
}

void DebugRenderer::createLineVBOs() {
	glGenBuffers(1, &m_posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVBO);
	glBufferData(GL_ARRAY_BUFFER,m_linePositions.size() * sizeof(m_linePositions[0]), &m_linePositions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_lineColors.size() * sizeof(m_lineColors[0]), &m_lineColors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}




