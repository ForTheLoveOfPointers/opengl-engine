#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tex_coords;
};

struct Texture {
	GLuint id;
	std::string type;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};