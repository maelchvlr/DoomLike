// Player.h
#pragma once
#include "Actor.h"
#include "Camera.h"

class Player : public Actor {
public:
    Player(glm::vec3 position, Camera& camera);

    void Update(float dt) override;

    void UpdateCamera(float dt, GLFWwindow* window, Shader& shaderProgram);
    // Add player-specific methods or properties here
    glm::vec3 getRigidBodyPosition()
    {
        return camera.rb->getPosition();
    }

private:
    Camera& camera;
};

