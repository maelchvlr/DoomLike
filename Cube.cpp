#include "Cube.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Cube::Cube(RigidBody rb, glm::vec3 center) : rbCube(rb)
{
    GLuint indices[] = {
        //Indices
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7,
        0, 1, 4,
        1, 4, 5,
        2, 3, 6,
        3, 6, 7,
        0, 2, 4,
        2, 4, 6,
        1, 3, 5,
        3, 5, 7
    };

    GLfloat vertices[] = {
        //Position
        -0.5f, -0.5f, -0.5f, 
         0.0f, -0.5f, -0.5f,
        -0.5f,  0.0f, -0.5f,
         0.0f,  0.0f, -0.5f,
        -0.5f, -0.5f,  0.0f,
         0.0f, -0.5f,  0.0f,
        -0.5f,  0.0f,  0.0f,
         0.0f,  0.0f,  0.0f,
    };

	rbCube.position = center;
    
    VAOCube = VAO();
    VBOCube = VBO(vertices, sizeof(vertices));
    
    VAOCube.Bind();
    VAOCube.LinkAttrib(VBOCube, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0, false);
    
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
	VAOCube.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    VAOCube.Unbind();
}
