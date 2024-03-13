#include "RigidBody.h"

RigidBody::RigidBody(float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration) 
	: mass(mass), position(position), velocity(velocity), acceleration(acceleration) {}

RigidBody::~RigidBody() {}

//Formule de la position : x(t) = x0 + v0*t + 1/2*a*t^2
void RigidBody::update(float dt) {
	acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
	position = position + velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
}