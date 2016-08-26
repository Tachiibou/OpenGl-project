#include "Camera.h"
#include <iostream>
#include "glm/ext.hpp"

Camera::Camera()
{
}


Camera::Camera(glm::vec3  pos, glm::vec3 up, glm::vec3 forward, float fov, float aspect, float zNear, float zFar, Terrain* terrain, bool stableCamera){
	this->terrain = terrain;
	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::perspective(fov, aspect, zNear, zFar);
	this->fov = fov;
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;


	if (this->terrain != nullptr && (this->pos.x < this->terrain->getWidth() && this->pos.z < this->terrain->getLength()) && (this->pos.x >= 0 && this->pos.z >= 0)) {
		this->pos.y = terrain->getHeightAt(this->pos.x, this->pos.z) + CAMERA_HEIGHT;
	}

	this->horizontalAngle = 0.f;
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
	
	this->activeStableCamera = stableCamera;

	if (this->activeStableCamera) {
		this->stableCamera = new Camera(glm::vec3(pos.x, 0, pos.z), up, forward, fov, aspect, zNear, zFar, nullptr, false);
		stableTinyPerspectiveMatrix = glm::perspective(fov, aspect/3, zNear, zFar);
	}

	look(0, 0, 0);
	verticleRotationEnabled = true;
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 forward, float left, float right, float bottom, float top, float zNear, float zFar) {
	this->terrain = nullptr;
	this->pos = pos;
	this->up = up;
	this->forward = forward;
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->perspectiveMatrix = glm::ortho<float>(left,right,bottom,top,zNear,zFar);

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

	activeStableCamera = false;
	verticleRotationEnabled = true;

}

Camera::~Camera()
{
	if (activeStableCamera)
		delete stableCamera;
}

glm::mat4 Camera::getViewMatrix()const
{
	return this->viewMatrix;
}

glm::mat4 Camera::getPerspectiveMatrix()const
{
	return this->perspectiveMatrix;
}

glm::mat4 & Camera::getViewPerspectiveMatrix() const
{
	return this->perspectiveMatrix * this->viewMatrix;
}

glm::vec3 & Camera::getPos()
{
	return this->pos;
}

glm::mat4 Camera::getStableViewMatrix()const
{
	return this->stableCamera->viewMatrix;
}

glm::mat4 Camera::getStableTinyPerspectiveMatrix()const {
	return stableTinyPerspectiveMatrix;
}

glm::mat4 Camera::getStablePerspectiveMatrix()const
{
	return this->stableCamera->perspectiveMatrix;
}

glm::mat4 & Camera::getStableViewPerspectiveMatrix() const
{
	return this->stableCamera->perspectiveMatrix * this->stableCamera->viewMatrix;
}

glm::vec3 & Camera::getStablePos()
{
	return this->stableCamera->pos;
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

	if (this->terrain != nullptr && (this->pos.x < this->terrain->getWidth() && this->pos.z < this->terrain->getLength()) && (this->pos.x>=0 && this->pos.z>=0)) {
		this->pos.y = terrain->getHeightAt(this->pos.x, this->pos.z) + CAMERA_HEIGHT;
	}

	

	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->direction, this->up);

	if (activeStableCamera) {
		stableCamera->pos = glm::vec3(pos.x, 0, pos.z);
		stableCamera->viewMatrix = glm::lookAt(this->stableCamera->pos, this->stableCamera->pos + this->stableCamera->direction, this->stableCamera->up);
		//std::cout << "STABLE: " << glm::to_string(stableCamera->pos) << glm::to_string(stableCamera->direction) << std::endl;
		//std::cout << "main: " << glm::to_string(pos) << glm::to_string(direction) << std::endl;


		//stableCamera->move(x, 0, z, deltaTime);
	}

	
}

void Camera::look(float x, float y, float deltaTime)
{
	float smoothing = 0.4f;
	
		this->horizontalAngle -= this->mouseSpeed * deltaTime * x * smoothing;

		if(this->verticleRotationEnabled)
			this->verticalAngle -= this->mouseSpeed * deltaTime * y * smoothing;

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

	if (activeStableCamera) {
		this->stableCamera->direction = glm::vec3(direction.x, 0, direction.z);
		this->stableCamera->right = this->right;
		this->stableCamera->up = glm::vec3(0, 1, 0);
		this->stableCamera->viewMatrix = glm::lookAt(this->stableCamera->pos, this->stableCamera->pos + this->stableCamera->direction, this->stableCamera->up);
	}
		//stableCamera->look(x, 0, deltaTime);

	//std::cout << "Camera Forw: " << glm::to_string(direction) << std::endl;
}

void Camera::setPos(glm::vec3 pos)
{
	this->viewMatrix = glm::lookAt(pos, pos + forward, up);
	this->pos = pos;
}

void Camera::setVerticleRotationEnabled(bool val) {
	this->verticleRotationEnabled = val;
}