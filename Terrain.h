#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Models.h"

class Terrain : public Models
{
public:
    explicit Terrain(std::string path,  glm::vec3 topLeft = glm::vec3(0), glm::vec3 size = glm::vec3(1), bool _Textured = false, Texture* _Texture = nullptr, float mass = 0.0f, float restitution = 0.1f, bool movable = false);  // Constructor to set up the terrain data
    ~Terrain(); // Destructor for cleanup

    void Draw(Shader *shaderProgram, float dt) override; // Method to draw the terrain

private:

    //void setupMesh() //je sais pas trop ce qu'on peut en faire ?
};

#endif // TERRAIN_H