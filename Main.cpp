#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Terrain.h"
#include "Cube.h"
#include "Camera.h"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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

    // Create Assets
    Cube cube1 = Cube();
    Cube cube2 = Cube(glm::vec3(1.f));
    Terrain terrain1 = Terrain();

    // Load the shader program
    Shader shaderProgram = Shader("VertexShader.glsl", "FragmentShader.glsl");

    // Set up vertex buffer object and vertex array object
    VAO VAO1 = VAO();
    VBO VBOCube = VBO(cube1.vertices, sizeof(cube1.vertices));
    //VBO VBOTerrain = VBO(terrain1.vertices, sizeof(terrain1.vertices));
    
    VAO1.Bind();
    VAO1.LinkAttrib(VBOCube, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    //VAO1.LinkAttrib(VBOTerrain, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    // Generate and bind an Element Buffer Object
    EBO EBOCube = EBO(cube1.indices, sizeof(cube1.indices));
    //EBO EBOTerrain = EBO(terrain1.indices, sizeof(terrain1.indices));

    VAO1.Unbind();
    VBOCube.Unbind();
    //VBOTerrain.Unbind();

    EBOCube.Unbind();
    //EBOTerrain.Unbind();

    // Create the camera
    Camera camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), &shaderProgram.ID);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render clearing
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1.0f);

		// Use the shader program
		shaderProgram.Activate();

        // Create transformations
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Camera/view transformation
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        // Get the uniform locations
        GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the cube(s)
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 1.0f, 0.0f);
        cube1.Draw(&shaderProgram.ID, &VAO1.ID);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 0.0f, 0.0f);
        cube2.Draw(&shaderProgram.ID, &VAO1.ID);

        // Camera
        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, "camMatrix");

		glfwSwapBuffers(window);    // Swap front and back buffers
		glfwPollEvents();	        // Poll for and process events

        // Close the window if the escape key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}   

    // De-allocate resources
    VAO1.Delete();
    VBOCube.Delete();
    //VBOTerrain.Delete();
    EBOCube.Delete();
    shaderProgram.Delete();

    // Terminate GLFW
    glfwTerminate();

	return 0;
}