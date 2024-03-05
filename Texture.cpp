#include "Texture.h"

Texture::Texture(const char* fileLoc, unsigned int _id)
{
		fileLocation = fileLoc;
		textureID = _id;
		width = 0;
		height = 0;
		bitDepth = 0;
}

Texture::~Texture()
{
}

void Texture::LoadTexture()
{

}

void Texture::UseTexture()
{
}

void Texture::ClearTexture()
{
}
