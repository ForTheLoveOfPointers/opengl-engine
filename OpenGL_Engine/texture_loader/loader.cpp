#include "loader.hpp"
#include <stb_image/stb_image.h>
#include <iostream>


GLuint load_texture(const char* file_path, GLuint texture_unit, Program &program, const char* uniform_name) {
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
		GLenum channel = GL_RGB;
		if (n_channels == 1)
			channel = GL_RED;
		else if (n_channels == 3)
			channel = GL_RGB;
		else if (n_channels == 4)
			channel = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0, channel, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Error loading image" << std::endl;
	}
	stbi_image_free(image);
	return texture;
}