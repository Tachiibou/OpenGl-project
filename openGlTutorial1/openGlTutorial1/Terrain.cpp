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
	delete mesh;
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
				unsigned char pixel_color = height_data[(x * t_width + y)];
				if (x ==15)
					int k = 0;
				float h = maxHeight* (pixel_color/255.f);
				setHeightAt(x, y, h);
			}
		}
		stbi_image_free(height_data);
		
		setNormals();
		calculateVertexInfo();
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
	glm::vec3** tempNormals = new glm::vec3*[width];
	for (int i = 0; i < this->width; i++) {
		tempNormals[i] = new glm::vec3[length];
	}


	//Making a line between the current point and the points that's adjecent. When there's a potential 4 lines, we cross product
	//Between thoose circular like a triangle.
	glm::vec3 tempZOut, tempZIn, tempXLeft, tempXRight, tempNormalSum;
	for (int x = 0; x < this->width; x++) {
		for (int z = 0; z < this->length; z++) {

			if (z > 0)
				tempZOut = glm::vec3(0.f, this->heights[x][z - 1] - this->heights[x][z], -1.f);

			if (z < this->length -1)
				tempZIn = glm::vec3(0.f, this->heights[x][z + 1] - this->heights[x][z], 1.f);

			if (x < this->width-1)
				tempXRight = glm::vec3(1.f, this->heights[x+1][z] - this->heights[x][z], 0.f);

			if (x > 0)
				tempXLeft = glm::vec3(-1.f, this->heights[x-1][z] - this->heights[x][z], 0.f);

			if (z > 0 && x > 0) {
				tempNormalSum += glm::cross(tempZOut, tempXLeft);
			}

			if (x > 0 && z < this->length - 1) {
				tempNormalSum += glm::cross(tempXLeft, tempZIn);
			}

			if (z < this->length - 1 && x < this->width - 1) {
				tempNormalSum += glm::cross(tempZIn, tempXRight);
			}

			if (x < this->width - 1 && z > 0) {
				tempNormalSum += glm::cross(tempXRight, tempZOut);
			}

			this->normals[x][z] = glm::normalize(tempNormalSum);
		}
	}



}

void Terrain::calculateVertexInfo() {
	VertexInfo* tempVertexInfo = new VertexInfo[this->width*this->length];;
	TriangleVertex* tempTriangleVertex = new TriangleVertex[this->width*this->length];
	glm::vec3 tempNormal;
	int cIndex;

	std::vector<int> indices;
	for (int x = 0; x < this->width-1; x++) {

		for (int z = 0; z < this->length-1; z++) {
			cIndex = x*this->length + z;

			indices.push_back(x*this->length + z);
			indices.push_back(x*this->length + z+1);
			indices.push_back((x+1)*this->length + z);

			tempNormal = this->normals[x][z];

			tempVertexInfo[cIndex] = VertexInfo(glm::vec3(x/5, this->heights[x][z], z/5), glm::vec2(0, 0), glm::vec3(tempNormal.x, tempNormal.y, tempNormal.z));
			 
			tempTriangleVertex[cIndex] = { tempVertexInfo[cIndex].pos.x, tempVertexInfo[cIndex].pos.y, tempVertexInfo[cIndex].pos.z,
				0.f, 0.f,
				tempVertexInfo[cIndex].normal.x, tempVertexInfo[cIndex].normal.y, tempVertexInfo[cIndex].normal.z };

			//printToScreen(tempVertexInfo[cIndex]);
		}
	}
	
	int *tempArr = new int[indices.size()];
	for (int i = 0; i < indices.size(); i++) {
		tempArr[i] = indices.at(i);
	}
	this->mesh = new Mesh(tempVertexInfo, this->width*this->length, tempArr, indices.size(), tempTriangleVertex);
}

void Terrain::printToScreen(const VertexInfo& info) {
	std::cout <<"|X: " << info.pos.x << "|Y: " << info.pos.y << "|Z: " << info.pos.z << std::endl;
	std::cout << "|NX: " << info.normal.x << "|NY: " << info.normal.y << "|NZ: " << info.normal.z << std::endl<<"---------------------------------"<<std::endl;
}

Mesh* Terrain::getMesh() {
	return this->mesh;
}
