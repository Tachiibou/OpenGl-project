#ifndef MESH_H
#define MESH_H
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "Texture.h"
// Vertex class can be used for normals aswell
class Vertex {
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
// Struct used for sending data to GPU
struct TriangleVertex
{
	float x, y, z;
	float u, v;
	float xn, yn, zn;
};
class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, int indices[], int indexSize);
	Mesh(unsigned int numVertices, int indices[], int numIndices, TriangleVertex* tv, Texture* texture);
	virtual ~Mesh();
	void Draw();
private:
	Texture *texture;
	const char* defaultTexture = "obj/cartoonStone.jpg";
	enum {
		POSITION_VB,
		UV_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;
	GLuint indexBufferObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
	unsigned int indexArraySize;
	bool usingVertexInfo;
};
#endif