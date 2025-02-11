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

// custom
#include "shader.hpp"
#include "program.hpp"
#include "camera.hpp"

#define WIDTH 840
#define HEIGHT 1024

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
		// positions          // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		   -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Vertex texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

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

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// TEXTURE AND IMAGE LOADING
	////////////////////////////
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int x, y, n_channels;
	unsigned char* image = stbi_load("C:/Users/unhap/OneDrive/Escritorio/C/Graphics/OpenGL_Engine/OpenGL_Engine/image.png", &x, &y, &n_channels, 0);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error loading image" << std::endl;
	}
	stbi_image_free(image);
	shaderProgram.useProgram();
	glUniform1i(glGetUniformLocation(fragmentShader.shader_id, "texSample"), 0);

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
	float delta = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		getInput(window, &cam);

		shaderProgram.useProgram();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		// retrieve the matrix uniform locations
		GLuint modelLoc = glGetUniformLocation(shaderProgram.program_id, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgram.program_id, "view");
		GLuint projectLoc = glGetUniformLocation(shaderProgram.program_id, "projection");
		// pass them to the shaders.
		// TODO: THIS SHOULD BE SETTERS INSIDE THE 'Program' CLASS
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.view));
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
	const float cameraSpeed = 0.01f; // adjust accordingly
	bool cam_adjust = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else {
		// We don't take diagonal movement into account here (yet),
		// this means that we are, effectively, 42% faster when moving
		// in a diagonal line.
		float yaw_change = glm::radians(0.1f);
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