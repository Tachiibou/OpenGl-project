#include "Terrain.h"



Terrain::Terrain()
{
	this->heights = 0;
	this->width = 0;
	
}


Terrain::~Terrain()
{
	
	for (int i = 0; i < this->length; i++) {
		delete this->heights[i];
	}
	delete[]this->heights;
	
	for (int i = 0; i < this->length; i++) {
		delete this->normals[i];
	}
	delete[]this->normals;
}

void Terrain::Initialize(int width, int length) {
	this->length = length;
	this->width = width;

	this->heights = new float*[width];
	for (int i = 0; i < this->length; i++) {
		this->heights[i] = new float[length];
	}

	this->normals = new glm::vec3*[width];
	for (int i = 0; i < this->length; i++) {
		this->normals[i] = new glm::vec3[length];
	}
}


void Terrain::loadTerrain(const char*fileName, float maxHeight) {
	int t_width, t_height, t_numComponent;
	unsigned char* height_data = stbi_load(fileName, &t_width, &t_height, &t_numComponent, 4);
	
	if (height_data == NULL) {
		int k = 0;
		//ERROR!!!
	}
	else {

		Initialize(t_width, t_height);

		for (int x = 0; x < t_width; x++) {
			for (int y = 0; y < t_height; y++) {
				//Get 
				unsigned char pixel_color =
					height_data[(y * t_width + x)];
				

				float h = maxHeight* (pixel_color/255.f);
				setHeightAt(x, y, h);
			}
		}
		stbi_image_free(height_data);
		
		setNormals();
	}

}

void Terrain::setHeightAt(int x, int z, float y) {
	this->heights[x][z] = y;
}

float Terrain::getHeightAt(int x, int z) {
	return this->heights[x][z];
}

glm::vec3 Terrain::getNormalAt(int x, int z) {
	return this->normals[x][z];
}

void Terrain::setNormals(){

}