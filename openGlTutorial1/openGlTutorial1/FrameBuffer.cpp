#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	this->framebufferName = -1;
	this->renderedTexture = -1;
	this->texUniformID = -1;
	this->textureAmount = -1;
	this->drawBuffers = nullptr;
}


FrameBuffer::~FrameBuffer()
{
	if (this->drawBuffers != nullptr)
		delete this->drawBuffers;
	glDeleteFramebuffers(1,&this->framebufferName);
}

void FrameBuffer::CreateFrameBuffer(int textureAmount, int screenWidth, int screenHeight)
{
	this->textureAmount = textureAmount;
	glGenFramebuffers(1, &this->framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferName);

	// The texture we're going to render to
	for (int i = 0; i < this->textureAmount; i++)
	{
		glGenTextures(1, &this->textures[i]);
	}


	for (int i = 0; i < this->textureAmount; i++)
	{
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	for (int i = 0; i < this->textureAmount; i++)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, this->textures[i], 0);
	}

	// Set the list of draw buffers.
	GLenum DrawBuffers[NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };

	glDrawBuffers(NUM_TEXTURES, DrawBuffers); // "NUM_TEXTURES" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR CREATING FRAMEBUFFER" << std::endl;
}

void FrameBuffer::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferName);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear last frame from texture
}

void FrameBuffer::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexturesToProgram(GLuint uniform, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + texture);

	glUniform1i(uniform, texture);
	glBindTexture(GL_TEXTURE_2D, this->textures[texture]);
	
}

void FrameBuffer::BindTexturesToProgram(GLuint uniform, GLuint texture, GLuint bindingLocation)
{
	glActiveTexture(GL_TEXTURE0 + bindingLocation);

	glBindTexture(GL_TEXTURE_2D, this->textures[texture]);
	glUniform1i(uniform, bindingLocation);
}

void FrameBuffer::BindImageTexturesToProgram(GLuint uniform, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + texture);

	glUniform1i(uniform, texture);
	glBindImageTexture(texture, this->textures[texture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}