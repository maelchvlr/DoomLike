#pragma once
#include "Models.h"
#include "glm.hpp"
#include <iostream>

struct CollisionData {
    bool CollisionDetected;
    glm::vec3 collisionNormal = glm::vec3(0);
};

// Utility to calculate collision normal based on predictive positions
glm::vec3 calculateCollisionNormal(RigidBody& rb1, RigidBody& rb2) {
	return glm::vec3(0, -1, 0);
}

// Checks if two models will collide based on their current velocities and positions
CollisionData willCollide(RigidBody& rb1, RigidBody& rb2, float deltaTime) {

    CollisionData collide;

    // Calculate the future positions of the two models
    glm::vec3 futurePos1 = rb1.position + rb1.velocity * deltaTime;
    glm::vec3 futurePos2 = rb2.position + rb2.velocity * deltaTime;

    //Re define the min and max of the two future models
    glm::vec3 frb1_min = futurePos1 - rb1.size / 2.0f;
    glm::vec3 frb1_max = futurePos1 + rb1.size / 2.0f;
    glm::vec3 frb2_min = futurePos2 - rb2.size / 2.0f;
    glm::vec3 frb2_max = futurePos2 + rb2.size / 2.0f;

    // define the min and max of the current models
    glm::vec3 rb1_min = rb1.position - rb1.size / 2.0f;
    glm::vec3 rb1_max = rb1.position + rb1.size / 2.0f;
    glm::vec3 rb2_min = rb2.position - rb2.size / 2.0f;
    glm::vec3 rb2_max = rb2.position + rb2.size / 2.0f;


    // Check for overlap in all axes
    bool x = frb1_min.x <= frb2_max.x && frb1_max.x >= frb2_min.x;
    bool y = frb1_min.y <= frb2_max.y && frb1_max.y >= frb2_min.y;
    bool z = frb1_min.z <= frb2_max.z && frb1_max.z >= frb2_min.z;

    bool Collide = x && y && z;

    collide.CollisionDetected = Collide;

    if (Collide) {
        x = rb1_min.x <= rb2_max.x && rb1_max.x >= rb2_min.x;
        y = rb1_min.y <= rb2_max.y && rb1_max.y >= rb2_min.y;
        z = rb1_min.z <= rb2_max.z && rb1_max.z >= rb2_min.z;

        if (!x) {
			collide.collisionNormal.x = glm::normalize(rb1.position - rb2.position).x;
        }
        else if (!y) {
            collide.collisionNormal.y = glm::normalize(rb1.position - rb2.position).y;
        }
        else if (!z) {
            collide.collisionNormal.z = glm::normalize(rb1.position - rb2.position).z;
		}
    }

    // Return the result
    return collide;
}

// Applies an impulse to models based on predicted collision, adjusting their velocities to prevent clipping
void applyImpulse(RigidBody& rb1, RigidBody& rb2, const glm::vec3& collisionNormal) {
    // Calculate the relative velocity of the two models
    glm::vec3 relativeVelocity = abs(rb2.velocity - rb1.velocity);

    //dot product : x1*x2 + y1*y2 + z1*z2
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    // They are moving apart, so no impulse is necessary
    if (velocityAlongNormal < 0) return;

    // Restitution depends of the bounciness of the object
    float restitution = std::min(rb1.restitution, rb2.restitution);
    float j = -(1 + restitution) * velocityAlongNormal;
    // apply the mass on the object to prevent it to just bounce to the infinity
    j /= rb1.getInverseMass() + rb2.getInverseMass();

    glm::vec3 impulse = j * collisionNormal;

    std::cout << "velocity 1: " << rb1.velocity.x << " " << rb1.velocity.y << " " << rb1.velocity.z << std::endl;
    // Apply the impulse to the two models to make them bounce
    rb1.velocity = -rb1.velocity * impulse;
    rb2.velocity = -rb2.velocity * impulse;
    std::cout << "velocity 1 after: " << rb1.velocity.x << " " << rb1.velocity.y << " " << rb1.velocity.z << std::endl;

    //Verify if the object is moving behind a certain threshold
    rb1.dampen();
    rb2.dampen();
}

// Handles collision detection and response using predictive logic
void handlePredictiveCollision(RigidBody* rb1, RigidBody* rb2, float deltaTime, std::string tag) {

    CollisionData collisionData = willCollide(*rb1, *rb2, deltaTime);

    if (collisionData.CollisionDetected) {
        //If the two models will collide
        rb1->startSimulate();
        rb2->startSimulate();
        applyImpulse(*rb1, *rb2, collisionData.collisionNormal);
    }
}

void handleHorizontalCollision(RigidBody* rb1, RigidBody* rb2, float deltaTime, std::string tag) {

}
