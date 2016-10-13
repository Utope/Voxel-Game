#pragma once
#include "ShaderProgram_Interface.h"


class StaticShader : public ShaderProgram_Interface {
	
public:
	StaticShader() : ShaderProgram_Interface(".\\res\\basicShader.vs", ".\\res\\basicShader.fs") {
		this->bindAttributes();
		this->getAllUniformLocations();
	};
	
	void loadVPmatrix(glm::mat4 vpMatrix) {
		loadMat4X4(location_vpMatrix, vpMatrix);
	}

	void loadModelMatrix(glm::mat4 modelMatrix) {
		loadMat4X4(location_modelMatrix, modelMatrix);
	}
	
	~StaticShader() {};

protected:
	virtual void bindAttributes() override {
		bindAttribute(0, "position");
		bindAttribute(1, "textureCoords");
	}
	virtual void getAllUniformLocations() override {
		location_vpMatrix = getUniformLocation("vpMatrix");
		location_modelMatrix = getUniformLocation("modelMatrix");
	}

private:
	GLuint location_vpMatrix;
	GLuint location_modelMatrix;

};