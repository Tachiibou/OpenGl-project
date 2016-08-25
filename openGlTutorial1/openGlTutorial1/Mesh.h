#ifndef MESH_H
#define MESH_H
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "glm\gtx\transform.hpp"
#include "Texture.h"
#include "Material.h"

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
	Mesh(unsigned int numVertices, int indices[], int numIndices, TriangleVertex* tv, Texture* texture, Material* material);
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
	Material* material;
	Texture *texture;
	GLfloat* specular;
	const char* defaultTexture = "obj/brick_16.jpg";
	enum {
		POSITION_VB,
		UV_VB,
		NORMAL_VB,
		INDEX_VB,
		HEIGHTS_VB,
		SPECULAR_VB,
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
	Material* getMaterial();
	
};
#endif