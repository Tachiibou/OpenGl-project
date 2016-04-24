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
}

void FrameBuffer::CreateFrameBuffer(int textureAmount)
{
	this->textureAmount = textureAmount;
	glGenFramebuffers(1, &this->framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferName);

	// The texture we're going to render to
	for (int i = 0; i < this->textureAmount; i++)
	{
		glGenTextures(1, &this->textures[i]);
	}
	//glGenTextures(1, &this->textures[0]);
	//glGenTextures(1, &renderedTexture2);


	for (int i = 0; i < this->textureAmount; i++)
	{
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	//// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, renderedTexture2);

	//// Give an empty image to OpenGL ( the last "0" )
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//// Poor filtering. Needed !
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	for (int i = 0; i < this->textureAmount; i++)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, this->textures[i], 0);
	}
	//// Set "renderedTexture" as our colour attachement #0
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->textures[0], 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, this->textures[1], 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	//this->drawBuffers = new GLenum[this->textureAmount];
	//for (int i = 0; i < this->textureAmount; i++)
	//{
	//	this->drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	//}
	glDrawBuffers(NUM_TEXTURES, DrawBuffers); // "NUM_TEXTURES" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR CREATING FRAMEBUFFER" << std::endl;
}

void FrameBuffer::AddDepthMap()
{
	glGenFramebuffers(1, &this->depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);

	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, constVar::SHADOW_WIDTH, constVar::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferName);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear last frame from texture
}

void FrameBuffer::BindDepthMapFB()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
}

void FrameBuffer::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexturesToProgram(GLuint uniform, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + texture);
	//this->texUniformID = glGetUniformLocation(program, "renderedTexture");
	glUniform1i(uniform, texture);
	glBindTexture(GL_TEXTURE_2D, this->textures[texture]);
}

void FrameBuffer::BindDepthMapToProgram(GLuint uniform)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform, 0);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
}
