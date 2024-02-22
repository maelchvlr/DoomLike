#include "Cube.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Cube::Cube(glm::vec3 center)
{
	cubeCenter = center;
}

Cube::~Cube()
{
}

void Cube::Draw(GLuint *shaderProgram, GLuint *VAO)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), cubeCenter);
	glUniformMatrix4fv(glGetUniformLocation(*shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(*VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
