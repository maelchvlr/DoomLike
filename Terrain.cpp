#include "Terrain.h"

#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Terrain::Terrain(glm::vec3 inTopLeft, glm::vec3 inScale): topLeft(inTopLeft), scale(inScale) {
    // Generation of a simple flat terrain
    // Define vertices for a flat grid
    float vertices[] = {
        // Positions        // Texture Coords (j'ai vu �a sur internet,
                            //  j'imagine que c'est pour avoir une id�e d'o� commence et
                            //  o� termine une texture)
        0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

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
    
    //Impl�menter la matrice de translation en dehors de la fonction draw
    // l'initialiser soit lors de la cr�ation de l'objet, soit lors 
    // de l'appel de la fonction draw dans le while
    glm::mat4 translation = glm::mat4(1.0f);
    translation[0].x = 1.0f;
    translation[1].y = 0.0f;
    translation[2].z = 1.0f;

    glm::mat4 model = glm::translate(translation, topLeft * scale);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOTerrain.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOTerrain.Unbind();
}