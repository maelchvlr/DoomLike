#pragma once
#include "glm.hpp"
#include "RigidBody.h"
#include <iostream>

class Actor {
public:
    Actor(glm::vec3 position);

    virtual void Update(float dt) = 0; // Virtual method for updating the actor
    glm::vec3 GetPosition() const { return rb->position; }

protected:
    RigidBody* rb;
};