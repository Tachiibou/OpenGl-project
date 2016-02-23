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

public:
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
	Mesh* getMesh();
};

#endif