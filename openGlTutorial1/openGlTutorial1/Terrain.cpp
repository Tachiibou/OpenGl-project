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
	position = glm::vec3(0);
	this->length = length;
	this->width = width;
	scaleSize = .4f;

	this->heights = new float*[width];
	for (int i = 0; i < this->length; i++) {
		this->heights[i] = new float[length];
	}

	this->normals = new glm::vec3*[width];
	for (int i = 0; i < this->length; i++) {
		this->normals[i] = new glm::vec3[length];
	}
}


void Terrain::loadTerrain(const char*fileName, float terrainScaler) {
	int t_width, t_height, t_numComponent;
	unsigned char* height_data = stbi_load(fileName, &t_width, &t_height, &t_numComponent, 0);
	
	if (height_data == NULL) {
		int k = 0;
		//ERROR!!!
	}
	else {
		Initialize(t_width, t_height);
		for (int y = 0; y < t_height; y++) {
			for (int x = 0; x < t_width; x++) {
				//Takes the Blue Color (R, G, (B))

				int nrAdded = 0;
				unsigned int total = 0;
				for (int yt = -2; yt < 3; yt++) {
					for (int xt = -2; xt < 3; xt++) {   


						if ((yt + y >= 0 && xt + x>=0) && (yt + y < t_height && xt + x < t_width)) {
							int index = 3 * (yt + y * t_width + x + xt);
							total += (unsigned char)height_data[index];
							nrAdded++;
						}

					}
				}

				total /= nrAdded;

				//int index = 3 * (y * t_width + x);
				//
				//unsigned char pixel_color = (unsigned char)height_data[index];
				//if (x==100 && y == 200)
				//	int k = 0;
				//float h = (pixel_color);

				if (y == t_height - 1) {
					if (x > t_width - 10) {
						int k = 0;
					}
				}

				setHeightAt(x, y, total * terrainScaler);
			}
		}
		stbi_image_free(height_data);
		
		setNormals();
		calculateVertexInfo();
		//smoothTerrain();
	}


}

void Terrain::smoothTerrain() {
	const float ratio = .5f;
	for (int x = 0; x < this->width; x++) {
		for (int z = 0; z < this->length; z++) {
			glm::vec3 n_total = this->normals[x][z];

			if (x > 0) {
				n_total += this->normals[z][x - 1] * ratio;
			}
			if (x < this->width - 1) {
				n_total += this->normals[z][x + 1] * ratio;
			}
			if (z > 0) {
				n_total += this->normals[z - 1][x] * ratio;
			}
			if (z < this->length - 1) {
				n_total += this->normals[z + 1][x];
			}

			if (n_total.length() == 0) {
				n_total= glm::vec3(0.0f, 1.0f, 0.0f);
			}
			normals[z][x] = n_total;

		}
	}
}

void Terrain::setHeightAt(int x, int z, float y) {
	this->heights[x][z] = 50 * (y/255);
}

float Terrain::getHeightAt(int x, int z) {
	return this->heights[x][z];
}

glm::vec3 Terrain::getNormalAt(int x, int z) {
	return this->normals[x][z];
}

void Terrain::setNormals(){
	

	//Making a line between the current point and the points that's adjecent. When there's a potential 4 lines, we cross product
	//Between thoose circular like a triangle.
	glm::vec3 tempZOut, tempZIn, tempXLeft, tempXRight, tempNormalSum;
	glm::vec3 currentPos;
	for (int x = 0; x < this->width; x++) {
		
		for (int z = 0; z < this->length; z++) {

			currentPos = glm::vec3(x, getHeightAt(x, z), z);

			int k = getHeightAt(x, z);
			tempNormalSum = glm::vec3();

			if (z > 0)
				tempZOut = glm::vec3(x, getHeightAt(x,z-1), z-1) -currentPos;
				//tempZOut = glm::vec3(0.f, this->heights[x][z - 1] - this->heights[x][z], -1.f);

			if (z < this->length - 1)
				tempZIn = glm::vec3(x, getHeightAt(x, z + 1), z + 1) - currentPos;
				//tempZIn = glm::vec3(0.f, this->heights[x][z + 1] - this->heights[x][z], 1.f);

			if (x < this->width-1)
				tempXRight =glm::vec3(x+1, getHeightAt(x+1, z), z) - currentPos;
				//tempXRight = glm::vec3(1.f, this->heights[x+1][z] - this->heights[x][z], 0.f);

			if (x > 0)
				tempXLeft = glm::vec3(x-1, getHeightAt(x-1, z),z) - currentPos;
				//tempXLeft = glm::vec3(-1.f, this->heights[x-1][z] - this->heights[x][z], 0.f);

			if (z > 0 && x > 0) {
				tempNormalSum += glm::normalize(glm::cross(tempZOut, tempXLeft));
			}

			if (x > 0 && z < this->length - 1) {
				tempNormalSum += glm::normalize(glm::cross(tempXLeft, tempZIn));
			}

			if (z < this->length - 1 && x < this->width - 1) {
				tempNormalSum += glm::normalize(glm::cross(tempZIn, tempXRight));
			}

			if (x < this->width - 1 && z > 0) {
				tempNormalSum += glm::normalize(glm::cross(tempXRight, tempZOut));
			}


			//int k = getHeightAt(x, z);
			normals[x][z] = glm::normalize(tempNormalSum);
			//std::cout<<"Height: " << k << " | X: " << this->normals[x][z].x << " | Y: " << this->normals[x][z].y << " | Z: " << this->normals[x][z].z << std::endl;

		}
	}



}

void Terrain::calculateVertexInfo() {
	TriangleVertex* tempTriangleVertex = new TriangleVertex[this->width*this->length];
	
	glm::vec3 tempNormal;
	int cIndex;
	std::vector<int> indices;
	float positionX, positionZ;
	float u, v;
	float maxHeight = 0;
	float* heights = new float[this->width*this->length];
	for (int x = 0; x < this->width; x++) {

		for (int z = 0; z < this->length; z++) {
			cIndex = x* this->length + z;
			if (x + 1 != this->width && z + 1 != this->length) {
				indices.push_back(x* this->length + z);
				indices.push_back(x*  this->length + z + 1);
				indices.push_back((x + 1)*  this->length + z);

				indices.push_back((x + 1)*  this->length + z);
				indices.push_back(x*  this->length + z + 1);
				indices.push_back((x + 1) *  this->length + z + 1);

				
			}
			else {
				int k = 0;
			}

			tempNormal = this->getNormalAt(x, z);


			//u = x;// x / (float)this->width;
			//v = z;// 1- (z / (float)this->length);
			u = (x / (float)this->width) * 2;
			v = (1 - (z / (float)this->length)) * 2;

			positionX = x;
			positionZ = z;

			float cHeight = this->heights[x][z];

			tempTriangleVertex[cIndex] = { positionX, cHeight,positionZ,
				u,v,
				tempNormal.x, tempNormal.y, tempNormal.z };
			
			if (maxHeight < cHeight)
				maxHeight = cHeight;

			heights[cIndex] = cHeight;

			//printToScreen(tempVertexInfo[cIndex]);
			//std::cout << cIndex << std::endl;
		}
	}

	for (unsigned int i = 0; i < this->width*this->length; i++) {
		heights[i] = heights[i] / maxHeight;
	}
		
	
	int *tempArr = new int[indices.size()];
	for (int i = 0; i < indices.size(); i++) {
		tempArr[i] = indices.at(i);
	}

	Texture* terrainTexture = new Texture();
	terrainTexture->addTexture("./obj/brick_16.jpg");
	terrainTexture->addTexture("./obj/Snow.jpg");

	//terrainTexture->addTexture("./obj/Stone.jpg");
	//terrainTexture->addTexture("./obj/Snow.jpg");
	this->mesh = new Mesh(this->width*this->length, tempArr, indices.size(), tempTriangleVertex, terrainTexture, heights);
	delete[]tempTriangleVertex;
	delete[] tempArr;
	delete[] heights;
}
Mesh* Terrain::getMesh() {
	return this->mesh;
}

int Terrain::getWidth() {
	return this->width;
}
int Terrain::getLength() {
	return this->length;
}

glm::vec3 Terrain::getPos() {
	return this->position;
}