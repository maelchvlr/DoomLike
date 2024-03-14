#pragma once
#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Models.h"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

class Cube : Models
{
public:
	explicit Cube(glm::vec3 center = glm::vec3(0.f), bool _Textured = false, Texture* _Texture = nullptr, float mass = 0.5f);
	~Cube();

	void Draw(Shader *shaderProgram, float dt) override;

private:
	glm::vec3 cubeCenter;
};

#endif // !CUBE_CLASS_H