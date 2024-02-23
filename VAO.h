#pragma once
#ifndef VAO_CLASS
#define VAO_CLASS

#include "glew.h"
#include "VBO.h"

class VAO {
public:
	GLuint ID;

	VAO();
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VAO_CLASS
