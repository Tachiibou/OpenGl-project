#include "Mesh.h"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

Mesh::Mesh(unsigned int numVertices, int indices[], int numIndices, VertexInfoStruct* tv, Texture* texture, Material* material)
{
	this->material = material;
	this->usingVertexInfo = true;
	this->m_drawCount = numVertices;
	this->indexArraySize = numIndices;
	GLuint pos, uv, normal;
	float f = numVertices*sizeof(tv[0]);
	m_vertexArrayBuffers = new GLuint[NUM_BUFFERS - 1];

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS-1, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(vertexInfo->pos), vertexInfo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(sizeof(float) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(sizeof(float) * 5));

	specular = new GLfloat[3 * numVertices];

	for (int i = 0; i < numVertices;i++) {
		specular[0 + i*3] = material->getSpecularRef().x;
		specular[1 + i*3] = material->getSpecularRef().y;
		specular[2 + i*3] = material->getSpecularRef().z;
	}

	//specular = new GLfloat[3 ];
	//	specular[0 ] = material->getSpecularRef().x;
	//	specular[1] = material->getSpecularRef().y;
	//	specular[2] = material->getSpecularRef().z;


	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[HEIGHTS_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(specular[0])*3 , specular, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(specular[0]) * 3,0);

	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[HEIGHTS_VB]);
	//glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(heights[0]), heights, GL_STATIC_DRAW);
	//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	if (texture == nullptr) // default texture is brick
		this->texture = new Texture(this->defaultTexture);
	else
		this->texture = texture;

	this->worldPos = glm::vec3();
	this->worldMatrix = glm::mat4(1.0f);
}

//For Terrain
Mesh::Mesh(unsigned int numVertices, int indices[], int numIndices, VertexInfoStruct* tv, Texture* texture, float* heights) {
	specular = nullptr;
	material = nullptr;
	this->usingVertexInfo = true;
	this->m_drawCount = numVertices;
	this->indexArraySize = numIndices;
	GLuint pos, uv, normal;
	float f = numVertices*sizeof(tv[0]);
	m_vertexArrayBuffers = new GLuint[NUM_BUFFERS];

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(vertexInfo->pos), vertexInfo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(sizeof(float) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfoStruct), BUFFER_OFFSET(sizeof(float) * 5));

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[HEIGHTS_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(heights[0]), heights, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	if (texture == nullptr) // default texture is brick
		this->texture = new Texture(this->defaultTexture);
	else
		this->texture = texture;

	this->worldPos = glm::vec3();
	this->worldMatrix = glm::mat4(1.0f);
}
Mesh::~Mesh()
{
	if (this->texture != nullptr)
		delete this->texture;
	glDeleteVertexArrays(1, &m_vertexArrayObject);

	delete[]m_vertexArrayBuffers;
	if(material != nullptr)
		delete material;

	delete[] specular;
}
void Mesh::Draw() {
	texture->Bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindVertexArray(m_vertexArrayObject);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	//glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLES, this->indexArraySize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setPos(glm::vec3 pos)
{
	this->worldPos = pos;
	this->bbox.pos = pos;
	this->worldMatrix = glm::translate(this->worldPos);
}

void Mesh::setWorldMatrix(glm::mat4 worldMatrix)
{
	this->worldMatrix = worldMatrix;
}

glm::vec3& Mesh::getPos()
{
	return this->worldPos;
}

glm::mat4& Mesh::getWorldMatrix()
{
	return this->worldMatrix;
}

void Mesh::createBoundingBox(float halfSize)
{
	this->bbox.pos = this->worldPos;
	this->bbox.halfSize = halfSize;
}

BoundingBox Mesh::getBoundingBox()
{
	return bbox;
}

Material* Mesh::getMaterial() {
	return this->material;
}