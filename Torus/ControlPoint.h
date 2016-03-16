#pragma once

#include "Utility.h"
class ControlPoint
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	float distance;
	ControlPoint();
	~ControlPoint();

};

