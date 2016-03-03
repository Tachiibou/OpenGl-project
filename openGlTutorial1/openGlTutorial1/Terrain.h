#ifndef TERRAIN_H
#define TERRAIN_H

#include "glm\glm.hpp"
#include "GL\glew.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include "Mesh.h"

class Terrain
{
private:
	int width;
	int length;
	float** heights;
	glm::vec3**normals;
	void setHeightAt(int x, int z, float y);
	float getHeightAt(int x, int z);
	glm::vec3 getNormalAt(int x, int z);

	void setNormals();
public:
	Terrain();
	void loadTerrain(const char*fileName, float maxHeight);
	~Terrain();
	void Initialize(int width, int length);

	void Draw();
};

#endif