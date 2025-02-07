#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Program {
public:
	GLuint program_id;
	void setupProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
	void useProgram();
};