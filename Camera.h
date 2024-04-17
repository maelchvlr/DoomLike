#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "RigidBody.h"
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

class Camera
{
public:
	RigidBody* rb;

	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

	int width;
	int height;

	bool flymode = false;
	float speed = 0.1f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position,GLuint *shaderProgram);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform, Shader* shaderProgram, float dt);
	void Inputs(GLFWwindow* window);

private:
	GLuint shader;
};

#endif // !CAMERA_CLASS_H