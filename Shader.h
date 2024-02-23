#pragma once
#ifndef SHADER_CLASS
#define SHADER_CLASS

#include "glew.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
	void debug();

private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif // !SHADER_CLASS