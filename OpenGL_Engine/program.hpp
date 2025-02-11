#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Program {
public:
	GLuint program_id;
	void setupProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
	void useProgram();
	void uniform3f(const char* uniform_name, glm::vec3 vec);
	void uniformMatrix4fv(const char* uniform_name, glm::mat4& mat);
};