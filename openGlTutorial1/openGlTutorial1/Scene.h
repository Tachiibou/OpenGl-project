#ifndef SCENE_H
#define SCENE_H

#include "Display.h"
#include "Camera.h"
#include "ConstVariables.h"
#include "Shader.h"
#include "Mesh.h"

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
	Vertex vertices[4];


	void eventHandler();
	void Update();
public:
	Scene();
	~Scene();

	void Start();
};

#endif