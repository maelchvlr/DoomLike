#include "Cube.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Cube::Cube(RigidBody rb, glm::vec3 center, bool _Textured, Texture* _Texture) : rbCube(rb)
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

    textured = _Textured;
    texture = _Texture;

	rbCube.position = center;
    
    VAOCube = VAO();
    VBOCube = VBO(vertices, sizeof(vertices));
    
    VAOCube.Bind();
    VAOCube.LinkAttrib(VBOCube, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0, true);
    
    EBOCube = EBO(indices, sizeof(indices));

    VAOCube.Unbind();
    VBOCube.Unbind();
    EBOCube.Unbind();
}

Cube::~Cube()
{
    VAOCube.Delete();
	VBOCube.Delete();
	EBOCube.Delete();
}

void Cube::Draw(Shader *shaderProgram, float dt)
{
    rbCube.update(dt);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), rbCube.position);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    if(textured)
    {
        texture->texUnit(*shaderProgram, "tex0", 0);
        glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexture"), 1);
        texture->Bind();
    }

	VAOCube.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    VAOCube.Unbind();

    if (textured)
    {
        texture->Unbind();
        glUniform1i(glGetUniformLocation(shaderProgram->ID, "useTexture"), 0);
    }
}
