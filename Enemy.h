// Enemy.h
#pragma once
#include "Actor.h"
#include "Cube.h"

class Enemy : public Actor {
public:
    Enemy(glm::vec3 position, float detectionRadius);

    void Update(float dt) override;
    void Render(Shader* shader, float dt);
    bool IsPlayerInRadius(const glm::vec3& playerPosition);
    RigidBody* GetRigidBody() { return cube->getRigidBody(); }

private:
    float detectionRadius;
    Cube* cube;
};