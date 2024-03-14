#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Models.h"

#include <glew.h> // Include OpenGL loader library
#include <glm.hpp> // Include OpenGL Mathematics library

class Terrain : Models
{
public:
    explicit Terrain(glm::vec3 topLeft = glm::vec3(0), glm::vec3 inScale = glm::vec3(1), bool _Textured = false, Texture* _Texture = nullptr, float mass = 0.0f);  // Constructor to set up the terrain data
    ~Terrain(); // Destructor for cleanup

    void Draw(Shader *shaderProgram, float dt) override; // Method to draw the terrain

private:

    glm::vec3 scale; // Scale of the terrain
    //void setupMesh() je sais pas trop ce qu'on peut en faire ?
};

#endif // TERRAIN_H