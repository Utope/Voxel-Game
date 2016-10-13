#pragma once
#include "RawModel.h"
#include "ModelTexture.h"

class TexturedModel {
public:
	TexturedModel() {};
	TexturedModel(RawModel rawModel, ModelTexture modelTexture) {
		this->m_rawModel = rawModel;
		this->m_modelTexture = modelTexture;
	};
	
	ModelTexture& getModelTexture();
	RawModel& getRawModel();

	~TexturedModel() {};
private:
	ModelTexture m_modelTexture;
	RawModel m_rawModel;
};

ModelTexture& TexturedModel::getModelTexture() {
	return this->m_modelTexture;
}

RawModel& TexturedModel::getRawModel() {
	return this->m_rawModel;
}