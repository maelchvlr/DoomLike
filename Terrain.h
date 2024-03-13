#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <glew.h> // Include OpenGL loader library
#include <glm.hpp> // Include OpenGL Mathematics library
#include "Texture.h"

class Terrain {
public:
    explicit Terrain(glm::vec3 inCenter = glm::vec3(0), glm::vec3 inTopLeft = glm::vec3(1));  // Constructor to set up the terrain data
    ~Terrain(); // Destructor for cleanup

    void Draw(Shader *shaderProgram); // Method to draw the terrain

private:
    // OpenGL IDs for vertex array, vertex buffer, and element buffer objects
    VAO VAOTerrain;
    VBO VBOTerrain;
    EBO EBOTerrain;

    glm::vec3 topLeft; // Center of the terrain
    glm::vec3 scale; // Scale of the terrain
    //void setupMesh() je sais pas trop ce qu'on peut en faire ?
};

#endif // TERRAIN_H