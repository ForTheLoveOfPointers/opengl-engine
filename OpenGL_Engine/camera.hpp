#pragma once
#include <glm/glm.hpp>

class Camera {

	glm::vec3 target;
	
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 view;
	float yaw = 0.0f;

	Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 rgt, glm::vec3 up);

	void update_view();
};