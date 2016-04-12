#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}


Camera::Camera(glm::vec3  pos, glm::vec3 up, glm::vec3 forward, float fov, float aspect, float zNear, float zFar){
	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::perspective(fov, aspect, zNear, zFar);


	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::perspective(fov, aspect, zNear, zFar);


	this->horizontalAngle = 3.14f;
	this->verticalAngle = 0.0f;
	this->mouseSpeed = 1.0f;
	this->moveSpeed = 60.0f;

	this->direction = glm::vec3(
		cos(this->verticalAngle) * sin(this->horizontalAngle),
		sin(this->verticalAngle),
		cos(this->verticalAngle) * cos(this->horizontalAngle));

	this->right = glm::vec3(
		sin(this->horizontalAngle - 3.14f / 2.0f),
		0,
		cos(this->horizontalAngle - 3.14f / 2.0f)
		);
	
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

glm::vec3 & Camera::getPos()
{
	return this->pos;
}

void Camera::move(glm::vec3 dir)
{
	this->pos += /*glm::normalize(this->forward -*/ dir;
	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->forward, this->up);
}

void Camera::move(float x, float y, float z, float deltaTime)
{
	if (z > 0)
		this->pos += this->direction * deltaTime * this->moveSpeed;
	else if (z < 0)
		this->pos -= this->direction * deltaTime * this->moveSpeed;

	if (x > 0)
		this->pos -= this->right * deltaTime * this->moveSpeed;
	else if (x < 0)
		this->pos += this->right * deltaTime * this->moveSpeed;

	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->direction, this->up);
}

void Camera::look(float x, float y, float deltaTime)
{
	if (x < 10)
		this->horizontalAngle -= this->mouseSpeed * deltaTime * x;
	if (y < 10)
		this->verticalAngle -= this->mouseSpeed * deltaTime * y;

	this->direction = glm::vec3(
		cos(this->verticalAngle) * sin(this->horizontalAngle),
		sin(this->verticalAngle),
		cos(this->verticalAngle) * cos(this->horizontalAngle));

	this->right = glm::vec3(
		sin(this->horizontalAngle - 3.14f / 2.0f),
		0,
		cos(this->horizontalAngle - 3.14f / 2.0f)
		);

	this->up = glm::cross(this->right, this->direction);

	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->direction, this->up);
}
