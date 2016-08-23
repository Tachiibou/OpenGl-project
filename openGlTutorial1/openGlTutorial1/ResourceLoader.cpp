#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
	this->fileName = this->OBJ_DIR + "box.obj";
	this->vertexArray = nullptr;
	this->vertexInfoArray = nullptr;
	this->triangleVert = nullptr;
	this->indexArr = nullptr;
}

ResourceLoader::ResourceLoader(std::string fileName)
{
	this->fileName = fileName;
	this->vertexArray = nullptr;
	this->vertexInfoArray = nullptr;
	this->triangleVert = nullptr;
	this->indexArr = nullptr;
	loadMesh();
}


ResourceLoader::~ResourceLoader()
{
	if(this->vertexArray != nullptr)
		delete[] this->vertexArray;

	if (this->vertexInfoArray != nullptr)
		delete[] this->vertexInfoArray;

	if (this->triangleVert != nullptr)
		delete[] this->triangleVert;

	if (this->indexArr != nullptr)
		delete[] this->indexArr;

	if (this->texture != nullptr)
		delete texture;
}

void ResourceLoader::loadMesh() {
	std::string line, mtlFileName, textureFileName;
	std::ifstream myfile(this->fileName);
	std::vector<glm::vec2> UVector;
	std::vector<glm::vec3> normalVector;
	std::vector<VertexInfo> vertexInfoVector;
	texture = nullptr;

	vertexAmount = 0;
	indexAmount = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line)) // looping through obj file
		{

			if (line.substr(0, 2) == "v ") //vertex
				this->insertVertex(line, vertexVector); // insert vertex from line into vector


			else if (line.substr(0, 2) == "vt") // UV
				this->insertUV(line, UVector);


			else if (line.substr(0, 2) == "vn") // normal
				this->insertNormal(line, normalVector);


			else if (line.substr(0, 2) == "f ") //face
				this->createVerticesFromLine(line, vertexVector, normalVector, UVector, vertexInfoVector);

			else if (line.substr(0, 2) == "mt") // find MTL filename
				mtlFileName = this->getSecondWord(line);
		}
		myfile.close();
	}

	if (!mtlFileName.empty()) // if we found a filename for MTL
	{
		std::ifstream mtlFile(this->OBJ_DIR + mtlFileName);

		while (getline(mtlFile, line))  // looping through MTL file
		{
			if (line.substr(0, 2) == "ma") // find texture filename "map_kd"
				textureFileName = this->getSecondWord(line);
			if (line.substr(0, 2) == "il") // illum
				illum = std::stoi(this->getSecondWord(line));
			if (line.substr(0, 2) == "Kd") // Kd	diffuse reflectance
				this->threeFloatIntoVariable(line, this->kd);
			if (line.substr(0, 2) == "Ka") // Ka	ambient reflectance
				this->threeFloatIntoVariable(line, this->ka);
			if (line.substr(0, 2) == "Tf") // Tf	transmission filter
				this->threeFloatIntoVariable(line, this->tf);
			if (line.substr(0, 2) == "Ni") // Ni optical_density This is also known as index of refraction
				this->ni = std::stof(this->getSecondWord(line));
		}
	}

	if (!textureFileName.empty())
	{
		textureFileName = this->OBJ_DIR + textureFileName;
		texture = new Texture(textureFileName.c_str());
	}

	if (vertexArray != nullptr) {
		delete this->vertexArray;
		delete this->vertexInfoArray;
		delete this->triangleVert;
		delete this->indexArr;
	}

	indexAmount = vertexInfoVector.size();
	vertexAmount = vertexInfoVector.size();
	this->indexArr = this->getIndexArr(indexAmount);



	this->vertexArray = this->createVertices(vertexVector);
	this->vertexInfoArray = this->VertexInfoVectorToArray(vertexInfoVector);

	this->triangleVert = this->makeStruct(vertexInfoVector);
}

Mesh* ResourceLoader::getMesh()
{
	Mesh* mesh = new Mesh(vertexAmount, indexArr, indexAmount, this->triangleVert, texture);
	mesh->createBoundingBox(getMeshHalfSize(&vertexVector));
	return mesh;
}

float ResourceLoader::getMeshHalfSize(std::vector<glm::vec3> *vertexVector) {
	float maxX = FLT_MIN;
	float minX = FLT_MAX;
	float maxZ = FLT_MIN;
	float minZ = FLT_MAX;

	glm::vec3 *vec;
	for (size_t i = 0; i < vertexVector->size(); i++)
	{
		vec = &vertexVector->at(i);

		if (vec->x>maxX)
			maxX = vec->x;
		if (vec->x < minX)
			minX = vec->x;
		if (vec->z > maxZ)
			maxZ = vec->z;
		if (vec->z < minZ)
			minZ = vec->z;


	}

	float x = maxX - minX;
	float z = maxZ - minZ;

	return (x>z) ? x / 2 : z / 2;

}

void ResourceLoader::printFile()
{
	std::string line;
	std::ifstream myfile(this->fileName);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::cout << line << '\n';
		}
		myfile.close();
	}
}

// convert VertexInfoVector into a pointer array
VertexInfo* ResourceLoader::VertexInfoVectorToArray(std::vector<VertexInfo>& vertecies)
{
	int size = vertecies.size();
	VertexInfo* vertexInfoArray = new VertexInfo[size];

	for (int i = 0; i < size; i++)
	{
		vertexInfoArray[i] = vertecies.at(i);
	}
	return vertexInfoArray;
}

//convert vector of vec3 into Vertex array
Vertex * ResourceLoader::createVertices(std::vector<glm::vec3> pos)
{
	int size = pos.size();
	Vertex* vertices = new Vertex[size];

	for (int i = 0; i < size; i++)
	{
		vertices[i] = Vertex(pos.at(i));
	}
	return vertices;
}

// Read line, create vertexInfo, insert into vertexInfoVector
void ResourceLoader::createVerticesFromLine(std::string& line, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& UVs, std::vector<VertexInfo>& vertexInfoVector)
{
	std::stringstream inputString, numberString;
	std::string temp, number;
	glm::vec3 vertexIndex, normalIndex, UVIndex;
	inputString.str(line);
	int index[3], k = 0;
	inputString >> temp; // scrap f

	for (int i = 0; i < 3; i++) // loop through the 3 sets of numbers
	{
		inputString >> temp;

		numberString.str(temp);
		while (std::getline(numberString, temp, '/')) // get numbers divided by / in index arr
		{
			index[k] = (std::stoi(temp) - 1);
			k++;
		}
		k = 0;
		numberString.clear();
		vertexInfoVector.push_back(VertexInfo(vertices.at(index[0]), UVs.at(index[1]), normals.at(index[2])));

	}
}

//create index array from 0-amount in numerical order: 1,2,3.....
int * ResourceLoader::getIndexArr(int amount)
{
	int* indexArr = new int[amount];
	for (int i = 0; i < amount; i++)
	{
		indexArr[i] = i;
	}
	return indexArr;
}

// get one line with vertices, should be formated as follows "v 1 2 3" where the numbers are vertex coorinates
// VertexVector is reference, and vertex will be added to that vector
void ResourceLoader::insertVertex(std::string line, std::vector<glm::vec3>& vertexVector)
{
	std::istringstream inputString;
	glm::vec3 vertexPos;
	std::string scrap; // scrap will contain the letter v

	inputString.str(line);

	inputString >> scrap >> vertexPos.x >> vertexPos.y >> vertexPos.z;
	vertexVector.push_back(vertexPos);
	/*

	ST�RSTA X
	MINSTA X
	ST�RSTA Z
	MINSTA Z
	TX = SX-MX = 2
	TZ = SZ-MZ = 6

	return (TX>TZ)?TX/2:TZ/2;

	SX = 2
	MX = 0
	SZ = 4
	MZ = -2
	*/
}

//insert UV coordinates from line into UVector&
void ResourceLoader::insertUV(std::string line, std::vector<glm::vec2>& UVector)
{
	std::istringstream inputString;
	glm::vec2 UVPos;
	std::string scrap; // scrap will contain the letter vt

	inputString.str(line);

	inputString >> scrap >> UVPos.x >> UVPos.y;
	UVector.push_back(-UVPos); // had to invert UV ?
}

//insert normal coordinates from line into normalVector&
void ResourceLoader::insertNormal(std::string line, std::vector<glm::vec3>& normalVector)
{
	std::istringstream inputString;
	glm::vec3 normal;
	std::string scrap; // scrap will contain the letter vn

	inputString.str(line);

	inputString >> scrap >> normal.x >> normal.y >> normal.z;
	normalVector.push_back(normal);
}

// Removes first value and inserts the three following into the vec3 variable
void ResourceLoader::threeFloatIntoVariable(std::string line, glm::vec3& variable)
{
	std::istringstream inputString;
	std::string scrap; // scrap will contain the first word

	inputString.str(line);

	inputString >> scrap >> variable.x >> variable.y >> variable.z;
}

// simply returns the second word in the sentence. Works for returning texture name and mtl name
std::string ResourceLoader::getSecondWord(std::string line)
{
	std::istringstream inputString;
	std::string returnString, scrap;

	inputString.str(line);

	inputString >> scrap >> returnString;
	return returnString;
}

// Convert vector of VertexInfo into a TriangleVertex* struct
TriangleVertex * ResourceLoader::makeStruct(std::vector<VertexInfo> vertexInfo)
{
	TriangleVertex* tv = new TriangleVertex[vertexInfo.size()];
	for (int i = 0; i < vertexInfo.size(); i++)
	{
		tv[i] = { vertexInfo.at(i).pos.x,vertexInfo.at(i).pos.y,vertexInfo.at(i).pos.z,
			vertexInfo.at(i).UV.x,vertexInfo.at(i).UV.y,
			vertexInfo.at(i).normal.x,vertexInfo.at(i).normal.y,vertexInfo.at(i).normal.z};
	}
	return tv;
}
