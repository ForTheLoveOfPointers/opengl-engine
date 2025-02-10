#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 rgt, glm::vec3 up) 
	: position(pos), target(tgt), right(rgt), up(up) {
	front = glm::normalize(pos - tgt);
	view = glm::lookAt(pos, tgt, up);
}

void Camera::update_view() {
	target = position -front;
	view = glm::lookAt(position, target, up);
}