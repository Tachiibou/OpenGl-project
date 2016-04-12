#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "GL\glew.h"
#include "Camera.h"
class Shader
{
public:
	Shader(const std::string& fileName, const bool& geo = false);
	virtual ~Shader();

	void Bind();

	void Update(Camera&camera);

	GLuint getProgram();

private:
	int nrOfShaders;

	enum uniform {
		PERSPECTIVE_U,
		VIEW_U,
		VIEW_POS,

		NUM_UNIFORMS
	};

	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint m_program;
	GLuint m_shaders[3];
};

#endif