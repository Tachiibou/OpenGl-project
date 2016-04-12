#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "glm\glm.hpp"
#include "GL\glew.h"
#include <iostream>
class FrameBuffer
{
private:
	enum
	{
		TEXTURE1,
		TEXTURE2,
		TEXTURE3,
		TEXTURE4,

		NUM_TEXTURES
	};
	GLuint texUniformID; // getuniformlocation 
	GLuint framebufferName;
	GLuint renderedTexture, renderedTexture2;
	GLuint textures[NUM_TEXTURES];
	GLenum* drawBuffers;
	int textureAmount;

public:
	FrameBuffer();
	~FrameBuffer();
	void CreateFrameBuffer(int textureAmount);
	void BindFrameBuffer();
	void UnbindFrameBuffer();
	void BindTexturesToProgram(GLuint uniform, GLuint texture);
};
#endif