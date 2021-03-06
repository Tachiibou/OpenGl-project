#pragma once
#include "glm\glm.hpp"
#include "GL\glew.h"
#include <vector>
enum Planes
{
	RIGHT,
	LEFT,
	NEAR,
	FAR,
	TOP,
	BOTTOM,
	NUM_PLANES
};
class ViewFrustum
{
private:
	struct Plane
	{
		float distance;
		glm::vec3 normal;
		void normalize(){
			float length = normal.length();
			normal /= length;
			distance /= length;
			//normal = glm::normalize(this->normal);
			//distance /= this->normal.length();
		};
	};
	Plane planes[NUM_PLANES];
	glm::vec3 frustumPos;
public:
	ViewFrustum();
	~ViewFrustum();
	void updateFrustrum(glm::mat4 viewM, glm::mat4 projM);
	bool dotInFrustrum(glm::vec3 pt);
	bool quadeNodeInsFrustum(glm::vec3 pos, float halfLength);

	glm::vec3 getFrustumPos();
};

