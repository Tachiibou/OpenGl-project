#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
	this->fileName = "obj/box.obj";
	this->vertexArray = nullptr;
}

ResourceLoader::ResourceLoader(std::string fileName)
{
	this->fileName = fileName;
	this->vertexArray = nullptr;
}


ResourceLoader::~ResourceLoader()
{
	if(this->vertexArray != nullptr)
		delete this->vertexArray;
}

Mesh* ResourceLoader::getMesh()
{
	std::string line;
	std::ifstream myfile(this->fileName);
	std::vector<glm::vec3> vertexVector;
	std::vector<glm::vec2> UVector;
	std::vector<glm::vec3> normalVector;

	int vertexAmount = 0, indexAmount = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			if (line.substr(0, 2) == "v ") //vertex
			{
				this->insertVertex(line,vertexVector); // insert vertex from line into vector
			}

			else if (line.substr(0, 2) == "vt") // UV
			{
				this->insertUV(line, UVector);
			}

			else if (line.substr(0, 2) == "vn") // normal
			{
				this->insertNormal(line, normalVector);
			}

			else if (line.substr(0, 2) == "f ") // normal
			{
				this->createVerticesFromLine(line,vertexVector,normalVector,UVector);
			}
		}
		myfile.close();
	}
	vertexAmount = vertexVector.size();
	indexAmount = 6;
	int indexArr[] = { 0,1,2,2,1,3 };
	//Vertex* vertices2 = VertexVectorToArray(vertexVector);

	this->vertexArray = this->createVertices(vertexVector);

	return new Mesh(this->vertexArray, vertexAmount, indexArr, indexAmount);
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

Vertex * ResourceLoader::VertexVectorToArray(std::vector<Vertex> vertecies)
{
	int size = vertecies.size();
	this->vertexArray = new Vertex[size];

	for (size_t i = 0; i < size; i++)
	{
		vertexArray[i] = vertecies.at(i);
	}
	return vertexArray;
}

Vertex * ResourceLoader::createVertices(std::vector<glm::vec3> pos)
{
	int size = pos.size();
	Vertex* vertices = new Vertex[size];

	for (size_t i = 0; i < size; i++)
	{
		vertices[i] = Vertex(pos.at(i));
	}
	return vertices;
}

Vertex * ResourceLoader::createVerticesFromLine(std::string line, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> UVs)
{
	std::stringstream inputString, numberString;
	std::string scrap, number, indexArr[3];
	glm::vec3 vertexIndex, normalIndex, UVIndex;
	inputString.str(line);
	inputString >> scrap; // scrap f

	for (int i = 0; i < 3; i++) // add all numbers to one string
	{
		inputString >> scrap;
		indexArr[i] += scrap;
	}


	return nullptr;
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

}

void ResourceLoader::insertUV(std::string line, std::vector<glm::vec2>& UVector)
{
	std::istringstream inputString;
	glm::vec2 vertexPos;
	std::string scrap; // scrap will contain the letter vt

	inputString.str(line);

	inputString >> scrap >> vertexPos.x >> vertexPos.y;
	UVector.push_back(vertexPos);
}

void ResourceLoader::insertNormal(std::string line, std::vector<glm::vec3>& normalVector)
{
	std::istringstream inputString;
	glm::vec3 normal;
	std::string scrap; // scrap will contain the letter vn

	inputString.str(line);

	inputString >> scrap >> normal.x >> normal.y >> normal.z;
	normalVector.push_back(normal);
}
