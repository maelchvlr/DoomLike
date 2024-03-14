#include "Terrain.h"

#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Terrain::Terrain( glm::vec3 topLeft, glm::vec3 _size, bool _Textured, Texture* _Texture, float mass, float restitution, bool movable)
    : Models(mass, restitution, movable, _Textured, _Texture, topLeft, _size), size(_size)
{
    // Define vertices for a flat grid
    float vertices[] = {
        // Positions        // Texture Coords 
        0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    Models::init(vertices, sizeof(vertices), indices, sizeof(indices));
}

Terrain::~Terrain() {
    Models::~Models();
}

void Terrain::Draw(Shader *shaderProgram, float dt) {

    rb.update(dt);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), rb.position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOModel.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOModel.Unbind();
}

