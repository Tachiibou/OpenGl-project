#ifndef SCENE_H
#define SCENE_H

#include "Display.h"
#include "Camera.h"
#include "ConstVariables.h"
#include "Shader.h"
#include "Mesh.h"
#include "ResourceLoader.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "FilterComputeShader.h"
#include "ViewFrustum.h"

using namespace constVar;

class Scene
{
private:
	GLFrustum frustrum;
	bool isRunning;
	SDL_Event sdlEvent;
	Display* display;
	Camera* camera, *lightCamera;
	Shader* shader, *shader2,
		  *geoShader, *lightShader, *depthShader, *terrainShader;
	Mesh* mesh;
	Terrain* terrain;
	Vertex vertices[4];
	GLuint quadVAO = 0;
	GLuint quadVBO;

	glm::vec3 cameraMove;
	bool mouseWarp;
	bool ignoreMouseMotion;
	float deltaTime;
	Uint32 currentTime, lastTime;
	float moveSpeed;
	FrameBuffer* frameBuffer, *frameBuffer2;
	FilterComputeShader* filterComputeShader;

	bool moveLight;


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