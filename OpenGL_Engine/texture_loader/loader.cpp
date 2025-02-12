#include "loader.hpp"
#include <stb_image/stb_image.h>
#include <iostream>


GLuint load_texture(const char* file_path, GLuint texture_unit, Program &program, const char* uniform_name, GLenum channel) {
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int x, y, n_channels;
	unsigned char* image = stbi_load(file_path, &x, &y, &n_channels, 0);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, channel, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error loading image" << std::endl;
	}
	stbi_image_free(image);
	program.useProgram();
	glUniform1i(glGetUniformLocation(program.program_id, "texSample"), texture_unit);
	return texture;
}