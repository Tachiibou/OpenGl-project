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
	
	void insertVertex(std::string line, std::vector<Vertex>& vertexVector);
	void insertUV(std::string line, std::vector<UV>& UVector);
	void insertNormal(std::string line, std::vector<Vertex>& normalVector); // Vertex used for normals as they both have 3 floats as values

public:
	ResourceLoader();
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
	Mesh* getMesh();
	
};

#endif