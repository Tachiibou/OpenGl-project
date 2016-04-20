#include "FilterComputeShader.h"



FilterComputeShader::FilterComputeShader(std::string fileName)
{
	this->program = -1;
}


FilterComputeShader::~FilterComputeShader()
{
}

std::string FilterComputeShader::LoadShader(const std::string & fileName)
{
	std::ifstream shaderFile;
	shaderFile.open((fileName).c_str());

	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	return shaderText;
}

void FilterComputeShader::CreateShader(const std::string & text)
{
	// Creating the compute shader, and the program object containing the shader
	this->program = glCreateProgram();
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	const char* shaderTextPtr = text.c_str();

	glShaderSource(shader, 1, &shaderTextPtr, NULL);
	glCompileShader(shader);

	int rvalue;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stderr, "Error in compiling the compute shader\n");
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(shader, 10239, &length, log);
		fprintf(stderr, "Compiler log:\n%s\n", log);
		exit(40);
	}
	glAttachShader(this->program, shader);

	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stderr, "Error in linking compute shader program\n");
		GLchar log[10240];
		GLsizei length;
		glGetProgramInfoLog(this->program, 10239, &length, log);
		fprintf(stderr, "Linker log:\n%s\n", log);
		exit(41);
	}
	glUseProgram(this->program);

	//glUniform1i(glGetUniformLocation(progHandle, "destTex"), 0);
}

void FilterComputeShader::BindShader()
{
	glUseProgram(this->program);
}

void FilterComputeShader::DispatchCompute(int width, int height, int depth)
{
	glDispatchCompute(width, height, depth);
}

GLint FilterComputeShader::GetUniformLocation(std::string name)
{
	GLint d = glGetUniformLocation(this->program, name.c_str());
	return glGetUniformLocation(this->program, name.c_str());
}

void FilterComputeShader::Uniform1f(std::string name, float number)
{
	glUniform1f(glGetUniformLocation(this->program, name.c_str()), number);
}

void FilterComputeShader::UniformVec3(std::string name, glm::vec3 vector)
{
	glUniform3fv(glGetUniformLocation(this->program, name.c_str()), 1, &vector[0]);
}