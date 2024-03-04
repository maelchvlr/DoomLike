#include "Terrain.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Terrain::Terrain() {}

Terrain::~Terrain() {}

void Terrain::Draw(GLuint* shaderProgram, GLuint* VAO)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), terrainCenter);
	glUniformMatrix4fv(glGetUniformLocation(*shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(*VAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}
