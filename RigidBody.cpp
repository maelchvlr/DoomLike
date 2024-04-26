#include "RigidBody.h"

#include <iostream>

RigidBody::RigidBody(float mass, float restitution, bool movable, glm::vec3 size, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, bool simulate)
	: mass(mass), size(size), movable(movable), position(position), velocity(velocity), acceleration(acceleration), restitution(restitution) , simulate(simulate)
{}

RigidBody::~RigidBody() {}

//Formule de la position : x(t) = x0 + v0*t + 1/2*a*t^2
void RigidBody::update(float dt) {
	if(!movable || !simulate) return;
	acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
	position = position + velocity * dt;
	velocity += acceleration * dt;
}

void RigidBody::dampen() {
	const float stopThreshold = 0.1f; // Threshold for when to completely stop the object

	if (abs(glm::length(velocity)) < stopThreshold) {
		velocity = glm::vec3(0.0f);
		stopSimulate();
	}
}

// Utility function to stop the object when its momentum nears zero
void RigidBody::stopSimulate() {
	simulate = false;
}

// Utility function to start the object momentum
void RigidBody::startSimulate() {
	simulate = true;
}
