#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "GL\glew.h"
#include "glm\glm.hpp"

// https://www.packtpub.com/books/content/opengl-40-building-c-shader-program-class

enum GLSL_ShaderType {
	VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, 
	TESS_CONTROL = GL_TESS_CONTROL_SHADER, TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};

class GLSL_ShaderProgram {

public:
	GLSL_ShaderProgram();
	~GLSL_ShaderProgram();

	void compileShaderFromFile(std::string fileName, GLSL_ShaderType type);
	void compileShaderFromString(std::string sourceString, GLSL_ShaderType type);

	void link();
	bool isLinked();
	GLuint getShaderProgramHandle();

	std::string getActiveUnifoms();
	std::string getActiveAttribs();

	void bind();
	void unbind();
	void cleanUp();

	/*Binds */
	void bindAttribLocation(GLuint location, const char* name);
	void bindFragLocation(GLuint location, const char* name);
	
	bool setUniform(const char *name, bool value);
	bool setUniform(const char *name, float x);
	bool setUniform(const char *name, float x, float y);
	bool setUniform(const char *name, float x, float y, float z);
	bool setUniform(const char *name, const glm::vec2& vector);
	bool setUniform(const char *name, const glm::vec3& vector);
	bool setUniform(const char *name, const glm::vec4& vector);
	bool setUniform(const char *name, const bool transpose, const glm::mat3& mat3);
	bool setUniform(const char *name, const bool transpose, const glm::mat4& mat4);

private:
	std::vector<GLuint> m_shaderObjects;
	GLuint m_shaderProgram;

	bool m_linked;

	int _getUniformLocation(const char *name);
	bool _checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage);

};
//CONSTRUCTOR & DECONSTRUCTOR // possibly add other "Default methods here"
GLSL_ShaderProgram::GLSL_ShaderProgram() {
	m_shaderProgram = 0;
	m_linked = false;
	m_shaderProgram = glCreateProgram();

	if (0 == m_shaderProgram) {
		m_linked = false;
		std::cout << "Error creating program object.\n" << std::endl;
		exit(1);
	}
}

GLSL_ShaderProgram::~GLSL_ShaderProgram() {

}

//Getters and setters

bool GLSL_ShaderProgram::isLinked() {
	return m_linked;
}

GLuint GLSL_ShaderProgram::getShaderProgramHandle() {
	return m_shaderProgram;
}

std::string GLSL_ShaderProgram::getActiveAttribs() {
	GLint numAttribs;
	glGetProgramInterfaceiv(m_shaderProgram, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	std::string activeAttributesString = "";
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	activeAttributesString.append("Active Attributes\n");
	for (int i = 0; i < numAttribs; i++) {
		GLint results[3];
		glGetProgramResourceiv(m_shaderProgram, GL_PROGRAM_INPUT, i, 3, properties, 3, nullptr, results);
		
		GLint nameBufSize = results[0] + 1;
		char* name = new char[nameBufSize];
		glGetProgramResourceName(m_shaderProgram, GL_PROGRAM_INPUT, i, nameBufSize, nullptr, name);
		activeAttributesString.append(name, results[0]);
		activeAttributesString.append("\n");
		delete[] name;
	}

	return activeAttributesString;
}

std::string GLSL_ShaderProgram::getActiveUnifoms() {
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_shaderProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

	std::string activeUniformsString = "";
	activeUniformsString.append("Active Uniforms\n");
	for (int i = 0; i < numUniforms; i++) {
		GLint results[4];
		glGetProgramResourceiv(m_shaderProgram, GL_UNIFORM, i, 4, properties, 4, nullptr, results);

		if (results[3] != -1) {
			continue;
		}

		GLint nameBufSize = results[0] + 1;
		char* name = new char[nameBufSize];
		glGetProgramResourceName(m_shaderProgram, GL_UNIFORM, i, nameBufSize, nullptr, name);
		activeUniformsString.append(name, results[0]);
		activeUniformsString.append("\n");
		delete[] name;
	}

	return activeUniformsString;

}

//Public Functions

void GLSL_ShaderProgram::bind() {
	if (m_linked) {
		glUseProgram(m_shaderProgram);
	}
}

void GLSL_ShaderProgram::unbind() {
	glUseProgram(0);
}

void GLSL_ShaderProgram::cleanUp() {
	for (unsigned int i = 0; i < m_shaderObjects.size(); i++) {
		glDetachShader(m_shaderProgram,m_shaderObjects[i]);
		glDeleteShader(m_shaderObjects[i]);
	}
	glDeleteProgram(m_shaderProgram);
}

void GLSL_ShaderProgram::link() {
	GLint status;
	glLinkProgram(m_shaderProgram);
	glValidateProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		m_linked = false;
		std::cout << "Failed to link shader program!\n" << std::endl;
	}
	else {
		m_linked = true;
	}
	
}

void GLSL_ShaderProgram::compileShaderFromString(std::string sourceString, GLSL_ShaderType type) {
	
	GLuint shaderID = glCreateShader(type);
	if (shaderID == 0) {
		std::cerr << "Error: Shader creation failed" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = sourceString.c_str();
	shaderSourceStringLengths[0] = sourceString.length();

	glShaderSource(shaderID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shaderID);

	// if there is no shader error add shaderto shader objects
	if (!_checkShaderError(shaderID, GL_COMPILE_STATUS, false, "Error: Shader comilation failed: ")) {
		m_shaderObjects.push_back(shaderID);
		glAttachShader(m_shaderProgram, shaderID);
	}
}

void GLSL_ShaderProgram::compileShaderFromFile(std::string fileName, GLSL_ShaderType type) {
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
	GLuint shaderID = glCreateShader(type);
	if (shaderID == 0) {
		std::cerr << "Error: Shader creation failed" << std::endl;
	}


	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = shaderCode.c_str();
	shaderSourceStringLengths[0] = shaderCode.length();

	glShaderSource(shaderID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shaderID);

	// if there is no shader error add shaderto shader objects
	if (!_checkShaderError(shaderID, GL_COMPILE_STATUS, false, "Error: Shader comilation failed: ")) {
		m_shaderObjects.push_back(shaderID);
		glAttachShader(m_shaderProgram, shaderID);
	}
}

void GLSL_ShaderProgram::bindAttribLocation(GLuint attributeIndex, const char* name) {
	glBindAttribLocation(m_shaderProgram, attributeIndex, name);
}

void GLSL_ShaderProgram::bindFragLocation(GLuint attributeIndex, const char* name) {
	glBindFragDataLocation(m_shaderProgram, attributeIndex, name);
}
			//UNIFORM SETTERS//
bool GLSL_ShaderProgram::setUniform(const char *name, bool value) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		if (value) {
			glUniform1f(loc, 1.0f);
		}
		else {
			glUniform1f(loc, 0.0f);
		}
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, float x) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform1f(loc,x);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, float x, float y) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform2f(loc, x, y);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, float x, float y, float z) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform3f(loc, x, y , z);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, const glm::vec2& vector) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform2fv(loc, 1, &vector[0]);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, const glm::vec3& vector) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform3fv(loc, 1, &vector[0]);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, const glm::vec4& vector) {
	int loc;
	if ((loc = glGetAttribLocation(m_shaderProgram, name) >= 0)) {
		glUniform4fv(loc, 1 , &vector[0]);
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, const bool transpose, const glm::mat3& mat3) {
	GLuint loc = glGetUniformLocation(m_shaderProgram, name);
	if (loc>= 0) {
		if (transpose) {
			glUniformMatrix3fv(loc, 1, GL_TRUE, &mat3[0][0]);
		}
		else {
			glUniformMatrix3fv(loc, 1, GL_FALSE, &mat3[0][0]);
		}
		return true;
	}
	else {
		return false;
	}
}
bool GLSL_ShaderProgram::setUniform(const char *name, const bool transpose, const glm::mat4& mat4) {
	GLuint loc =  glGetUniformLocation(m_shaderProgram, name);
	if (loc >= 0) {
		if (transpose) {
			glUniformMatrix4fv(loc, 1, GL_TRUE, &mat4[0][0]);
		}
		else {
			glUniformMatrix4fv(loc, 1, GL_FALSE, &mat4[0][0]);			
		}
		return true;
	}
	else {
		return false;
	}
}

//Private Functions

int GLSL_ShaderProgram::_getUniformLocation(const char* name) {
	GLuint loc = glGetUniformLocation(m_shaderProgram, name);
	if (loc >= 0){
		return loc;
	}
	else {
		return -1;
	}
}

bool GLSL_ShaderProgram::_checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
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
		return true;
	}
	return false;
}