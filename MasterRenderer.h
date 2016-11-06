#pragma once
#include "ChunkRenderer.h"
#include "DebugRenderer.h"
#include "Camera.h"
#include "GLSL_ShaderProgram.h"
#include "GLSL_ShaderManager.h"
#include "glm\glm.hpp"



class MasterRenderer{
public:
	MasterRenderer();
	~MasterRenderer() {};

	void render(Camera camera);
	DebugRenderer* getDebugRenderer() { return &m_debugRenderer; }
	ChunkRenderer* getChunkRenderer() { return &m_chunkRenderer; }


private:
	ChunkRenderer m_chunkRenderer;
	GLSL_ShaderProgram* m_chunkShader;

	DebugRenderer m_debugRenderer;
	GLSL_ShaderProgram* m_debugShader;

	GLSL_ShaderManager m_shaderManager;
	
	glm::vec4 m_clearColor;
	glm::mat4 m_matrixPerspective;
	glm::mat4 m_matrixView;
	glm::mat4 m_matrixVP;
	glm::mat4 m_matrixOrthographic;

	void _prepare(Camera camera);

	
};

MasterRenderer::MasterRenderer() {
	
	GLSL_ShaderProgram* debugShader = new GLSL_ShaderProgram;
	debugShader->compileShaderFromFile(".\\res\\DebugShader.vs", GLSL_ShaderType::VERTEX);
	debugShader->compileShaderFromFile(".\\res\\DebugShader.fs", GLSL_ShaderType::FRAGMENT);
	debugShader->bindAttribLocation(0, "position");
	debugShader->bindAttribLocation(1, "color");
	debugShader->link();

	/*Temporary way to set shaders*/
	m_shaderManager.addShader("debugShader", debugShader);
	m_debugShader = debugShader;
	m_chunkShader = debugShader;
		
}

void MasterRenderer::render(Camera camera) {
	_prepare(camera);
	
	m_debugShader->bind();
	m_debugShader->setUniform("matrixVP",false, m_matrixVP);
	m_debugRenderer.render();
	m_debugShader->unbind();

	m_chunkShader->bind();
	m_debugShader->setUniform("matrixVP", false, m_matrixVP);
	m_chunkRenderer.render();
	m_chunkShader->unbind();

}

void MasterRenderer::_prepare(Camera camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	//m_orthographic = camera.getOrthograpic();
	m_matrixPerspective = camera.getMatrixPerspective();
	m_matrixView = camera.getMatrixView();
	m_matrixVP = camera.getMatrixVP();

}
