#include "program.hpp"
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

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

void Program::uniform3f(const char* uniform_name, glm::vec3 vec) {
	GLuint location = glGetUniformLocation(program_id, uniform_name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void Program::uniformMatrix4fv(const char* uniform_name, glm::mat4& mat) {
	GLuint location = glGetUniformLocation(program_id, uniform_name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}