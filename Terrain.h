#pragma once
#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

class Terrain
{
public:
    explicit Terrain();
    ~Terrain();

    void Draw(GLuint* shaderProgram, GLuint* VAO);

    GLuint indices[6] = {
        //Indices
        0, 1, 2,
        1, 2, 3,
    };

    GLfloat vertices[12] = {
        //Position
          0.0f,   0.0f,   0.0f,
         -0.5f,   0.0f,   0.0f,
          0.0f,   0.0f,  -0.5f,
         -0.5f,   0.0f,  -0.5f
    };

private:
    glm::vec3 terrainCenter;
};

#endif