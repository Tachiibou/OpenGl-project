#ifndef CONSTVARIABLES_H
#define CONSTVARIABLES_H
#include "GL\glew.h"
#include "glm\glm.hpp"
//#define WIDTH 1024
//Sdefine HEIGHT 768
namespace constVar {
	//DISPLAY
	static const float WINDOW_WIDTH = 1024;
	static const float WINDOW_HEIGHT = 768;
	static const char* WINDOW_NAME = "3D Project";

	//CAMERA
	static const glm::vec3 CAM_POS = glm::vec3(124, 20, 124);
	static const glm::vec3 CAM_FORWARD = glm::vec3(.7f, 0, .7f);
	static const glm::vec3 CAM_UP = glm::vec3(0, 1, 0);
	static const float CAM_FOV = (70);
	static const float CAM_ZNEAR = .01f;
	static const float CAM_ZFAR = 1000.f;
	static const float CAM_ASPECT = (WINDOW_WIDTH / WINDOW_HEIGHT);

}



#endif