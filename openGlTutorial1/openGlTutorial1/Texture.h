#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm\glm.hpp"
#include "GL\glew.h"
#include <vector>

class Texture
{
private:
	std::vector<GLuint> textures;
	GLuint texture;
public:
	Texture();
	Texture(const char* fileName);

	void Bind();
	~Texture();

	void addTexture(const char* fileName);
};

#endif