#pragma once

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& rot = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f,1.0f,1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}
	~Transform() {};

	static inline glm::mat4 GetTransformationMatirx(glm::vec3 position, float rx,float ry, float rz, glm::vec3 scale) {
		glm::mat4 posMatrix = glm::translate(position);
		glm::mat4 rotXMatrix = glm::rotate(rx, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix = glm::rotate(ry, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix = glm::rotate(rz, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(scale);

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
		
		return posMatrix * rotMatrix * scaleMatrix;
	}

	inline glm::vec3& GetPos() { return m_pos; };
	inline glm::vec3& GetRot() { return m_rot; };
	inline glm::vec3& GetScale() { return m_scale; };

	inline void SetPos(const glm::vec3& pos) { m_pos = pos; };
	inline void SetRot(const glm::vec3& rot) { m_rot = rot; };
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; };
protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

