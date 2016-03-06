#ifndef TERRAIN_H
#define TERRAIN_H

#include "glm\glm.hpp"
#include "GL\glew.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include "Mesh.h"
#include <vector>

class Terrain
{
private:
	int width;
	int length;
	float** heights;
	glm::vec3**normals;
	float scaleSize;
	void setHeightAt(int x, int z, float y);
	float getHeightAt(int x, int z);
	glm::vec3 getNormalAt(int x, int z);
	void calculateVertexInfo();
	Mesh* mesh;

	void smoothTerrain();
	void setNormals();
public:
	Terrain();
	void loadTerrain(const char*fileName, float maxHeight);
	~Terrain();
	void Initialize(int width, int length);
	Mesh*getMesh();
};

#endif