#pragma once
class Texture
{
public:
	Texture(const char* fileLoc, unsigned int _id);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:

	const char* fileLocation;
	unsigned int textureID;
	int width, height, bitDepth;

};

