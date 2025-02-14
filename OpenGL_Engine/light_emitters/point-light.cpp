#include "point-light.hpp"

PointLight::PointLight() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}

PointLight::PointLight(glm::vec3 pos) : position(pos) {}

void PointLight::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}