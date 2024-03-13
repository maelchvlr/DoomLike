#pragma once
#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Shader.h"
#include "RigidBody.h"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

class Cube
{
public:
	explicit Cube(RigidBody rb, glm::vec3 center = glm::vec3(0.f));
	~Cube();

	void Draw(Shader *shaderProgram, float dt);

private:
    // OpenGL IDs for vertex array, vertex buffer, and element buffer objects
    VAO VAOCube;
    VBO VBOCube;
    EBO EBOCube;

	RigidBody rbCube;

	glm::vec3 cubeCenter;
};

#endif // !CUBE_CLASS_H