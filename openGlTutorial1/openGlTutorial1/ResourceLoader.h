#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include<string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Mesh.h"

class ResourceLoader
{
private:
	std::string fileName;

public:
	ResourceLoader(std::string fileName);
	void printFile();
	~ResourceLoader();
};

#endif