#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Loads vertex and fragment shaders, compiles them, and links them into a shader program
GLuint loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	// Open and read the shader files
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	vShaderFile.close();
	fShaderFile.close();
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	// Convert shader source strings into character arrays
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Link shaders into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	// Delete shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shaderProgram; // Return the shader program ID
}

int main()
{
	// Initialize GLFW
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(640, 480, "doom like", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		return -1;

	// Initialize OpenGL
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Bottom-left
		0.5f, -0.5f, 0.0f, // Bottom-right
		0.0f, 0.5f, 0.0f // Top-center
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	// Load and compile shaders
	GLuint shaderProgram = loadShader("vertexShader.glsl", "fragmentShader.glsl");

	// Check if the shader program compiled and linked successfully
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // Unbind VAO


		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
