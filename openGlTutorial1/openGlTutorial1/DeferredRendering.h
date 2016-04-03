#ifndef DEFERREDRENDERING_H
#define DEFERREDRENDERING_H

#include "Shader.h"
#include "Camera.h"

class DeferredRendering
{
private:
	Shader* geometry, * light;
	GLuint frameBuffer, fPosition, fNormal, fColor, depth;
	GLuint fPositonLoc, fNormalLoc, fColorLoc;
	GLuint attachments[3];
public:
	DeferredRendering();
	~DeferredRendering();


	void geoPass(Camera& cam);
	void endGeoPass();
};

#endif