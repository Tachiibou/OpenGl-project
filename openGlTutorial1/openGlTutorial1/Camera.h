#ifndef CAMERA_H
#define CAMERA_H

#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

class Camera
{
private:
	glm::vec3 pos, up, forward;
	glm::mat4 viewMatrix, perspectiveMatrix;
public:
	Camera();
	Camera(glm::vec3& pos, glm::vec3& up, glm::vec3& forward, float fov, float aspect, float zNear, float zFar);
	~Camera();
	
	glm::mat4 &getViewMatrix();
	glm::mat4 &getPerspectiveMatrix();
};

#endif