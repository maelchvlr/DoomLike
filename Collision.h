#pragma once
#include "Models.h"
#include "glm.hpp"
#include <iostream>

// Utility to calculate collision normal based on predictive positions
glm::vec3 calculateCollisionNormal(RigidBody& rb1, RigidBody& rb2) {
	return glm::vec3(0, -1, 0);
}

// Checks if two models will collide based on their current velocities and positions
bool willCollide(RigidBody& rb1, RigidBody& rb2, float deltaTime) {
    // Calculate the future positions of the two models
    glm::vec3 futurePos1 = rb1.position + rb1.velocity * deltaTime;
    glm::vec3 futurePos2 = rb2.position + rb2.velocity * deltaTime;

    //Re define the min and max of the two models
    glm::vec3 rb1_min = futurePos1 - rb1.size / 2.0f;
    glm::vec3 rb1_max = futurePos1 + rb1.size / 2.0f;
    glm::vec3 rb2_min = futurePos2 - rb2.size / 2.0f;
    glm::vec3 rb2_max = futurePos2 + rb2.size / 2.0f;

    // Check for overlap in all axes
    bool collide = (rb1_min.x <= rb2_max.x && rb1_max.x >= rb2_min.x) &&
        (rb1_min.y <= rb2_max.y && rb1_max.y >= rb2_min.y) &&
        (rb1_min.z <= rb2_max.z && rb1_max.z >= rb2_min.z);

    // Return the result
    return collide;
}

// Applies an impulse to models based on predicted collision, adjusting their velocities to prevent clipping
void applyImpulse(RigidBody& rb1, RigidBody& rb2, const glm::vec3& collisionNormal) {
    // Calculate the relative velocity of the two models
    glm::vec3 relativeVelocity = abs(rb2.velocity - rb1.velocity);
    //dot product : x1*x2 + y1*y2 + z1*z2
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    //Debug purpose
    //std::cout << "relative velocity: " << relativeVelocity.x << " | " << relativeVelocity.y << " | " << relativeVelocity.z << std::endl;
    //std::cout << "velocity along normal: " << velocityAlongNormal << std::endl;

    // They are moving apart, so no impulse is necessary
    if (velocityAlongNormal > 0) return; 

    // Restitution depends of the bounciness of the object
    float restitution = std::min(rb1.restitution, rb2.restitution);
    float j = -(1 + restitution) * velocityAlongNormal;
    // apply the mass on the object to prevent it to just bounce to the infinity
    j /= rb1.getInverseMass() + rb2.getInverseMass();

    glm::vec3 impulse = j * collisionNormal;
    
    // Apply the impulse to the two models to make them bounce
    rb1.velocity -= rb1.getInverseMass() * impulse;
    rb2.velocity += rb2.getInverseMass() * impulse;

    //Verify if the object is moving behind a certain threshold
    rb1.dampen();
    rb2.dampen();
}

// Handles collision detection and response using predictive logic
void handlePredictiveCollision(Models& model1, Models& model2, float deltaTime, std::string tag) {
    RigidBody* rb1 = model1.getRigidBody();
    RigidBody* rb2 = model2.getRigidBody();

    if (willCollide(*rb1, *rb2, deltaTime)) {
        //std::cout << "collision detected between " << tag << std::endl;

        //If the two models will collide
        rb1->startSimulate();
        rb2->startSimulate();
        glm::vec3 collisionNormal = calculateCollisionNormal(*rb1, *rb2);
        applyImpulse(*rb1, *rb2, collisionNormal);
    }
}

void handleCameraCollision(RigidBody& camera, Models& model, float deltaTime, std::string tag) {
    RigidBody* rbModel = model.getRigidBody();
    if (willCollide(camera, *rbModel, deltaTime)) {
        model.getRigidBody()->startSimulate();
        std::cout << "collision detected between " << tag << std::endl;
        glm::vec3 collisionNormal = calculateCollisionNormal(camera, *rbModel);
        applyImpulse(camera, *rbModel, collisionNormal);
    }
}