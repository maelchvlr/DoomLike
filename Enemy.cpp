// Enemy.cpp
#include "Enemy.h"
#include <gtx/norm.hpp>


Enemy::Enemy(glm::vec3 position, float detectionRadius)
    : Actor(position), detectionRadius(detectionRadius) 
{
    cube = new Cube(position, glm::vec3(0.3), false, nullptr, 10, 0, true);
}

void Enemy::Update(float dt) 
{
    rb = cube->getRigidBody();
}

void Enemy::Render(Shader* shader, float dt) {
    glUniform3f(glGetUniformLocation(shader->ID, "color"), 1.0f, 0.0f, 0.0f);
    cube->Draw(shader, dt);
}

bool Enemy::IsPlayerInRadius(const glm::vec3& playerPosition) {
    float distanceSq = glm::distance2(rb->position, playerPosition);
    bool isPlayerInRadius = distanceSq <= (detectionRadius * detectionRadius);
    if (isPlayerInRadius) {
        //std::cout << "Player is in radius!" << std::endl;

        // slowly move towards player
        glm::vec3 direction = glm::normalize(playerPosition - rb->position);
        rb->position += direction * 0.01f;

	}
	return isPlayerInRadius;
}