#include "ViewFrustum.h"



GLFrustum::GLFrustum()
{
}


GLFrustum::~GLFrustum()
{
}

void GLFrustum::createPlanesFromMatrix(glm::mat4 matrix)
{
	planes[LEFT_PLANE].normal.x = matrix[0][3] + matrix[0][0];
	planes[LEFT_PLANE].normal.y = matrix[1][3] + matrix[1][0];
	planes[LEFT_PLANE].normal.z = matrix[2][3] + matrix[2][0];
	planes[LEFT_PLANE].w = matrix[3][3] + matrix[3][0];

	planes[RIGHT_PLANE].normal.x = matrix[0][3] - matrix[0][0];
	planes[RIGHT_PLANE].normal.y = matrix[1][3] - matrix[1][0];
	planes[RIGHT_PLANE].normal.z = matrix[2][3] - matrix[2][0];
	planes[RIGHT_PLANE].w = matrix[3][3] - matrix[3][0];

	planes[BOTTOM_PLANE].normal.x = matrix[0][3] + matrix[0][1];
	planes[BOTTOM_PLANE].normal.y = matrix[1][3] + matrix[1][1];
	planes[BOTTOM_PLANE].normal.z = matrix[2][3] + matrix[2][1];
	planes[BOTTOM_PLANE].w = matrix[3][3] + matrix[3][1];

	planes[TOP_PLANE].normal.x = matrix[0][3] - matrix[0][1];
	planes[TOP_PLANE].normal.y = matrix[1][3] - matrix[1][1];
	planes[TOP_PLANE].normal.z = matrix[2][3] - matrix[2][1];
	planes[TOP_PLANE].w = matrix[3][3] - matrix[3][1];

	planes[NEAR_PLANE].normal.x = matrix[0][3] + matrix[0][2];
	planes[NEAR_PLANE].normal.y = matrix[1][3] + matrix[1][2];
	planes[NEAR_PLANE].normal.z = matrix[2][3] + matrix[2][2];
	planes[NEAR_PLANE].w = matrix[3][3] + matrix[3][2];

	planes[FAR_PLANE].normal.x = matrix[0][3] - matrix[0][2];
	planes[FAR_PLANE].normal.y = matrix[1][3] - matrix[1][2];
	planes[FAR_PLANE].normal.z = matrix[2][3] - matrix[2][2];
	planes[FAR_PLANE].w = matrix[3][3] - matrix[3][2];

	for (unsigned int i = 0; i < 6; i++)
	{
		float length = planes[i].normal.length();
		planes[i].normal /= length;
		planes[i].w /= length;
	}
}

bool GLFrustum::pointIsInFrustum(glm::vec3 pt)
{
	for (int i = 0; i < NUM_PLANES; i++)
	{
		if (planes[i].signedDistanceToPoint(pt) < 0)
		{
			return false;
		}
	}

	return true;
}

int GLFrustum::AABBIsInFrustum(glm::vec3 center, glm::vec3 extent)
{
	int results = INSIDE;
	for (int i = 0; i < NUM_PLANES; i++)
	{
		float d = dot(center, planes[i].normal) + planes[i].w;
		float r = dot(extent, glm::abs(planes[i].normal));
		if (d + r < 0) return OUTSIDE;
		if (d - r < 0) results = INTERSECT;
	}
	return results;
}
