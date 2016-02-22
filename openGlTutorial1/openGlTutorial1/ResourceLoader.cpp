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
	std::string line, scrap;
	std::ifstream myfile(this->fileName);
	std::istringstream inputString;
	std::vector<Vertex> vertexVector;
	glm::vec3 vertexPos;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			inputString.str(line);

			if (line.substr(0, 2) == "v ")
			{
				inputString >> scrap >> vertexPos.x >> vertexPos.y >> vertexPos.z;
				vertexVector.push_back(Vertex(vertexPos));
			}
			inputString.clear();
		}
		myfile.close();
	}
	int derp[] = { 0,1,2,2,1,3 };
	int test = sizeof(vertexVector.front()) * vertexVector.size();
	//Vertex* derp = VertexVectorToArray(vertexVector);
	int test2 = sizeof(*VertexVectorToArray(vertexVector)) / sizeof(vertexVector.front());
	Vertex* vertices2 = VertexVectorToArray(vertexVector);

	return new Mesh(vertices2, 4, derp, 6);
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
