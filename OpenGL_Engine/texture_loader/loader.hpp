#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../program.hpp"

GLuint load_texture(const char* file_path, GLuint texture_unit, Program& program, const char* uniform_name);