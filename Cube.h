#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>


class Cube
{
public:
	explicit Cube(glm::vec3 center = glm::vec3(0.f));
	~Cube();

	void Draw(GLuint *shaderProgram, GLuint *VAO);

    GLuint indices[36] = {
        //Indices
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

    GLfloat vertices[36] = {
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

private:
	glm::vec3 cubeCenter;
};