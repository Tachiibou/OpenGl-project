#include "glm\glm.hpp"
#include <vector>
#include "Mesh.h"
#include <iostream>
#include "glm/ext.hpp"
#include "ViewFrustum.h"

class QuadTree2
{

private:
	enum QuadPos {
		UP_LEFT,
		UP_RIGHT,
		DOWN_LEFT,
		DOWN_RIGHT,
		NUM
	};
	struct QuadNode {
		glm::vec3 Pos;
		float halfSize;
		std::vector<Mesh*> meshes;
		QuadNode *children [QuadPos::NUM];
		QuadPos quadLocation; 

		QuadNode(glm::vec3 pos, float halfSize, QuadPos location) {
			this->quadLocation = location;
			this->Pos = pos;
			this->halfSize = halfSize;

			for (unsigned int i = 0; i < QuadPos::NUM; i++) {
				children[i] = nullptr;
			}
		}

		void deleteNodes() {
			if (children[UP_LEFT] != nullptr) {
				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					children[i]->deleteNodes();
					delete children[i];
				}
			}
			
			//for (unsigned int i = 0; i < QuadPos::NUM; i++)
			//{
			//	delete this;
			//}

			//delete[]children;
		}


		void insertQuadNodes(int depth) {
			//check if we're above the leafe node
			if (depth > 0) {
				//Creates four new quads, half the size of the current one, and places in the speciefied corner.
				children[UP_LEFT] = new QuadNode(glm::vec3(Pos.x -( halfSize / 2), Pos.y, Pos.z + (halfSize / 2)), this->halfSize / 2, QuadPos::UP_LEFT);
				children[UP_RIGHT] = new QuadNode(glm::vec3(Pos.x +( halfSize / 2), Pos.y, Pos.z + (halfSize / 2)), this->halfSize / 2, QuadPos::UP_RIGHT);
				children[DOWN_LEFT] = new QuadNode(glm::vec3(Pos.x -( halfSize / 2), Pos.y, Pos.z - (halfSize / 2)), this->halfSize / 2, QuadPos::DOWN_LEFT);
				children[DOWN_RIGHT] = new QuadNode(glm::vec3(Pos.x +( halfSize / 2), Pos.y, Pos.z - (halfSize / 2)), this->halfSize / 2, QuadPos::DOWN_RIGHT);
				
				//Creates four new nodes 
				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					children[i]->insertQuadNodes(depth - 1);
				}
				//delete[]children;
			}
		}

		//Adds a mesh to the quad tree
		void addMesh(Mesh* mesh) {
			if (children[UP_LEFT] != nullptr) { // if we have children

				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					//The current quad is allready intersecting, meaning we just have to check if it's children also is.
					if (children[i]->intersects(mesh->getBoundingBox())) {
						children[i]->addMesh(mesh);

					}
					//children[i]->insertQuadNodes(depth - 1);
				}
			}
			else {
				//This meaning that this leaf node intersects with the mesh, and will be added to it's list.
				this->meshes.push_back(mesh);
			}
			
		}

		//The intersection test with the meshes boundingbox and the quad
		bool intersects(BoundingBox box) {
			return (abs(Pos.x - box.pos.x)  < (halfSize + box.halfSize)) &&
				(abs(Pos.z - box.pos.z)  < (halfSize + box.halfSize));
		}

		//This intersection test checks wether a point is in our quad
		bool intersects(glm::vec3 pos) {
			return (abs(this->Pos.x - pos.x)  < (halfSize)) &&
				(abs(this->Pos.z - pos.z)  < (halfSize));
		}

		//This function will return all meshes found in the quadtree
		std::vector<Mesh*> getMeshesInFrustum(ViewFrustum* frustum) {
			std::vector<Mesh*> returnMeshes;
			if (children[UP_LEFT] != nullptr) { // if we have children

				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					//Checks if any quad corner/children is inside frustum OR if frustum is inside quad
					if (frustum->quadeNodeInsFrustum(children[i]->Pos, children[i]->halfSize) || children[i]->intersects(frustum->getFrustumPos())) {
						std::vector<Mesh*> tempMeshes  = children[i]->getMeshesInFrustum(frustum);

						//Check and dont add copies to the meshvector
						for (size_t i = 0; i < tempMeshes.size(); i++)
						{
							bool exist = false;
							for (size_t k = 0; k < returnMeshes.size(); k++)
							{
								if (tempMeshes.at(i) == returnMeshes.at(k))
									exist = true;
							}

							if (!exist)
								returnMeshes.push_back(tempMeshes.at(i));
						}
					}
					//children[i]->insertQuadNodes(depth - 1);
				}
			}
			else {
				// leafs return this
				return this->meshes;
			}
			//parents return this
			return returnMeshes;
		}
	};
	QuadNode* node;
public:
	QuadTree2();
	~QuadTree2();

	void createQuadTree(glm::vec3 pos, float halfSize, int depth);
	void addMesh(Mesh* mesh);

	std::vector<Mesh*> getMeshes(ViewFrustum* frustum);
};

