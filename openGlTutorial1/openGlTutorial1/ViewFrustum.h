#pragma once
#include "glm\glm.hpp"
class GLFrustum
{
public:
	enum planeEnum
	{
		NEAR_PLANE = 0,
		FAR_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		NUM_PLANES
	};

	enum IntersectEnum
	{
		INSIDE,
		INTERSECT,
		OUTSIDE
	};

	GLFrustum();
	~GLFrustum();

	void createPlanesFromMatrix(glm::mat4 matrix);
	bool pointIsInFrustum(glm::vec3 pt);
	int AABBIsInFrustum(glm::vec3 center, glm::vec3 extent);
private:
	struct plane
	{
		glm::vec3 normal;
		float w;

		float signedDistanceToPoint(glm::vec3 pt)
		{
			return glm::dot(normal, pt) + w;
		}
	};

	plane planes[NUM_PLANES];
};

