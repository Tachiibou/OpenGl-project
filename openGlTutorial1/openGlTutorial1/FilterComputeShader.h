#ifndef FILTER_COMPUTE_SHADER_H
#define FILTER_COMPUTE_SHADER_H
#include <string>
#include "GL\glew.h"
#include "glm\glm.hpp"

#include <string>
#include <fstream>

class FilterComputeShader
{
private:
	GLuint program;

public:
	FilterComputeShader(std::string fileName);
	~FilterComputeShader();
	std::string LoadShader(const std::string& fileName);
	void CreateShader(const std::string& text);
	void BindShader();
	void DispatchCompute(int width, int height, int depth);
	GLint GetUniformLocation(std::string name);
	void Uniform1f(std::string name, float number);
	void UniformVec3(std::string name, glm::vec3 vector);
};
#endif