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

    // Set the vertices indices and buffer
    VAOTerrain = VAO();
    VBOTerrain = VBO(vertices, sizeof(vertices));

    VAOTerrain.Bind();
    VAOTerrain.LinkAttrib(VBOTerrain, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);

    EBOTerrain = EBO(indices, sizeof(indices));

    // Texture Coord attribute (plus tard du coup)
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    VAOTerrain.Unbind();
    VBOTerrain.Unbind();
    EBOTerrain.Unbind();
}

Terrain::~Terrain() {
    // Cleanup
    VAOTerrain.Delete();
    VBOTerrain.Delete();
    EBOTerrain.Delete();
}

void Terrain::Draw(GLuint shaderProgram) {
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), topLeft);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOTerrain.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOTerrain.Unbind();
}