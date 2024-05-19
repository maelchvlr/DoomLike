#pragma once
#include "Models.h"
#include "glm.hpp"
#include <iostream>

struct CollisionData {
    bool CollisionDetected = false;
    glm::vec3 collisionNormal = glm::vec3(0);
};

// Checks if two models will collide based on their current velocities and positions
CollisionData& willCollide(RigidBody& rb1, RigidBody& rb2, float deltaTime) {

    CollisionData collide;

    // Calculate the future positions of the two models
    glm::vec3 futurePos1 = rb1.position + rb1.velocity * deltaTime;
    glm::vec3 futurePos2 = rb2.position + rb2.velocity * deltaTime;

    //Re define the min and max of the two future models

    //Ici, on prend en compte la size des rb qui rentrent en collision. Cela
    // peut etre problematique dans le cas d'une collision avec une surface plate comme un mur 
    // mais ayant un taille non nulle car pouvant etre positionné dans differentes directions

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
            float surface_collider_z_min = 0;
            float surface_collider_z_max = 0;
            float surface_collider_y_collision = 0;
            float surface_collider_y_min = 0;

            rb2_max.z = abs(rb2_max.z);
            rb2_min.z = abs(rb2_min.z);
            rb1_max.z = abs(rb1_max.z);
            rb1_min.z = abs(rb1_min.z);

            //On recupere notre largeur de surface de collision
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

            rb1_min.y = abs(rb1_min.y);
            rb1_max.y = abs(rb1_max.y);
            rb2_min.y = abs(rb2_min.y);
            rb2_max.y = abs(rb2_max.y);

            //On recupere la hauteur de cette surface
            if (rb1_min.y < rb2_min.y) {
                surface_collider_y_min = rb2_min.y;
            }
            else {
                surface_collider_y_min = rb1_min.y;
            }

            if (rb1_max.y < rb2_max.y) {
                surface_collider_y_collision = rb1_max.y;
            }
            else {
                surface_collider_y_collision = rb2_max.y;
            }

            //Verification de la Surface de collision
            float CollisionSurface = (surface_collider_z_max - surface_collider_z_min) * (surface_collider_y_collision - surface_collider_y_min);

            if (CollisionSurface > 0.1f) {
                std::cout << "COLLISION DETECTED ! " << std::endl;
                std::cout << "Collision surface : " << CollisionSurface << std::endl;
                collide.collisionNormal.x = glm::normalize(rb1.position - rb2.position).x;
                //std::cout << "Rb1 position : " << rb1.position.x << " | " << rb1.position.y << " | " << rb1.position.z << std::endl;
                //std::cout << "Rb2 position : " << rb2.position.x << " | " << rb2.position.y << " | " << rb2.position.z << std::endl;
                //std::cout << "Collision normal in X axis : " << collide.collisionNormal.x << std::endl;
                collide.CollisionDetected = true;
            }
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
    float velocityAlongNormal = abs(glm::dot(relativeVelocity, collisionNormal));

    // They are moving apart, so no impulse is necessary
    if (velocityAlongNormal < 0) return;

    // Restitution depends of the bounciness of the object
    float restitution = std::min(rb1.restitution, rb2.restitution);

    float j = restitution * velocityAlongNormal;

    // apply the mass on the object to prevent it to just bounce to the infinity
    j *= rb1.getInverseMass() + rb2.getInverseMass();
    
    glm::vec3 impulse = abs(j * collisionNormal);

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

    //Verify if the object is moving behind a certain threshold
    rb1.dampen();
    rb2.dampen();
}

// Handles collision detection and response using predictive logic
void handlePredictiveCollision(RigidBody* rb1, RigidBody* rb2, float deltaTime, std::string tag) {
    CollisionData collisionData = willCollide(*rb1, *rb2, deltaTime);

    if (collisionData.CollisionDetected) {
        if (collisionData.collisionNormal.z != 0) {
            std::cout << "Collision detected on z" << std::endl;
            std::cout << tag << std::endl;
            std::cout << "camera rb position : " << rb1->position.x << " | " << rb1->position.y << " | " << rb1->position.z << std::endl;
            std::cout << "wall rb position : " << rb2->position.x << " | " << rb2->position.y << " | " << rb2->position.z << std::endl;
        }

        //If the two models will collide
        rb1->startSimulate();
        rb2->startSimulate();
        applyImpulse(*rb1, *rb2, collisionData.collisionNormal);
    }
}