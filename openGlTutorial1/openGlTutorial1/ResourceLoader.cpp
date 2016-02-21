#include "ResourceLoader.h"



ResourceLoader::ResourceLoader(std::string fileName)
{
	this->fileName = fileName;
}


ResourceLoader::~ResourceLoader()
{
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
