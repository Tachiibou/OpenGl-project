#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm\glm.hpp"
#include "GL\glew.h"

class Texture
{
private:
		GLuint texture;
public:
	Texture();
	Texture(const char* filename);

	void Bind();
	~Texture();
};

#endif