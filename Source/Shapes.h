#pragma once
#include "glm\glm.hpp"

class AABB {
public:
	AABB();
	~AABB();
	
	//Lengths are in the x,y,z left/right, top/bottom, diagonal
	glm::vec3 getLengths();
	glm::vec3 getTopLCorner();
	glm::vec3 getBotRCorner();
private:
	glm::vec3 m_topLCorner;
	glm::vec3 m_botRCorner;
};
