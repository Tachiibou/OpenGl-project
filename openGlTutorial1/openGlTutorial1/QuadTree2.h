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
			std::cout<<"POS: " << glm::to_string(pos)<<" || SIZE: " << halfSize << std::endl;
			this->quadLocation = location;
			this->Pos = pos;
			this->halfSize = halfSize;

			for (unsigned int i = 0; i < QuadPos::NUM; i++) {
				children[i] = nullptr;
			}
		}

		void insertQuadNodes(int depth) {
			if (depth > 0) {
				children[UP_LEFT] = new QuadNode(glm::vec3(Pos.x -( halfSize / 2), Pos.y, Pos.z + (halfSize / 2)), this->halfSize / 2, QuadPos::UP_LEFT);
				children[UP_RIGHT] = new QuadNode(glm::vec3(Pos.x +( halfSize / 2), Pos.y, Pos.z + (halfSize / 2)), this->halfSize / 2, QuadPos::UP_RIGHT);
				children[DOWN_LEFT] = new QuadNode(glm::vec3(Pos.x -( halfSize / 2), Pos.y, Pos.z - (halfSize / 2)), this->halfSize / 2, QuadPos::DOWN_LEFT);
				children[DOWN_RIGHT] = new QuadNode(glm::vec3(Pos.x +( halfSize / 2), Pos.y, Pos.z - (halfSize / 2)), this->halfSize / 2, QuadPos::DOWN_RIGHT);
				
				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					children[i]->insertQuadNodes(depth - 1);
				}
				//delete[]children;
			}
		}

		void addMesh(Mesh* mesh) {
			if (children[UP_LEFT] != nullptr) {
				for (unsigned int i = 0; i < QuadPos::NUM; i++)
				{
					if (children[i]->intersects(mesh->getBoundingBox())) {
						children[i]->addMesh(mesh);

					}
					//children[i]->insertQuadNodes(depth - 1);
				}
			}


			else {
				this->meshes.push_back(mesh);
				std::cout << glm::to_string(Pos) << " || ADDED MESH YAO" << std::endl;
			}
			
		}

		bool intersects(BoundingBox box) {
			return (abs(Pos.x - box.pos.x)  < (halfSize + box.halfSize)) &&
				(abs(Pos.z - box.pos.z)  < (halfSize + box.halfSize));
		}
	};
	QuadNode* node;
public:
	QuadTree2();
	~QuadTree2();

	void createQuadTree(glm::vec3 pos, float halfSize, int depth);
	void addMesh(Mesh* mesh);
};

