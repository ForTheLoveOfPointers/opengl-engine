#include <glad/glad.h>
#include <GLFW/glfw3.h>

// images
#include <stb_image/stb_image.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// standard
#include <stdexcept>
#include <iostream>
#include <string>
#include <format>

// custom
#include "shader.hpp"
#include "program.hpp"
#include "camera.hpp"
#include "./texture_loader/loader.hpp"
#include "./light_emitters/point-light.hpp"

#define WIDTH 1920
#define HEIGHT 1080

float delta = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* setupEnv() {
	if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("Failed to initialize glfw");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Engine", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw std::runtime_error("Failed to create a window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	return window;
}


void getInput(GLFWwindow* window, Camera* cam);

int main() {
	
	auto window = setupEnv();
	
	float vertices[] = {
		// positions          // texture coords  // normals
			0.5f,  0.5f, 0.0f,   1.0f, 1.0f,      0.0f, 0.0f, 1.0f,// top right
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f,      0.0f, 0.0f, 1.0f,// bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,      0.0f, 0.0f, 1.0f,// bottom left
		   -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,      0.0f, 0.0f, 1.0f // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Vertex texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// Vertex normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SHADER AND PROGRAM SETUP
	//////////////////////////
	Shader vertexShader{};
	vertexShader.read_shader("./shaders/basic_shader.vert");
	vertexShader.setup_shader(GL_VERTEX_SHADER);

	Shader fragmentShader{};
	fragmentShader.read_shader("./shaders/basic_shader.frag");
	fragmentShader.setup_shader(GL_FRAGMENT_SHADER);

	Program shaderProgram{};

	shaderProgram.setupProgram(vertexShader.shader_id, fragmentShader.shader_id);

	vertexShader.delete_shader();
	fragmentShader.delete_shader();


	// TEXTURE AND IMAGE LOADING
	////////////////////////////
	const char* image_path = "container2.jpg";
	const char* specular_path = "container2_specular.jpg";

	GLuint texture1 = load_texture(image_path, 0, shaderProgram, "texSample");
	shaderProgram.uniform1i("texSample", 0);
	
	GLuint texture2 = load_texture(specular_path, 1, shaderProgram, "specularSample");
	shaderProgram.uniform1i("specularSample", 1);
	// TRANSFORMS
	/////////////
	glm::vec3 translationVec = glm::vec3(0.2f, -0.2f, 0.0f);
	glm::mat4 transform = glm::mat4(1.0f); // Identity matrix
	transform = glm::translate(transform, translationVec);

	// CAMERA TRIHEDRON SETUP
	/////////////////////////
	
	Camera cam(
		glm::vec3(0.0f, 0.0f, 2.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	// MAIN LOOP
	////////////
	// TODO: CLEAN THE MAIN FUNCTION AND MAKE IT SMALLER
	float lastTime = (float)glfwGetTime();
	PointLight* lightArr[3] = { 0 };

	lightArr[0] = new PointLight( glm::vec3(0.2f, 0.5f, 0.5f) );
	lightArr[1] = new PointLight( glm::vec3(-0.2f, 0.5f, 0.5f) );
	lightArr[2] = new PointLight( glm::vec3(0.0f, 0.5f, -0.5f) );

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		getInput(window, &cam);

		shaderProgram.useProgram();
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		for (int i = 0; i < 3; i++) {
			std::string name = std::format("lightPositions[{}]", i);
			shaderProgram.uniform3f(name.c_str(), lightArr[i]->position);
		}
		// set uniforms
		shaderProgram.uniformMatrix4fv("model", model);
		shaderProgram.uniformMatrix4fv("projection", projection);
		shaderProgram.uniformMatrix4fv("view", cam.view);
		shaderProgram.uniform3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.uniform3f("cameraPos", cam.position);

		shaderProgram.uniform3f("material.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		shaderProgram.uniform3f("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shaderProgram.uniform3f("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		shaderProgram.uniform1f("material.shininess", 128.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		float currTime = (float)glfwGetTime();
		delta = currTime - lastTime;
		lastTime = currTime;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

// THIS CODE ABSOLUTELY SUCKS. REFACTOR THE BILLION 'if' STATEMENTS
void getInput(GLFWwindow* window, Camera* cam) {
	const float cameraSpeed = 1.5f * delta; // adjust accordingly
	bool cam_adjust = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else {
		// We don't take diagonal movement into account here (yet),
		// this means that we are, effectively, 42% faster when moving
		// in a diagonal line.
		float yaw_change = delta * 1.0f;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec4 helper_front = glm::vec4(cam->front, 1.0f);
			glm::vec4 helper_right = glm::vec4(cam->right, 1.0f);
			model = glm::rotate(model, -yaw_change, cam->up);
			
			cam->front = glm::vec3( model * helper_front );
			cam->right = glm::vec3( model * helper_right );
			cam_adjust = true;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec4 helper_front = glm::vec4(cam->front, 1.0f);
			glm::vec4 helper_right = glm::vec4(cam->right, 1.0f);
			model = glm::rotate(model, yaw_change, cam->up);

			cam->front = glm::vec3(model * helper_front);
			cam->right = glm::vec3(model * helper_right);
			cam_adjust = true;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cam->position -= cameraSpeed * cam->front;
			cam_adjust = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cam->position += cameraSpeed * cam->front;
			cam_adjust = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cam->position -= cam->right * cameraSpeed;
			cam_adjust = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cam->position += cam->right * cameraSpeed;
			cam_adjust = true;
		}
		
		if (cam_adjust) {
			cam->update_view();
		}
	}
	
}