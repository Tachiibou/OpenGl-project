#include "QuadTree2.h"



QuadTree2::QuadTree2()
{
}


QuadTree2::~QuadTree2()
{
	node->deleteNodes();
	delete node;
}

void QuadTree2::createQuadTree(glm::vec3 pos, float halfSize, int depth) {
	this->node = new QuadNode(pos, halfSize, QuadPos::NUM);

	node->insertQuadNodes(depth);
}

void QuadTree2::addMesh(Mesh * mesh)
{
	node->addMesh(mesh);
}

std::vector<Mesh*> QuadTree2::getMeshes(ViewFrustum* frustum) {
	return node->getMeshesInFrustum(frustum);
}