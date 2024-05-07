#pragma once
#include "Models.h"
#include "glm.hpp"
#include <iostream>

struct CollisionData {
    bool CollisionDetected = false;
    glm::vec3 collisionNormal = glm::vec3(0);
};

// Utility to calculate collision normal based on predictive positions
glm::vec3 calculateCollisionNormal(RigidBody& rb1, RigidBody& rb2) {
	return glm::vec3(0, -1, 0);
}

// Checks if two models will collide based on their current velocities and positions
CollisionData& willCollide(RigidBody& rb1, RigidBody& rb2, float deltaTime) {

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

    bool futurCollision = x && y && z;

    if (futurCollision) {
        x = rb1_min.x <= rb2_max.x && rb1_max.x >= rb2_min.x;
        y = rb1_min.y <= rb2_max.y && rb1_max.y >= rb2_min.y;
        z = rb1_min.z <= rb2_max.z && rb1_max.z >= rb2_min.z;

        //std::cout << "Collide : " << x << " | " << y << " | " << z << std::endl;

        if (!x) {

            //Variables pour la surface de collision
            int surface_collider_z_min = 0;
            int surface_collider_z_max = 0;
            int surface_collider_y_collision = 0;
            int surface_collider_y_min = 0;

            //Formatage des valeurs pour éviter les valeurs négatives
            rb2_max.z = abs(rb2_max.z);
            rb2_min.z = abs(rb2_min.z);
            rb1_max.z = abs(rb1_max.z);
            rb1_min.z = abs(rb1_min.z);

            //On récupère notre largeur de surface de collision
            if (rb2_min.z > rb1_min.z && rb2_max.z < rb1_max.z) {
                surface_collider_z_min = rb2_min.z;
                surface_collider_z_max = rb2_max.z;
            }
            else if (rb2_min.z < rb1_min.z && rb2_max.z < rb1_max.z) {
                surface_collider_z_min = rb1_min.z;
                surface_collider_z_max = rb2_max.z;
            }
            else if (rb2_min.z > rb1_min.z && rb2_max.z > rb1_max.z) {
                surface_collider_z_min = rb2_min.z;
                surface_collider_z_max = rb1_max.z;
            }
            else if (rb2_min.z < rb1_min.z && rb2_max.z > rb1_max.z) {
				surface_collider_z_min = rb1_min.z;
				surface_collider_z_max = rb1_max.z;
			}

            //On récupère la hauteur de cette surface
            if (rb1_min.y < rb2_min.y) {
                surface_collider_y_min = rb1_min.y;
                surface_collider_y_collision = rb2_min.y;
            }
            else {
                surface_collider_y_min = rb2_min.y;
                surface_collider_y_collision = rb1_min.y;
            }

            //Vérification de la Surface de collision
            int CollisionSurface = surface_collider_z_max - surface_collider_z_min * (surface_collider_y_min - surface_collider_y_collision);

            std::cout << "Surface de collision : " << CollisionSurface << std::endl;

            if (CollisionSurface > 5.0f) {
                collide.collisionNormal.x = glm::normalize(rb1.position - rb2.position).x;
                collide.CollisionDetected = true;
            }			
            std::cout << "Collision normal in X axis : " << collide.collisionNormal.x << std::endl;

            std::cout << "Future rigidbody 1 min : " << frb1_min.x << " | " << frb1_min.y << " | " << frb1_min.z << std::endl;
            std::cout << "Future rigidbody 1 max : " << frb1_max.x << " | " << frb1_max.y << " | " << frb1_max.z << std::endl;

            std::cout << "Future rigidbody 2 min : " << frb2_min.x << " | " << frb2_min.y << " | " << frb2_min.z << std::endl;
            std::cout << "Future rigidbody 2 max : " << frb2_max.x << " | " << frb2_max.y << " | " << frb2_max.z << std::endl;

            std::cout << "                        " << std::endl;

            std::cout << "Future Camera position : " << futurePos1.x << " | " << futurePos1.y << " | " << futurePos1.z << std::endl;
            std::cout << "Future Terrain position : " << futurePos2.x << " | " << futurePos2.y << " | " << futurePos2.z << std::endl;

            std::cout << "Camera position : " << rb1.position.x << " | " << rb1.position.y << " | " << rb1.position.z << std::endl;
            std::cout << "Terrain position : " << rb2.position.x << " | " << rb2.position.y << " | " << rb2.position.z << std::endl;

            std::cout << "                        " << std::endl;

            // print velocity of the two models
            std::cout << "Velocity of the camera : " << rb1.velocity.x << " | " << rb1.velocity.y << " | " << rb1.velocity.z << std::endl;
            std::cout << "Velocity of the terrain : " << rb2.velocity.x << " | " << rb2.velocity.y << " | " << rb2.velocity.z << std::endl;
    
            std::cout << "                        " << std::endl;
        }
        else if (!y) {
            collide.collisionNormal.y = glm::normalize(rb1.position - rb2.position).y;
            collide.CollisionDetected = true;
        }
        else if (!z) {
            collide.collisionNormal.z = glm::normalize(rb1.position - rb2.position).z;
            collide.CollisionDetected = true;
            //std::cout << "Collision normal in Z axis : " << collide.collisionNormal.z << std::endl;
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
    float restitution = std::max(rb1.restitution, rb2.restitution);

    float j = restitution * velocityAlongNormal;

    // apply the mass on the object to prevent it to just bounce to the infinity
    j *= rb1.getInverseMass() + rb2.getInverseMass();

    glm::vec3 impulse = j * collisionNormal;

    //std::cout << "Impulse : " << impulse.x << " | " << impulse.y << " | " << impulse.z << std::endl;

    // Apply the impulse to the two models to make them bounce
    if (collisionNormal.x != 0) {
        rb1.velocity.x = -rb1.velocity.x * impulse.x;
        rb2.velocity.x = -rb2.velocity.x * impulse.x;
    }
    if (collisionNormal.y != 0) {
		rb1.velocity.y = -rb1.velocity.y * impulse.y;
        rb2.velocity.y = -rb2.velocity.y * impulse.y;
	}
	if (collisionNormal.z != 0) {
		rb1.velocity.z = -rb1.velocity.z * impulse.z;
        rb2.velocity.z = -rb2.velocity.z * impulse.z;
	}

    /*
    rb1.velocity = -rb1.velocity * impulse;
    rb2.velocity = -rb2.velocity * impulse;
    */

    //Verify if the object is moving behind a certain threshold
    rb1.dampen();
    rb2.dampen();
}

// Handles collision detection and response using predictive logic
void handlePredictiveCollision(RigidBody* rb1, RigidBody* rb2, float deltaTime, std::string tag) {
    CollisionData collisionData = willCollide(*rb1, *rb2, deltaTime);

    if (collisionData.CollisionDetected) {
        //std::cout << "RigidBody tag : " << tag << std::endl;
        //If the two models will collide
        rb1->startSimulate();
        rb2->startSimulate();
        applyImpulse(*rb1, *rb2, collisionData.collisionNormal);
    }
}