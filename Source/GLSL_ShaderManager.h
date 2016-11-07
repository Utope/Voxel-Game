#pragma once
#include <map>
#include "GLSL_ShaderProgram.h"

class GLSL_ShaderManager {
public:
	GLSL_ShaderManager() {};
	~GLSL_ShaderManager();
	bool addShader(std::string shaderName, std::string vertexFileName, std::string fragmentFileName);
	bool addShader(std::string shaderName, GLSL_ShaderProgram* shader);
	
	void removeShader(std::string shaderName);
	GLSL_ShaderProgram* getShader(std::string shaderName);

private:
	std::map<std::string, GLSL_ShaderProgram*> m_shaders;
};

GLSL_ShaderManager::~GLSL_ShaderManager() {
	for (std::map<std::string, GLSL_ShaderProgram*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it){
		delete it->second;
	}
	m_shaders.clear();
}

bool GLSL_ShaderManager::addShader(std::string shaderName, std::string vertexFileName, std::string fragmentFileName) {
	if (m_shaders.count(shaderName) == 0) {
		m_shaders.insert(std::pair<std::string, GLSL_ShaderProgram*>(shaderName, new GLSL_ShaderProgram));
		m_shaders.at(shaderName)->compileShaderFromString(vertexFileName,GLSL_ShaderType::VERTEX);
		m_shaders.at(shaderName)->compileShaderFromString(vertexFileName, GLSL_ShaderType::FRAGMENT);
		m_shaders.at(shaderName)->link();
		return true;
	}
	return false;
}

bool GLSL_ShaderManager::addShader(std::string shaderName, GLSL_ShaderProgram* shader) {
	if (m_shaders.count(shaderName) == 0) {
		m_shaders.insert(std::pair<std::string, GLSL_ShaderProgram*>(shaderName, shader));
		return true;
	}
	return false;
}

GLSL_ShaderProgram* GLSL_ShaderManager::getShader(std::string shaderName) {
	if (m_shaders.count(shaderName) != 0) {
		return m_shaders.at(shaderName);
	}
	else {
		return nullptr;
	}
}

void GLSL_ShaderManager::removeShader(std::string shaderName) {
	if (m_shaders.count(shaderName) != 0) {
		delete m_shaders.at(shaderName);
		m_shaders.erase(shaderName);
	}
}