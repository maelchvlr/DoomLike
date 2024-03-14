#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "Terrain.h"
#include "Texture.h"
#include "Models.h"
#include "Cube.h"
#include "Camera.h"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Collision.h"

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

    // Create textures
    Texture dirtTex = Texture("ressources/textures/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture crateTex = Texture("ressources/textures/crate.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    Cube cube1 = Cube(glm::vec3(1.4, 4, 1), glm::vec3(1), true, &crateTex, 0.1f, 1);
    Cube cube2 = Cube(glm::vec3(1,4,1), glm::vec3(1), true, &crateTex, 0.5f);

    Cube cube3 = Cube(glm::vec3(3, 4, 3), glm::vec3(1), true, &crateTex, 2.f, 0);

    Cube cube4 = Cube(glm::vec3(3, 4, 1), glm::vec3(1), true, &crateTex, 0.05f);


    Terrain terrain1 = Terrain(glm::vec3(0, -4, 0), glm::vec3(6, 0, 6), false, nullptr, 50.0f, 0.5);

    // Load the shader program
    Shader shaderProgram = Shader("VertexShader.glsl", "FragmentShader.glsl");

    // Create the camera
    Camera camera = Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), &shaderProgram.ID);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    double lastFrame = 0;
    double deltaTime = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        //Delta time = time between two frames
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

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
        cube1.Draw(&shaderProgram, deltaTime);

        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 0.0f, 0.0f);
        cube2.Draw(&shaderProgram, deltaTime);

        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 0.0f, 1.0f);
        cube3.Draw(&shaderProgram, deltaTime);

        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 1.0f, 1.0f);
        cube4.Draw(&shaderProgram, deltaTime);


        // Draw the terrain(s)
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 0.0f, 1.0f);
        dirtTex.texUnit(shaderProgram, "tex0", 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 1);
        dirtTex.Bind();
        terrain1.Draw(&shaderProgram, deltaTime);
        dirtTex.Unbind();
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 0);

        // handle camera collisions
        handleCameraCollision(camera.rb, terrain1, deltaTime, "camera terrain");

        //handleCollisions
        handlePredictiveCollision(cube2, terrain1, deltaTime, "cube2 terrain");
        handlePredictiveCollision(cube1, terrain1, deltaTime, "cube1 terrain");
        handlePredictiveCollision(cube1, cube2, deltaTime, "cube1 cube2");
        handlePredictiveCollision(cube3, terrain1, deltaTime, "cube3 terrain1");
        handlePredictiveCollision(cube4, terrain1, deltaTime, "cube4 terrain1");

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
    shaderProgram.Delete();
    cube1.~Cube();
    cube2.~Cube();
    cube3.~Cube();
    cube4.~Cube();
    terrain1.~Terrain();

    // Terminate GLFW
    glfwTerminate();

	return 0;
}