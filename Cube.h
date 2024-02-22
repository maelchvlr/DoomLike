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

private:
	glm::vec3 cubeCenter;

	
};