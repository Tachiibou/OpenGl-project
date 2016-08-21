#ifndef MESH_H
#define MESH_H
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "glm\gtx\transform.hpp"
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

struct BoundingBox {
	float halfSize;
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
	Mesh(unsigned int numVertices, int indices[], int numIndices, TriangleVertex* tv, Texture* texture, float* heights);
	virtual ~Mesh();
	void Draw();
	
	void setPos(glm::vec3 pos);
	void setWorldMatrix(glm::mat4 worldMatrix);
	glm::vec3& getPos();
	glm::mat4& getWorldMatrix();

	void createBoundingBox(float halfSize);
	BoundingBox getBoundingBox();
private:
	Texture *texture;
	const char* defaultTexture = "obj/brick_16.jpg";
	enum {
		POSITION_VB,
		UV_VB,
		NORMAL_VB,
		INDEX_VB,
		HEIGHTS_VB,
		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;
	GLuint indexBufferObject;
	GLuint *m_vertexArrayBuffers;
	unsigned int m_drawCount;
	unsigned int indexArraySize;
	bool usingVertexInfo;

	glm::vec3 worldPos;
	glm::mat4 worldMatrix;
	BoundingBox bbox;
	
};
#endif