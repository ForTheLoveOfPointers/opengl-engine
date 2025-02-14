#pragma once
#include <glm/glm.hpp>

class PointLight {
public:
	glm::vec3 position;
	PointLight();
	PointLight(glm::vec3 pos);
	void setPosition(float x, float y, float z);
};