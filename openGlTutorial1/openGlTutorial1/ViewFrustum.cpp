#include "ViewFrustum.h"

ViewFrustum::ViewFrustum()
{
}


ViewFrustum::~ViewFrustum()
{
}

void ViewFrustum::updateFrustrum(glm::mat4 viewProjection)
{
	//planes[RIGHT].normal.x = viewProjection[0][3] - viewProjection[0][0];
	//planes[RIGHT].normal.y = viewProjection[1][3] - viewProjection[1][0];
	//planes[RIGHT].normal.z = viewProjection[2][3] - viewProjection[2][0];
	//planes[RIGHT].distanceistance = viewProjection[3][3] - viewProjection[3][0];

	//planes[LEFT].normal.x = viewProjection[0][3] + viewProjection[0][0];
	//planes[LEFT].normal.y = viewProjection[1][3] + viewProjection[1][0];
	//planes[LEFT].normal.z = viewProjection[2][3] + viewProjection[2][0];
	//planes[LEFT].distanceistance = viewProjection[3][3] + viewProjection[3][0];

	//planes[NEAR].normal.x = viewProjection[0][3] + viewProjection[0][2];
	//planes[NEAR].normal.y = viewProjection[1][3] + viewProjection[1][2];
	//planes[NEAR].normal.z = viewProjection[2][3] + viewProjection[2][2];
	//planes[NEAR].distanceistance = viewProjection[3][3] + viewProjection[3][2];

	//planes[FAR].normal.x = viewProjection[0][3] - viewProjection[0][2];
	//planes[FAR].normal.y = viewProjection[1][3] - viewProjection[1][2];
	//planes[FAR].normal.z = viewProjection[2][3] - viewProjection[2][2];
	//planes[FAR].distanceistance = viewProjection[3][3] - viewProjection[3][2];

	//planes[TOP].normal.x = viewProjection[0][3] - viewProjection[0][1];
	//planes[TOP].normal.y = viewProjection[1][3] - viewProjection[1][1];
	//planes[TOP].normal.z = viewProjection[2][3] -				[2][1];
	//planes[TOP].distanceistance = viewProjection[3][3] - viewProjection[3][1];

	//planes[BOTTOM].normal.x = viewProjection[0][3] + viewProjection[0][1];
	//planes[BOTTOM].normal.y = viewProjection[1][3] + viewProjection[1][1];
	//planes[BOTTOM].normal.z = viewProjection[2][3] + viewProjection[2][1];
	//planes[BOTTOM].distanceistance = viewProjection[3][3] + viewProjection[3][1];


	planes[RIGHT].normal.x = viewProjection[0][3] - viewProjection[0][0];
	planes[RIGHT].normal.y = viewProjection[1][3] - viewProjection[1][0];
	planes[RIGHT].normal.z = viewProjection[2][3] - viewProjection[2][0];
	planes[RIGHT].distance = viewProjection[3][3] - viewProjection[3][0];

	planes[LEFT].normal.x = viewProjection[0][3] + viewProjection[0][0];
	planes[LEFT].normal.y = viewProjection[1][3] + viewProjection[1][0];
	planes[LEFT].normal.z = viewProjection[2][3] + viewProjection[2][0];
	planes[LEFT].distance = viewProjection[3][3] + viewProjection[3][0];

	planes[BOTTOM].normal.x = viewProjection[0][3] + viewProjection[0][1];
	planes[BOTTOM].normal.y = viewProjection[1][3] + viewProjection[1][1];
	planes[BOTTOM].normal.z = viewProjection[2][3] + viewProjection[2][1];
	planes[BOTTOM].distance = viewProjection[3][3] + viewProjection[3][1];
		
	planes[TOP].normal.x = viewProjection[0][3] - viewProjection[0][1];
	planes[TOP].normal.y = viewProjection[1][3] - viewProjection[1][1];
	planes[TOP].normal.z = viewProjection[2][3] - viewProjection[2][1];
	planes[TOP].distance = viewProjection[3][3] - viewProjection[3][1];

	planes[FAR].normal.x = viewProjection[0][3] - viewProjection[0][2];
	planes[FAR].normal.y = viewProjection[1][3] - viewProjection[1][2];
	planes[FAR].normal.z = viewProjection[2][3] - viewProjection[2][2];
	planes[FAR].distance = viewProjection[3][3] - viewProjection[3][2];

	planes[NEAR].normal.x = viewProjection[0][3] + viewProjection[0][2];
	planes[NEAR].normal.y = viewProjection[1][3] + viewProjection[1][2];
	planes[NEAR].normal.z = viewProjection[2][3] + viewProjection[2][2];
	planes[NEAR].distance = viewProjection[3][3] + viewProjection[3][2];

	for (size_t i = 0; i < NUM_PLANES; i++) // normalize all planes
	{
		planes[i].normalize();
	}
}

bool ViewFrustum::dotInFrustrum(glm::vec3 pt) {
	for (size_t i = 0; i < NUM_PLANES; i++) {

		if (glm::dot(planes[i].normal,pt) + planes[i].distance < 0)
			return false;
	}

	return true;
}