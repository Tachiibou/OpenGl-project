#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}


Camera::Camera(glm::vec3  pos, glm::vec3 up, glm::vec3 forward, float fov, float aspect, float zNear, float zFar, Terrain* terrain){
	this->terrain = terrain;
	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::perspective(fov, aspect, zNear, zFar);


	if ((this->pos.x < this->terrain->getWidth() && this->pos.z < this->terrain->getLength()) && (this->pos.x >= 0 && this->pos.z >= 0)) {
		this->pos.y = terrain->getHeightAt(this->pos.x, this->pos.z) + CAMERA_HEIGHT;
		this->pos.x = terrain->getWidth() - 10;
		this->pos.z = terrain->getLength() - 10;
	}

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

	//if (y == 0)
	//	this->direction = glm::vec3(this->direction.x, 0,  this->direction.z);

	if (z > 0)
		this->pos += this->direction * deltaTime * this->moveSpeed;
	else if (z < 0)
		this->pos -= this->direction * deltaTime * this->moveSpeed;

	if (x > 0)
		this->pos -= this->right * deltaTime * this->moveSpeed;
	else if (x < 0)
		this->pos += this->right * deltaTime * this->moveSpeed;


	//int nrAdded = 0;
	//unsigned int total = 0;
	//for (int yt = -1; yt < 2; yt++) {
	//	for (int xt = -1; xt < 2; xt++) {
	//		if ((yt + y >= 0 && xt + x >= 0) && (yt + y <= terrain->getLength() && xt + x <= terrain->getWidth())) {
	//			int index = 3 * (yt + y * terrain->getWidth() + x + xt);
	//			total += terrain->getHeightAt(x + xt, z + yt);
	//			nrAdded++;
	//		}

	//	}
	//}

	//total /= nrAdded;

	if ((this->pos.x < this->terrain->getWidth() && this->pos.z < this->terrain->getLength()) && (this->pos.x>=0 && this->pos.z>=0)) {
		this->pos.y = terrain->getHeightAt(this->pos.x, this->pos.z) + CAMERA_HEIGHT;
	}

	

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
