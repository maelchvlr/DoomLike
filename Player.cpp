// Player.cpp
#include "Player.h"

Player::Player(glm::vec3 position, Camera& _camera)
    : Actor(position), camera(_camera) {}


void Player::Update(float dt)
{
    rb = camera.rb;
}

void Player::UpdateCamera(float dt, GLFWwindow* window, Shader& shaderProgram) {
    camera.Inputs(window);
    camera.Matrix("camMatrix", &shaderProgram, dt);
}