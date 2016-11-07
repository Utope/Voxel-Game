#pragma once
#include "GL\glew.h"
#include "Entity.h"
#include "glm\gtx\transform.hpp"
#include "Camera.h"
#include "BasicShader.h"
#include "Chunk.h"
#include "GLSL_ShaderProgram.h"

class Renderer {
public:
	Renderer() {};
	void prepare();

	void render(Entity entity, GLSL_ShaderProgram& shader, Camera camera);
	~Renderer() {};
private:

};

void Renderer::prepare() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void Renderer::render(Entity entity, GLSL_ShaderProgram& shader,Camera camera) {
	TexturedModel texturedModel = entity.getTexturedModel();
	RawModel rawModel = texturedModel.getRawModel();

	glBindVertexArray(rawModel.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Loading unifrom variables
	shader.setUniform("vpMatrix",false,camera.getMatrixVP());
	shader.setUniform("modelMatrix", false, entity.getModelMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.getModelTexture().getTextureID());
	glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

