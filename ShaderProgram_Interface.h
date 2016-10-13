#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "GL\glew.h"

class ShaderProgram_Interface {
public:
	virtual ~ShaderProgram_Interface() {};

	void start();
	void stop();
	void cleanUp();
protected:
	ShaderProgram_Interface(std::string vertexFile, std::string fragmentFile);
	virtual void getAllUniformLocations() {};
	GLuint getUniformLocation(std::string uniformName);
	void bindAttribute(int attribute, std::string variableName);
	virtual void bindAttributes() { };
	
	void loadFloat(GLuint location, float value);
	void loadVector2D(GLuint location, glm::vec2 value);
	void loadVector3D(GLuint location, glm::vec3 value);
	void loadBoolean(GLuint location, bool value);
	void loadMat3X3(GLuint location, glm::mat3x3 value);
	void loadMat4X4(GLuint location, glm::mat4x4 value);

private:
	GLuint m_programID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	
	static GLuint loadShader(std::string file, GLuint shaderType);
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};


ShaderProgram_Interface::ShaderProgram_Interface(std::string vertexFile, std::string fragmentFile) {
	m_vertexShaderID = ShaderProgram_Interface::loadShader(vertexFile, GL_VERTEX_SHADER);
	m_fragmentShaderID = ShaderProgram_Interface::loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	bindAttributes();
	glLinkProgram(m_programID);
	glValidateProgram(m_programID);
	getAllUniformLocations();
}

GLuint ShaderProgram_Interface::getUniformLocation(std::string uniformName) {
	std::cout << "Got location" << std::endl;
	return glGetUniformLocation(m_programID,uniformName.c_str());
}

void ShaderProgram_Interface::start() {
	glUseProgram(m_programID);
}

void ShaderProgram_Interface::stop() {
	glUseProgram(0);
}

void ShaderProgram_Interface::cleanUp() {
	stop();
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
	glDeleteProgram(m_programID);
}

void ShaderProgram_Interface::loadFloat(GLuint location, float value) {
	glUniform1f(location, value);
}

void ShaderProgram_Interface::loadVector2D(GLuint location, glm::vec2 value) {
	glUniform2f(location, value.x, value.y);
}

void ShaderProgram_Interface::loadVector3D(GLuint location, glm::vec3 value) {
	glUniform3f(location, value.x, value.y, value.z);
}

void ShaderProgram_Interface::loadBoolean(GLuint location, bool value) {
	if (value) {
		glUniform1f(location, 1.0f);
	}
	else {
		glUniform1f(location, 0.0f);
	}
}

void ShaderProgram_Interface::loadMat3X3(GLuint location, glm::mat3x3 value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram_Interface::loadMat4X4(GLuint location, glm::mat4x4 value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

GLuint ShaderProgram_Interface::loadShader(const std::string fileName, GLuint shaderType) {
	//Load shader from a file
	std::fstream shaderFile;
	shaderFile.open(fileName);

	std::string shaderCode;
	std::string line;
	if (shaderFile.is_open()) {
		while (shaderFile.good()) {
			getline(shaderFile, line);
			shaderCode.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}
	/// end load shader from file

	//allocate and compile shader
	GLuint shaderID = glCreateShader(shaderType);
	if (shaderID == 0) {
		std::cerr << "Error: Shader creation failed" << std::endl;
	}


	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = shaderCode.c_str();
	shaderSourceStringLengths[0] = shaderCode.length();

	glShaderSource(shaderID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shaderID);
	
	CheckShaderError(shaderID, GL_COMPILE_STATUS, false, "Error: Shader comilation failed: ");

	return shaderID;
	
}

void ShaderProgram_Interface::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	}
	else {
		glGetShaderiv(shader, flag, &success);
	}

	if (success != GL_TRUE) {
		if (isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cerr << errorMessage << ": " << error << "'" << std::endl;
	}
}

void ShaderProgram_Interface::bindAttribute(int attribute, std::string variableName) {
	glBindAttribLocation(m_programID, attribute, variableName.c_str());
}

