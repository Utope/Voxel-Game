#pragma once

#include "ShaderProgram_Interface.h"
#include "glm\glm.hpp"

class BasicShader : public ShaderProgram_Interface {

public:
	BasicShader() : ShaderProgram_Interface(".\\res\\Test_Positions_Shader.vs", ".\\res\\Test_Positions_Shader.fs") {
		this->bindAttributes();
		this->getAllUniformLocations();
	};

	void loadVPmatrix(glm::mat4 vpMatrix) {
		loadMat4X4(location_vpMatrix, vpMatrix);
	}

	~BasicShader() {};

protected:
	virtual void bindAttributes() override {
		bindAttribute(0, "position");
	}
	virtual void getAllUniformLocations() override {
		location_vpMatrix = getUniformLocation("vpMatrix");
	}

private:
	GLuint location_vpMatrix;

};