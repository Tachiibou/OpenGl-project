#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include<string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"

// Vertex, UV and normal
class VertexInfo {
public:
	VertexInfo(const glm::vec3&pos, const glm::vec2& UV, const glm::vec3& normal) {
		this->pos = pos;
		this->normal = normal;
		this->UV = UV;
	}
	VertexInfo() {
		this->pos = glm::vec3(0, 0, 0);
		this->normal = glm::vec3(0, 0, 0);
		this->UV = glm::vec2(0, 0);
	}
protected:
public:
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 UV;

};


class ResourceLoader
{
private:
	std::string fileName;
	Vertex* vertexArray;
	VertexInfo* vertexInfoArray;
	TriangleVertex* triangleVert;
	int* indexArr;
	const std::string OBJ_DIR = "obj/";
	glm::vec3 kd, ka, tf;
	int illum;
	float ni;

	VertexInfo* VertexInfoVectorToArray(std::vector<VertexInfo>& vertexInfo);
	Vertex* createVertices(std::vector<glm::vec3> pos);

	void createVerticesFromLine(std::string& line, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& UVs, std::vector<VertexInfo>& vertexInfoVector);
	int* getIndexArr(int amount);
	
	void insertVertex(std::string line, std::vector<glm::vec3>& vertexVector);
	void insertUV(std::string line, std::vector<glm::vec2>& UVector);
	void insertNormal(std::string line, std::vector<glm::vec3>& normalVector);
	void threeFloatIntoVariable(std::string line, glm::vec3& variable);
	std::string getSecondWord(std::string line);

	TriangleVertex* makeStruct(std::vector<VertexInfo> vertexInfo);

	float getMeshHalfSize(std::vector<glm::vec3> *vertexVector);

public:
	ResourceLoader();
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
	Mesh* getMesh();
	
};

#endif