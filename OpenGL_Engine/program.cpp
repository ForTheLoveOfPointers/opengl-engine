#include "program.hpp"
#include <stdexcept>

void Program::setupProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
	int  success;
	char infoLog[512] = { 0 };

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader_id);
	glAttachShader(program, fragment_shader_id);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		throw std::exception(infoLog);
	}

	program_id = program;
}

void Program::useProgram() {
	glUseProgram(program_id);
}