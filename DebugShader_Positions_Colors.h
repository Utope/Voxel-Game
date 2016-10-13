#pragma once
#include "ShaderProgram_Interface.h"
#include "glm\glm.hpp"

class DebugShader_Positions_Colors : public ShaderProgram_Interface {

public:
	DebugShader_Positions_Colors() : ShaderProgram_Interface(".\\res\\DebugShader.vs", ".\\res\\DebugShader.fs") {
		this->bindAttributes();
		this->getAllUniformLocations();
	};

	void loadVPmatrix(glm::mat4 vpMatrix) {
		loadMat4X4(location_vpMatrix, vpMatrix);
	}

	~DebugShader_Positions_Colors() {};

protected:
	virtual void bindAttributes() override {
		bindAttribute(0, "position");
		bindAttribute(1, "color");
	}
	virtual void getAllUniformLocations() override {
		location_vpMatrix = getUniformLocation("vpMatrix");
	}

private:
	GLuint location_vpMatrix;

};