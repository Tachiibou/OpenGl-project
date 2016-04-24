#ifndef CAMERA_H
#define CAMERA_H

#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "Terrain.h"

class Camera
{
private:
	glm::vec3 pos, up, forward;
	glm::mat4 viewMatrix, perspectiveMatrix;

	glm::vec3 direction, right;

	float verticalAngle, horizontalAngle;
	float moveSpeed, mouseSpeed;

	bool horizontalMove, verticalMove;
	int horizontalDir, verticalDir;
	Terrain* terrain;
	const int CAMERA_HEIGHT = 2;

public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 forward, float fov, float aspect, float zNear, float zFar, Terrain* terrain );
	Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 forward, float left, float right, float bottom, float top, float zNear, float zFar);
	~Camera();
	
	glm::mat4 &getViewMatrix();
	glm::mat4 &getPerspectiveMatrix();
	glm::vec3 &getPos();

	void move(glm::vec3 dir);
	void move(float x, float y, float z, float deltaTime);
	void look(float x, float y, float deltaTime);
};

#endif