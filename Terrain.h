#ifndef TERRAIN_H
#define TERRAIN_H

#include <GLEW.h> // Include OpenGL loader library
#include <glm.hpp> // Include OpenGL Mathematics library

class Terrain {
public:
    explicit Terrain(glm::vec3 inCenter = glm::vec3(0), glm::vec3 inTopLeft = glm::vec3(1));  // Constructor to set up the terrain data
    ~Terrain(); // Destructor for cleanup

    void Draw(GLuint shaderProgram); // Method to draw the terrain

private:
    GLuint VAO, VBO, EBO; // OpenGL IDs for vertex array, vertex buffer, and element buffer objects
    glm::vec3 topLeft; // Center of the terrain
    glm::vec3 scale; // Scale of the terrain
    //void setupMesh() je sais pas trop ce qu'on peut en faire ?
};

#endif // TERRAIN_H