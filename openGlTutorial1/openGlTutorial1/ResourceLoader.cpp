#include "ResourceLoader.h"

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
	std::vector<Vertex> vertexVector;

	int vertexAmount = 0, indexAmount = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			if (line.substr(0, 2) == "v ") 
			{
				this->insertVertex(line,vertexVector); // insert vertex from one line into vector
			}
		}
		myfile.close();
	}
	vertexAmount = vertexVector.size();
	indexAmount = 6;
	int indexArr[] = { 0,1,2,2,1,3 };
	Vertex* vertices2 = VertexVectorToArray(vertexVector);

	return new Mesh(vertices2, vertexAmount, indexArr, indexAmount);
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

// get one line with vertices, should be formated as follows "v 1 2 3" where the numbers are vertex coorinates
// VertexVector is reference, and vertex will be added to that vector
void ResourceLoader::insertVertex(std::string line, std::vector<Vertex>& vertexVector) 
{
	std::istringstream inputString;
	glm::vec3 vertexPos;
	std::string scrap; // scrap will contain the letter V

	inputString.str(line);

	inputString >> scrap >> vertexPos.x >> vertexPos.y >> vertexPos.z;
	vertexVector.push_back(Vertex(vertexPos));

}
