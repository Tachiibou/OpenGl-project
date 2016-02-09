#include "Camera.h"

Camera::Camera()
{
}


Camera::Camera(glm::vec3  pos, glm::vec3 up, glm::vec3 forward, float fov, float aspect, float zNear, float zFar){
	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::perspective(fov, aspect, zNear, zFar);
	
}

Camera::~Camera()
{
}

glm::mat4& Camera::getViewMatrix()
{
	return this->viewMatrix;
}

glm::mat4& Camera::getPerspectiveMatrix()
{
	return this->perspectiveMatrix;
}
