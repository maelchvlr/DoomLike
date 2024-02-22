#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Cube.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Camera.h"

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

    //debug
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
    else
    {
        std::cout << "Shader program linked" << std::endl;
    }

    return shaderProgram; // Return the shader program ID
}

void setupCubeRenders(GLuint *VAO, GLuint *VBO, GLuint *EBO) {
    GLfloat vertices[36] =
    {
        //Position
        -0.5f, -0.5f, -0.5f,
         0.0f, -0.5f, -0.5f,
        -0.5f,  0.0f, -0.5f,
         0.0f,  0.0f, -0.5f,
        -0.5f, -0.5f,  0.0f,
         0.0f, -0.5f,  0.0f,
        -0.5f,  0.0f,  0.0f,
         0.0f,  0.0f,  0.0f,
    };

    GLint indices[36] = {
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7,
        0, 1, 4,
        1, 4, 5,
        2, 3, 6,
        3, 6, 7,
        0, 2, 4,
        2, 4, 6,
        1, 3, 5,
        3, 5, 7
    };

    // Generate and bind a Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    // Generate and bind an Element Buffer Object (EBO)
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Enable vertex attribute pointers for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

const unsigned int width = 800;
const unsigned int height = 800;

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
    else
		std::cout << "GLFW initialized" << std::endl;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "DoomLike", NULL, NULL);
    if (!window) {
		glfwTerminate();
		return -1;
	}

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize GLEW" << std::endl;
    else
        std::cout << "GLEW initialized" << std::endl;

    // Load the shader program
    GLuint shaderProgram = loadShader("VertexShader.glsl", "FragmentShader.glsl");

    // Create the camera
    Camera camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), &shaderProgram);

    // Create the cubes
    Cube cube1 = Cube();
    Cube cube2 = Cube(glm::vec3(1.f));

    // Set up vertex buffer object and vertex array object
    GLuint VBO, VAO, EBO;

    setupCubeRenders(&VAO, &VBO, &EBO);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
		// Render here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1.0f);

		// Use the shader program
		glUseProgram(shaderProgram);

        // Create transformations
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Camera/view transformation
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // Get the uniform locations
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the cube
        glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 1.0f, 0.0f);
        cube1.Draw(&shaderProgram, &VAO);

        glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 0.0f, 0.0f);
        cube2.Draw(&shaderProgram, &VAO);

        
        // Camera
        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, "camMatrix");


		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

        // Close the window if the escape key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}   

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Terminate GLFW
    glfwTerminate();

	return 0;

}