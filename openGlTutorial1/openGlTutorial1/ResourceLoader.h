#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include<string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"


class ResourceLoader
{
private:
	std::string fileName;
	Vertex* VertexVectorToArray(std::vector<Vertex> vertices);
	Vertex* vertexArray;
	Vertex* createVertices(std::vector<glm::vec3> pos);
	Vertex* createVerticesFromLine(std::string line, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> UVs);
	
	void insertVertex(std::string line, std::vector<glm::vec3>& vertexVector);
	void insertUV(std::string line, std::vector<glm::vec2>& UVector);
	void insertNormal(std::string line, std::vector<glm::vec3>& normalVector); // Vertex used for normals as they both have 3 floats as values

public:
	ResourceLoader();
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
	Mesh* getMesh();
	
};

#endif