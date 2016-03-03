#include "Mesh.h"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, int indices[], int numIndices)
{
	this->usingVertexInfo = false;
	this->m_drawCount = numVertices;
	this->indexArraySize = numIndices;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,numIndices * sizeof(indices[0]),indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

Mesh::Mesh(unsigned int numVertices, int indices[], int numIndices, TriangleVertex* tv, Texture* texture)
{
	this->usingVertexInfo = true;
	this->m_drawCount = numVertices;
	this->indexArraySize = numIndices;
	GLuint pos, uv, normal;

	float f = numVertices*sizeof(tv[0]);

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(vertexInfo->pos), vertexInfo, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(tv[0]), tv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 5));

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
}

Mesh::~Mesh()
{
	if (this->texture != nullptr)
		delete this->texture;
	glDeleteVertexArrays(1, &m_vertexArrayObject);
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

	glDrawElements(GL_TRIANGLES,this->indexArraySize ,GL_UNSIGNED_INT,0);

	glBindVertexArray(0);
}
