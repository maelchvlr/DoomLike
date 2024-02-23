#pragma once
#ifndef EBO_CLASS
#define EBO_CLASS

#include "glew.h"

class EBO {

public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();

};

#endif // !EBO_CLASS