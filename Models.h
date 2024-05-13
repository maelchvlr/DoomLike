#pragma once
#ifndef MODELS_H
#define MODELS_H

#include "Shader.h"
#include "RigidBody.h"
#include "Texture.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <fstream>
#include <iostream>

class Models
{
public:
	
	Models(float mass, float restitution, bool movable, bool _Textured = false, Texture* _Texture = nullptr, glm::vec3 center = glm::vec3(0), glm::vec3 _size = glm::vec3(1));
	~Models();

	void init(GLfloat* vertices, int sizeVer, GLuint indices[], int sizeInd);
	virtual void Draw(Shader* shaderProgram, float dt);
	virtual RigidBody* getRigidBody() { return &rb; }
	//virtual void render(Shader shader, float dt);

protected:
	// OpenGL IDs for vertex array, vertex buffer, and element buffer objects
	VAO VAOModel;
	VBO VBOModel;
	EBO EBOModel;

	//rigid body
	RigidBody rb;

	// Texture for the model
	bool textured;
	Texture* texture;
	glm::vec3 size;
};

#endif // !MODELS_H