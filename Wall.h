#pragma once
#ifndef WALL_H
#define WALL_H

#include "Models.h"

class Wall : public Models
{

public:
    explicit Wall(std::string path, 
        glm::vec3 topLeft = glm::vec3(0), 
        glm::vec3 size = glm::vec3(1),
        bool _Textured = false, 
        Texture* _Texture = nullptr, 
        float mass = 0.0f, 
        float restitution = 0.1f, 
        bool movable = false);  // Constructor to set up the terrain data

    glm::mat4 model;            // Model matrix of the wall

    ~Wall();                    // Destructor for cleanup

    void Draw(Shader* shaderProgram, float dt) override; // Method to draw the terrain
};

#endif