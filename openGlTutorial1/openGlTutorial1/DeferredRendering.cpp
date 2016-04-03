#include "DeferredRendering.h"
#include "ConstVariables.h"
#include <iostream>

DeferredRendering::DeferredRendering()
{
	this->geometry = new Shader("geo");
	this->light = new Shader("light");

	//Framebuffer
	glGenFramebuffers(1, &this->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

	//Position
	glGenTextures(1, &this->fPosition);
	glBindTexture(GL_TEXTURE_2D, this->fPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, constVar::WINDOW_WIDTH, constVar::WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fPosition, 0);
	this->attachments[0] = GL_COLOR_ATTACHMENT0;

	//Normal
	glGenTextures(1, &this->fNormal);
	glBindTexture(GL_TEXTURE_2D, this->fNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, constVar::WINDOW_WIDTH, constVar::WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->fNormal, 0);
	this->attachments[1] = GL_COLOR_ATTACHMENT1;

	//Color
	glGenTextures(1, &this->fColor);
	glBindTexture(GL_TEXTURE_2D, this->fColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, constVar::WINDOW_WIDTH, constVar::WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->fColor, 0);
	this->attachments[2] = GL_COLOR_ATTACHMENT2;

	glDrawBuffers(3, this->attachments);

	glGenRenderbuffers(1, &this->depth);
	glBindRenderbuffer(GL_RENDERBUFFER, this->depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, constVar::WINDOW_WIDTH, constVar::WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Frambuffer faild!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DeferredRendering::~DeferredRendering()
{
	delete this->geometry;
	delete this->light;

	glDeleteFramebuffers(1, &this->frameBuffer);
	glDeleteTextures(1, &this->fPosition);
	glDeleteTextures(1, &this->fNormal);
	glDeleteTextures(1, &this->fColor);
	glDeleteRenderbuffers(1, &this->depth);
}

void DeferredRendering::geoPass(Camera& cam)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->geometry->Bind();
	this->geometry->Update(cam);
}

void DeferredRendering::endGeoPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
