#pragma once
#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Models.h"

class Cube : public Models
{
public:
	explicit Cube(glm::vec3 center = glm::vec3(0.f), glm::vec3 size = glm::vec3(1), bool _Textured = false, Texture* _Texture = nullptr, float mass = 0.5f, float restitution = 0.4f, bool movable = true);
	~Cube();

	void Draw(Shader *shaderProgram, float dt) override;

private:
	glm::vec3 cubeCenter;
};

#endif // !CUBE_CLASS_H