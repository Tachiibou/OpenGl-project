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
			this->normal = glm::normalize(this->normal);
			this->distance /= this->normal.length();
		};
	};
	Plane planes[NUM_PLANES];
public:
	ViewFrustum();
	~ViewFrustum();
	void updateFrustrum(glm::mat4 viewProjection);
	bool dotInFrustrum(const glm::vec3& pt);

};

