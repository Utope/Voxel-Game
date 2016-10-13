#pragma once
#include "SpriteBillboard.h"
#include "vector"

class SpriteBillboardRenderer {
public:
	SpriteBillboardRenderer();
	~SpriteBillboardRenderer();
	
	void render();
	void addSpriteBillboard(SpriteBillboard* sprite);
	void flushBuffer();

private:
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec2> m_textureCoords;
	std::vector<unsigned int> m_vertexIndices;

	unsigned short m_maxBufferSize;

	void _computeIndexs();
	void _sortSpriteBatch();
	void _updateVBOs();

	GLuint m_VAO;
	GLuint m_positionVBO;
	GLuint m_colorVBO;
	GLuint m_textureCoordsVBO;
	GLuint m_indexVBO;
};

//Constructor and Destructor
SpriteBillboardRenderer::SpriteBillboardRenderer() {
	m_maxBufferSize = 1000;
	_computeIndexs();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_textureCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &m_indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndices.size() * sizeof(unsigned short), &m_vertexIndices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

SpriteBillboardRenderer::~SpriteBillboardRenderer() {

}
//

//Public Methods

//Private Methods
void SpriteBillboardRenderer::_computeIndexs() {
	for (int i = 0; i < m_maxBufferSize * 3; i+=4) {
		m_vertexIndices.push_back(0+i);
		m_vertexIndices.push_back(1+i);
		m_vertexIndices.push_back(2+i);
		m_vertexIndices.push_back(0+i);
		m_vertexIndices.push_back(2+i);
		m_vertexIndices.push_back(3 + i);
	}

}

void SpriteBillboardRenderer::_updateVBOs() {
	glBindVertexArray(m_VAO);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}