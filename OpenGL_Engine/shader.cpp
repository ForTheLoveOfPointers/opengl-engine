#include "shader.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

void Shader::print_code() {
	std::cout << shader_code << "\r\n";
}

bool Shader::setup_shader(GLenum shader_type) {
	const char* code = shader_code.c_str();
	
	bool supported = false;
	for (auto type : SUPPORTED_SHADERS) {
		if (shader_type == type) {
			supported = true;
		}
	}
	
	if (!supported) {
		return false;
	}

	GLuint shader_ = glCreateShader(shader_type);
	glShaderSource(shader_, 1, &code, NULL);
	glCompileShader(shader_);
	shader_id = shader_;
	return true;
}


void Shader::read_shader(const char* file_path) {
	std::ifstream file;
	std::stringstream shader;
	std::string shader_code;
	try
	{
		file.open(file_path);
		shader << file.rdbuf();
		file.close();
		shader_code = shader.str();
	}
	catch (const std::exception& except)
	{
		throw std::runtime_error(except.what());
	}
	this->shader_code = shader_code;
}

void Shader::delete_shader() {
	glDeleteShader(shader_id);
}