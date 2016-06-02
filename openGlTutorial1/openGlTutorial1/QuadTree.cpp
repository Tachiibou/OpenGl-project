#include "QuadTree.h"

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

QuadTree::QuadTree(float halfSide, int depth, std::vector<Mesh*> meshes)
{
	for (int i = 0; i < NUM_QUADTREE_AREAS; i++)
	{
		this->qt[i] = nullptr;
	}
	this->halfSide = halfSide;
	this->depth = depth;
	this->meshes = meshes;
	
	glm::vec3 newCenter;
	float newHalfSide = this->halfSide * 0.5f;


	for (int i = 0; i < NUM_QUADTREE_AREAS; i++)
	{
		std::vector<Mesh*> temp;
	}
}

QuadTree::~QuadTree()
{

}

std::vector<Mesh*> QuadTree::getMeshesInFrustum(ViewFrustum * frustum)
{
	return std::vector<Mesh*>();
}
