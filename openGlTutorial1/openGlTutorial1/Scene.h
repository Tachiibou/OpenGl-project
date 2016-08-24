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
#include "QuadTree.h"
#include "QuadTree2.h"

using namespace constVar;

class Scene
{
private:
	bool reduceAspect;
	ViewFrustum frustrum;
	bool isRunning;
	SDL_Event sdlEvent;
	Display* display;
	Camera* camera, *lightCamera;
	Shader* geoShader, *lightShader, *depthShader, *terrainShader;
	Camera* displayCamera;

	std::vector<Mesh*> meshes;
	Terrain* terrain;
	Vertex vertices[4];
	GLuint quadVAO = 0;
	GLuint quadVBO;
	QuadTree2* quadTree;

	glm::vec3 cameraMove;
	bool mouseWarp;
	bool ignoreMouseMotion;
	float deltaTime;
	Uint32 currentTime, lastTime;
	float moveSpeed;
	FrameBuffer* frameBuffer, *frameBuffer2;
	FilterComputeShader* filterComputeShader;

	bool moveLight;
	QuadTree* qt;

	void eventHandler();
	void Update();

	void keyBoardCheck();
	void mouseCheck();

	void RenderQuad();

	void instantiateQuadTree();

public:
	Scene();
	~Scene();

	void Start();
};

#endif