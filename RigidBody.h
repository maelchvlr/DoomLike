#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "glm.hpp"

class RigidBody {
public:
	float mass;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 size;

	// restitution is the bounciness of the object (0 = no bounce, 1 = perfect bounce)
	float restitution;
	// movable is a flag to determine if the object is static or not
	bool movable;
	// simulate is a flag to determine if the object should be updated
	bool simulate;

	RigidBody(float mass, float restitution, bool movable, glm::vec3 size, glm::vec3 position = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), bool simulate = true);
	~RigidBody();

	void update(float dt);
	void dampen();
	void stopSimulate();
	void startSimulate();
	
	

	//determine if the object is static or not
	bool isMovable() { return movable; }
	float getInverseMass() { return !movable ? 0.0f : 1.0f / mass; }
};

#endif