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
	VertexInfoStruct* triangleVert;
	int* indexArr;
	const std::string OBJ_DIR = "obj/";
	glm::vec3 kd, ka, tf, ks;	// Material values
	int illum;					// Material values
	float ni;					// Material values
	int vertexAmount, indexAmount;
	Texture* texture;
	std::vector<glm::vec3> vertexVector;
	float halfSize; // used for bounding box


	void createVerticesFromLine(std::string& line, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& UVs, std::vector<VertexInfo>& vertexInfoVector);
	int* getIndexArr(int amount);
	
	void insertVertex(std::string line, std::vector<glm::vec3>& vertexVector);
	void insertUV(std::string line, std::vector<glm::vec2>& UVector);
	void insertNormal(std::string line, std::vector<glm::vec3>& normalVector);
	void stringIntoVec3(std::string line, glm::vec3& variable);
	std::string getSecondWord(std::string line);

	VertexInfoStruct* makeStruct(std::vector<VertexInfo> vertexInfo);

	float getMeshHalfSize(std::vector<glm::vec3> *vertexVector);

public:
	ResourceLoader();
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
	Mesh* getMesh();
	void loadMesh();
	
};

#endif