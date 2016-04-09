#ifndef SCENE_H
#define SCENE_H

#include "Display.h"
#include "Camera.h"
#include "ConstVariables.h"
#include "Shader.h"
#include "Mesh.h"
#include "ResourceLoader.h"
#include "Terrain.h"

using namespace constVar;

class Scene
{
private:
	bool isRunning;
	SDL_Event sdlEvent;
	Display* display;
	Camera* camera;
	Shader* shader;
	Mesh* mesh;
	Terrain* terrain;
	Vertex vertices[4];

	glm::vec3 cameraMove;
	bool mouseWarp;
	bool ignoreMouseMotion;
	float deltaTime;
	Uint32 currentTime, lastTime;
	float moveSpeed;


	void eventHandler();
	void Update();

	void keyBoardCheck();
	void mouseCheck();
	void RenderQuad();

public:
	Scene();
	~Scene();

	void Start();
};

#endif