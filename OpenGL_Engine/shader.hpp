#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLenum SUPPORTED_SHADERS[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

class Shader {
	
public:
	GLuint shader_id;
	bool setup_shader(GLenum shader_type);
	void read_shader(const char* file_path);
	void print_code();
	void delete_shader();
private:
	std::string shader_code;
};