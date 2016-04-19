#include "Shader.h"
#include <iostream>
#include <fstream>

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errormessage);
static std::string LoadShader(const std::string &fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string & fileName, const bool& geo)
{
	m_program = glCreateProgram();

	if (geo)
	{
		this->nrOfShaders = 3;
		//this->m_shaders = new GLuint[this->nrOfShaders];
		m_shaders[0] = CreateShader(LoadShader("./res/" + fileName + "Vertex.glsl"), GL_VERTEX_SHADER);
		m_shaders[1] = CreateShader(LoadShader("./res/" + fileName + "Fragment.glsl"), GL_FRAGMENT_SHADER);
		m_shaders[2] = CreateShader(LoadShader("./res/" + fileName + "Geometry.glsl"), GL_GEOMETRY_SHADER);
	}
	else
	{
		this->nrOfShaders = 2;
		//this->m_shaders = new GLuint[this->nrOfShaders];
		m_shaders[0] = CreateShader(LoadShader("./res/" + fileName + "Vertex.glsl"), GL_VERTEX_SHADER);
		m_shaders[1] = CreateShader(LoadShader("./res/" + fileName + "Fragment.glsl"), GL_FRAGMENT_SHADER);
	}

	for (unsigned int i = 0; i < nrOfShaders; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniforms[uniform::PERSPECTIVE_U] = glGetUniformLocation(m_program, "perspectiveMatrix");
	m_uniforms[uniform::VIEW_U] = glGetUniformLocation(m_program, "viewMatrix");
	m_uniforms[uniform::VIEW_POS] = glGetUniformLocation(m_program, "viewPos");
}

void Shader::Update(Camera&camera) {

	glUniformMatrix4fv(m_uniforms[uniform::VIEW_U], 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
	glUniformMatrix4fv(m_uniforms[uniform::PERSPECTIVE_U], 1, GL_FALSE, &camera.getPerspectiveMatrix()[0][0]);
	glUniform3f(m_uniforms[uniform::VIEW_POS], camera.getPos().x, camera.getPos().y, camera.getPos().z);
}

GLuint Shader::getProgram()
{
	return this->m_program;
}

static GLuint CreateShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0) 
	{
		std::cerr << "Error: Shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLength[1];
	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLength[0] = text.length();
	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader Compilation Error");

	return shader;
}

Shader::~Shader()
{
	for (unsigned i = 0; i < this->nrOfShaders; i++) 
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);

	//delete[]this->m_shaders;
}

void Shader::Bind() {
	glUseProgram(m_program);
}

static std::string LoadShader(const std::string &fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}