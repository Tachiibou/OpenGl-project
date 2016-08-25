#include "QuadTree2.h"



QuadTree2::QuadTree2()
{
}


QuadTree2::~QuadTree2()
{
	node->deleteNodes();
	delete node;
}

//Instantiates the quadtree, the pos is the location of the center, halfsize is the width/length of the biggest square
//Depth is how many time we want to creater inner squares
void QuadTree2::createQuadTree(glm::vec3 pos, float halfSize, int depth) {
	this->node = new QuadNode(pos, halfSize, QuadPos::NUM);

	node->insertQuadNodes(depth);
}

//Adds a mesh to the quad tree, meaning that each leaf will have it's own list of meshes that is inside that square
void QuadTree2::addMesh(Mesh * mesh)
{
	node->addMesh(mesh);
}

//Returns all meshes thats withing the frustum
std::vector<Mesh*> QuadTree2::getMeshes(ViewFrustum* frustum) {
	return node->getMeshesInFrustum(frustum);
}