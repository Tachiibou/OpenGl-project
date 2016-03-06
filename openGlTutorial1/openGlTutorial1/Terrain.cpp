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


void Terrain::loadTerrain(const char*fileName, float maxHeight) {
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
				int index = 3 * (y * t_width + x);
				
				unsigned char pixel_color = (unsigned char)height_data[index];
				if (x==150 && y == 150)
					int k = 0;
				float h = maxHeight* (pixel_color/255.f);
				setHeightAt(x, y, h);
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

			this->normals[x][z] = tempNormalSum;
		}
	}

	for (int i = 0; i < this->width; i++) {
		delete tempNormals[i];
	}
	delete[] tempNormals;


}

void Terrain::calculateVertexInfo() {
	TriangleVertex* tempTriangleVertex = new TriangleVertex[this->width*this->length];
	
	glm::vec3 tempNormal;
	int cIndex;

	std::vector<int> indices;
	float positionX, positionZ;
	float u, v;
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
			

			tempNormal = this->normals[x][z];

			u = x / (float)this->width;
			v = 1- (z / (float)this->length);

			positionX = x*this->scaleSize;
			positionZ = z*this->scaleSize;
			 
			tempTriangleVertex[cIndex] = { positionX, this->heights[x][z], positionZ,
				u, v,
				tempNormal.x, tempNormal.y, tempNormal.z };

			//printToScreen(tempVertexInfo[cIndex]);
			//std::cout << cIndex << std::endl;
		}
	}

	
	int *tempArr = new int[indices.size()];
	for (int i = 0; i < indices.size(); i++) {
		tempArr[i] = indices.at(i);
	}
	this->mesh = new Mesh(NULL, this->width*this->length, tempArr, indices.size(), tempTriangleVertex);

	delete[]tempTriangleVertex;
	delete[] tempArr;
}

void Terrain::printToScreen(const VertexInfo& info) {
	std::cout <<"|X: " << info.pos.x << "|Y: " << info.pos.y << "|Z: " << info.pos.z << std::endl;
	std::cout << "|NX: " << info.normal.x << "|NY: " << info.normal.y << "|NZ: " << info.normal.z << std::endl<<"---------------------------------"<<std::endl;
}

Mesh* Terrain::getMesh() {
	return this->mesh;
}
