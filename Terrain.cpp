#include "Terrain.h"
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Terrain::Terrain(glm::vec3 inTopLeft, glm::vec3 inScale) {
    // Generation of a simple flat terrain
    // Define vertices for a flat grid
    float vertices[] = {
        // Positions        // Texture Coords (j'ai vu ça sur internet,
                            //  j'imagine que c'est pour avoir une idée d'où commence et
                            //  où termine une texture)
        0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    // Set the center of the terrain
    topLeft = inTopLeft;

    // Set the scale of the terrain
    scale = inScale;

    // Set the vertices and indices
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coord attribute (plus tard du coup)
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);
}

Terrain::~Terrain() {
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Terrain::Draw(GLuint shaderProgram) {
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), topLeft);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}