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

//After reading the width and height of the heightmap, this function will be called
//Initializing all the necisary values (allocating memory)
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

//This will load the terrain, call initializon
void Terrain::loadTerrain(const char*fileName, float terrainScaler) {
	int t_width, t_height, t_numComponent;
	unsigned char* height_data = stbi_load(fileName, &t_width, &t_height, &t_numComponent, 0);
	
	if (height_data == NULL) {
		std::cout << "Couldnt load terrain";
	}
	else {
		Initialize(t_width, t_height);
		//this will loop every pixel in the heightmap
		for (int y = 0; y < t_height; y++) {
			for (int x = 0; x < t_width; x++) {
				//Takes the Blue Color (R, G, (B))

				int nrAdded = 0;
				unsigned int total = 0;
				for (int yt = -2; yt < 3; yt++) {
					for (int xt = -2; xt < 3; xt++) {   

						//this will check weither we're reading inside the texture( example, we cant read -1x)
						if ((yt + y >= 0 && xt + x>=0) && (yt + y < t_height && xt + x < t_width)) {
							//makes the terrain from 2d -> 1d
							int index = 3 * (yt + y * t_width + x + xt);
							total += (unsigned char)height_data[index];
							nrAdded++;
						}

					}
				}
				//This will get the average height of all 9 squares checked
				total /= nrAdded;

				setHeightAt(x, y, total * terrainScaler);
			}
		}
		stbi_image_free(height_data);
		
		//Calculates the normals for the terrain
		setNormals();

		calculateVertexInfo();
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

//Set all the normals for the terrain
void Terrain::setNormals(){
	

	//Making a line between the current point and the points that's adjecent. When there's a potential 4 lines, we cross product
	//Between thoose circular like a triangle. Then we will take the average normal of up to four lines.
	//          |
	//          |
	//     -----.-----
	//			|
	//			|
	glm::vec3 tempZOut, tempZIn, tempXLeft, tempXRight, tempNormalSum;
	glm::vec3 currentPos;
	for (int x = 0; x < this->width; x++) {
		
		for (int z = 0; z < this->length; z++) {
			//the position of the current vertex we're checking for
			currentPos = glm::vec3(x, getHeightAt(x, z), z);

			tempNormalSum = glm::vec3();
			

			if (z > 0)
				tempZOut = glm::vec3(x, getHeightAt(x,z-1), z-1) -currentPos;

			if (z < this->length - 1)
				tempZIn = glm::vec3(x, getHeightAt(x, z + 1), z + 1) - currentPos;

			if (x < this->width-1)
				tempXRight =glm::vec3(x+1, getHeightAt(x+1, z), z) - currentPos;

			if (x > 0)
				tempXLeft = glm::vec3(x-1, getHeightAt(x-1, z),z) - currentPos;

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
			normals[x][z] = glm::normalize(tempNormalSum);

		}
	}



}

//This will create a mesh
//Meaning that it will load textures, calculate indices(For it to be a square at a time
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
				//the down right corner of the square (Triangle)
				indices.push_back(x* this->length + z);
				indices.push_back(x*  this->length + z + 1);
				indices.push_back((x + 1)*  this->length + z);
				//the upper left corner of the square (Triangle)
				indices.push_back((x + 1)*  this->length + z);
				indices.push_back(x*  this->length + z + 1);
				indices.push_back((x + 1) *  this->length + z + 1);

				
			}

			tempNormal = this->getNormalAt(x, z);

			u = (x / (float)this->width) * 2;
			v = (1 - (z / (float)this->length)) * 2;

			positionX = x;
			positionZ = z;

			float cHeight = this->heights[x][z];

			tempTriangleVertex[cIndex] = { positionX, cHeight,positionZ,
				u,v,
				tempNormal.x, tempNormal.y, tempNormal.z };
			
			//this will make sure we will know the highest point of the terrain
			//Which is necisarry for the blendmapping to know
			if (maxHeight < cHeight)
				maxHeight = cHeight;

			heights[cIndex] = cHeight;

		}
	}

	//Makes the heights a percentage value between 0 -> 1
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