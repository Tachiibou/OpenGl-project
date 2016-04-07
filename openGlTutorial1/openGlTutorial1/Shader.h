#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "GL\glew.h"
#include "Camera.h"
class Shader
{
public:
	Shader(const std::string& fileName, bool& geo);
	virtual ~Shader();

	void Bind();

	void Update(Camera&camera);

private:

	static const unsigned int NUM_SHADERS = 2;
	int nrOfShaders;

	enum uniform {
		PERSPECTIVE_U,
		VIEW_U,

		NUM_UNIFORMS
	};

	GLuint m_uniforms[NUM_UNIFORMS];
	GLuint m_program;
	GLuint* m_shaders;
};

#endif