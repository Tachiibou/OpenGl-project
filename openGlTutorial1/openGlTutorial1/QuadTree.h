#pragma once

#include "ViewFrustum.h"
#include <vector>
#include "Mesh.h"

class QuadTree
{
private:
	enum QuadTreeAreas
	{
		UP_LEFT,
		UP_RIGHT,
		DOWN_LEFT,
		DOWN_RIGHT,
		NUM_QUADTREE_AREAS
	};
	
	glm::vec3 center;
	float halfSide;	// Distance to the sie from center
	int depth;
	std::vector<Mesh*> meshes;
	QuadTree* qt[NUM_QUADTREE_AREAS];
	
	bool checkIfMeshIsIn(glm::vec3 center, float halfSide, Mesh* mesh);
	
public:
	QuadTree();
	QuadTree(float halfSide, int depth, std::vector<Mesh*> meshes);
	virtual ~QuadTree();
	std::vector<Mesh*> getMeshesInFrustum(ViewFrustum* frustum);
};

