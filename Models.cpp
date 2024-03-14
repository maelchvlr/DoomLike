#include "Models.h"

Models::Models(float mass, bool _Textured, Texture* _Texture, glm::vec3 center) 
	: rb (mass, center, glm::vec3(0.0f), glm::vec3(0.0f))
{
	textured = _Textured;
	texture = _Texture;
	rb.position = center;
}

Models::~Models() {
	VAOModel.Delete();
	VBOModel.Delete();
	EBOModel.Delete();
}

void Models::init(GLfloat vertices[], int sizeVer, GLuint indices[], int sizeInd) {
	
	VAOModel = VAO();
	VBOModel = VBO(vertices, sizeVer);
	VAOModel.Bind();
    //3 numComponents = x y z, 5 stride = 3 numComponents + 2 for textures
    VAOModel.LinkAttrib(VBOModel, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0, true);

    EBOModel = EBO(indices, sizeInd);

    VAOModel.Unbind();
    VBOModel.Unbind();
    EBOModel.Unbind();

}

//It's only purpose is to be overriten by the children classes
void Models::Draw(Shader* shaderProgram, float dt) {}