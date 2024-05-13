#include "Cube.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


Cube::Cube(glm::vec3 center, glm::vec3 _size, bool _Textured, Texture* _Texture, float mass, float restitution, bool movable)
    : Models(mass, restitution, movable, _Textured, _Texture, center, _size)
{
    GLuint indices[] = {
        // Front face
        0,  1,  2,  2,  3,  0,
        // Right face
        4,  5,  6,  6,  7,  4,
        // Back face
        8,  9,  10, 10, 11, 8,
        // Left face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    GLfloat vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left

        // Right face
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left

         // Back face
          0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
          0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left

          // Left face
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
          -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left

          // Top face
          -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
           0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left

          // Bottom face
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
           0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right
           0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // Top-right
          -0.5f, -0.5f,  0.5f,  0.0f, 1.0f  // Top-left
    };

    Models::init(vertices, sizeof(vertices), indices, sizeof(indices));
}

Cube::~Cube()
{
    Models::~Models();
}

void Cube::Draw(Shader *shaderProgram, float dt)
{
    rb.update(dt);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), rb.position);
    model = glm::scale(model, size);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if(textured)
    {
        texture->texUnit(*shaderProgram, "tex0", 0);
        glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexture"), 1);
        texture->Bind();
    }

	VAOModel.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    VAOModel.Unbind();

    if (textured)
    {
        texture->Unbind();
        glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexture"), 0);
    }
}
