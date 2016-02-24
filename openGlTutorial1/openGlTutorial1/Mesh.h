#ifndef MESH_H
#define MESH_H

#include "glm\glm.hpp"
#include "GL\glew.h"

class Vertex { // Vertex class can be used for normals aswell
public:
	Vertex(const glm::vec3&pos) {
		this->pos = pos;
	}
	Vertex() {
		this->pos = glm::vec3(0, 0, 0);
	}
protected:
private:
	glm::vec3 pos;
	
};
class UV { // UV class
public:
	UV(const glm::vec2&UVpos) {
		this->UVpos = UVpos;
	}
	UV() {
		this->UVpos = glm::vec2(0, 0);
	}
protected:
private:
	glm::vec2 UVpos;

};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, int indices[], int indexSize);
	virtual ~Mesh();

	void Draw();
private:

	enum {
		POSITION_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint indexBufferObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
	unsigned int indexArraySize;
};

#endif