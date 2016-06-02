#include "QuadTree.h"
#include <iostream>

bool QuadTree::checkIfMeshIsIn(glm::vec3 center, float halfSide, Mesh* mesh)
{
	glm::vec3 meshPos = mesh->getPos();

	if (meshPos.x >= this->center.x && meshPos.x >= this->center.x + halfSide &&
		meshPos.y >= this->center.y && meshPos.y >= this->center.y + halfSide &&
		meshPos.z >= this->center.z && meshPos.z >= this->center.z + halfSide)
		return true;
	return false;
}

QuadTree::QuadTree()
{

}

QuadTree::QuadTree(glm::vec3 center, float halfSide, int depth, std::vector<Mesh*> meshes)
{
	for (int i = 0; i < NUM_QUADTREE_AREAS; i++)
		this->qt[i] = nullptr;

	this->center = center;
	this->halfSide = halfSide;
	this->depth = depth;
	this->meshes = meshes;
	//
	//if (depth < 4)
	//{
	//	std::vector<Mesh*> temp;
	//	glm::vec3 newCenter = this->center;
	//	float newHalfSide = this->halfSide * 0.5f;

	//	// UP_LEFT
	//	newCenter.x -= newHalfSide;
	//	newCenter.z += newHalfSide;
	//	for (size_t i = 0; i < this->meshes.size(); i++)
	//	{
	//		if (checkIfMeshIsIn(newCenter, newHalfSide, this->meshes[i]))
	//			temp.push_back(this->meshes[i]);
	//	}
	//	this->qt[UP_LEFT] = new QuadTree(newCenter, newHalfSide, this->depth + 1, temp);
	//	std::cout << "UP_LEFT: X: " << newCenter.x << " Z: " << newCenter.z << " nrOfMeshes: " << temp.size() << std::endl;
	//	temp.clear();

	//	// UP_RIGHT
	//	glm::vec3 newCenter = this->center;
	//	newCenter.x += newHalfSide;
	//	newCenter.z += newHalfSide;
	//	for (size_t i = 0; i < this->meshes.size(); i++)
	//	{
	//		if (checkIfMeshIsIn(newCenter, newHalfSide, this->meshes[i]))
	//			temp.push_back(this->meshes[i]);
	//	}
	//	this->qt[UP_RIGHT] = new QuadTree(newCenter, newHalfSide, this->depth + 1, temp);
	//	std::cout << "UP_RIGHT: X: " << newCenter.x << " Z: " << newCenter.z << " nrOfMeshes: " << temp.size() << std::endl;
	//	temp.clear();

	//	// DOWN_LEFT
	//	glm::vec3 newCenter = this->center;
	//	newCenter.x -= newHalfSide;
	//	newCenter.z -= newHalfSide;
	//	for (size_t i = 0; i < this->meshes.size(); i++)
	//	{
	//		if (checkIfMeshIsIn(newCenter, newHalfSide, this->meshes[i]))
	//			temp.push_back(this->meshes[i]);
	//	}
	//	this->qt[DOWN_LEFT] = new QuadTree(newCenter, newHalfSide, this->depth + 1, temp);
	//	std::cout << "DOWN_LEFT: X: " << newCenter.x << " Z: " << newCenter.z << " nrOfMeshes: " << temp.size() << std::endl;
	//	temp.clear();

	//	// DOWN_RIGHT
	//	glm::vec3 newCenter = this->center;
	//	newCenter.x += newHalfSide;
	//	newCenter.z -= newHalfSide;
	//	for (size_t i = 0; i < this->meshes.size(); i++)
	//	{
	//		if (checkIfMeshIsIn(newCenter, newHalfSide, this->meshes[i]))
	//			temp.push_back(this->meshes[i]);
	//	}
	//	this->qt[DOWN_RIGHT] = new QuadTree(newCenter, newHalfSide, this->depth + 1, temp);
	//	std::cout << "DOWN_RIGHT: X: " << newCenter.x << " Z: " << newCenter.z << " nrOfMeshes: " << temp.size() << std::endl;
	//	temp.clear();
	//}
}

QuadTree::~QuadTree()
{

}

std::vector<Mesh*> QuadTree::getMeshesInFrustum(ViewFrustum * frustum)
{
	return std::vector<Mesh*>();
}
