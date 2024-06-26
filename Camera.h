#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
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
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	bool firstClick = true;

	int width;
	int height;

	bool flymode = false;
	float speed = 0.1f;
	float sensitivity = 60.0f;

	Camera(int width, int height, glm::vec3 position,GLuint *shaderProgram);

	void Matrix(const char* uniform, Shader* shaderProgram, float dt);
	void Inputs(GLFWwindow* window);

	std::vector<glm::vec3> RayCast(GLFWwindow* window);

private:
	GLuint shader;
	bool gameEntered = false;

	float FOVdeg;
	float nearPlane;
	float farPlane;
};

#endif // !CAMERA_CLASS_H