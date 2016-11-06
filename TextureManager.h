#pragma once
#include <map>
#include "GL\glew.h"
#include "Texture.h"

class TextureManager {
public:
	TextureManager();
	~TextureManager();
	Texture* loadTexture(std::string fileName);
	Texture* getTexture(std::string textureName);
	void deleteTexture(std::string textureName);
private:
	std::map<std::string, Texture*> m_textures;
	Texture m_DEFAULT;
};

TextureManager::TextureManager() {
	m_DEFAULT.setTextureName("DEFAULT");
}

TextureManager::~TextureManager() {
	std::map<std::string, Texture*>::iterator it;
	for (it = m_textures.begin(); it != m_textures.end(); ++it) {
		delete it->second;
	}
}

Texture* TextureManager::loadTexture(std::string fileName) {
	Texture* texture = new Texture(fileName);
	if (texture->load()) {
		m_textures.insert(std::pair<std::string,Texture*>(texture->getTextureName(),texture));
		return texture;
	}
	return nullptr;
}

Texture* TextureManager::getTexture(std::string textureName) {
	std::map<std::string, Texture*>::iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}
	return &m_DEFAULT;
}

void TextureManager::deleteTexture(std::string textureName) {
	std::map<std::string, Texture*>::iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		delete it->second;
		m_textures.erase(it);
	}
}