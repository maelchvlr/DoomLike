#pragma once
#ifndef VBO_CLASS
#define VBO_CLASS

#include "glew.h"

class VBO {
public:
	GLuint ID;

	VBO();
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VBO_CLASS