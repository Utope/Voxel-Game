#pragma once
#include "TexturedModel.h"
#include "Transform.h"

class Entity {
public:
	Entity(TexturedModel texturedModel,glm::vec3 position, float rx, float ry, float rz, glm::vec3 scale) {
		this->m_texturedModel = texturedModel;
		this->m_position = position;
		this->rotX = rx;
		this->rotY = ry;
		this->rotZ = rz;
		this->scale = scale;
	}

	Entity(TexturedModel texturedModel) {
		this->m_texturedModel = texturedModel;
		this->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotX = 0.0f;
		this->rotY = 0.0f;
		this->rotZ = 0.0f;
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	
	}

	TexturedModel getTexturedModel() {
		return m_texturedModel;
	}

	void increasePosition(float dx, float dy, float dz) {
		this->m_position.x += dx;
		this->m_position.y += dy;
		this->m_position.z += dz;
	}

	void increaseRotation(float rx, float ry, float rz) {
		this->rotX += rx;
		this->rotY += ry;
		this->rotZ += rz;
	}

	float getRotX() {
		return this->rotX;
	}

	float getRotY() {
		return this->rotY;
	}
	
	float getRotZ() {
		return this->rotZ;
	}

	glm::vec3 getPosition() {
		return this->m_position;
	}

	glm::vec3 getScale() {
		return this->scale;
	}

	glm::mat4 getModelMatrix() {
		glm::mat4 posMatrix = glm::translate(m_position);
		glm::mat4 rotXMatrix = glm::rotate(rotX, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix = glm::rotate(rotY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix = glm::rotate(rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(scale);

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}


	~Entity() {};
private:
	TexturedModel m_texturedModel;
	glm::vec3 m_position;
	float rotX, rotY, rotZ;
	glm::vec3 scale;

};
