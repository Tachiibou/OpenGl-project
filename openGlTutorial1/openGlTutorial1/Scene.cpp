#include "Scene.h"
#include "glm/ext.hpp"
Scene::Scene()
{
	
	this->reduceAspect = false;
	this->isRunning = true;
	this->display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

	
	this->deltaTime = 0.0f;
	this->currentTime = 0.0f;
	this->lastTime = 0.0f;

	this->geoShader = new Shader("geoPass", true);
	this->lightShader = new Shader("lightPass");
	this->depthShader = new Shader("depth");
	this->terrainShader = new Shader("terrain", true);


	this->terrain = new Terrain();
	this->terrain->loadTerrain("./res/heightmapClone.png", .4f);

	this->camera = new Camera(CAM_POS, CAM_UP, CAM_FORWARD, CAM_FOV, CAM_ASPECT, CAM_ZNEAR, CAM_ZFAR, terrain,true);
	this->lightCamera = new Camera(glm::vec3(124,70,124), glm::vec3(1,0,0), glm::vec3(0,-1,0),-100,100,-100,100, 0.f, 200);

	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->CreateFrameBuffer(4,WINDOW_WIDTH, WINDOW_HEIGHT);
	this->frameBuffer->UnbindFrameBuffer();

	this->frameBuffer2 = new FrameBuffer();
	this->frameBuffer2->CreateFrameBuffer(1, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->frameBuffer2->UnbindFrameBuffer();
	
	this->filterComputeShader = new FilterComputeShader("derp");
	this->filterComputeShader->CreateShader(filterComputeShader->LoadShader("./res/blur.glsl"));

	
	ResourceLoader r = ResourceLoader("obj/sphere1.obj");


	int totalMeshes = 5;
	for (size_t i = 0; i < totalMeshes; i++)
	{
		meshes.push_back(r.getMesh());

		meshes.back()->setPos(glm::vec3((124 - totalMeshes * 2.5f) + i*5, 10, 124));
		//meshes.back()->setPos(glm::vec3(124 , 20-i*5, 124));
	}
	

	
	instantiateQuadTree();
	displayCamera = camera;

	SDL_SetRelativeMouseMode(SDL_TRUE); // lock mouse
	this->mouseLocked = true;
}

void Scene::instantiateQuadTree() {
	this->quadTree = new QuadTree2();

	//Instantiates the quad tree, Position, Size of biggest quad, Depth
	this->quadTree->createQuadTree(glm::vec3(124, 0, 124), 256, 7);



	//Adds meshes to the quad tree, will add right meach to corresponding quad.
	for (size_t i = 0; i < meshes.size(); i++)
	{
		this->quadTree->addMesh(meshes[i]);

	}
}

Scene::~Scene()
{
	delete this->camera;
	delete this->display;
	delete this->terrain;
	delete this->terrainShader;
	delete this->frameBuffer;
	delete this->geoShader;
	delete this->lightShader;
	delete this->filterComputeShader;
	delete this->depthShader;
	delete this->lightCamera;
	delete this->frameBuffer2;
	delete this->quadTree;

	//Delete vector of meshes
	for (size_t i = 0; i < meshes.size(); i++)
	{
		delete meshes.at(i);
	}
}

void Scene::Start() 
{
	GLint viewUniform = glGetUniformLocation(this->geoShader->getProgram(), "lightViewMatrix");
	GLint projectionUniform = glGetUniformLocation(this->geoShader->getProgram(), "lightPerspectiveMatrix");

	glm::mat4 worldMatrix(1.0f);
	std::vector<Mesh*> meshesInQuadTree;
	//Game loop
	while (isRunning)
	{
		this->frustrum.updateFrustrum(this->camera->getStableViewMatrix(),(reduceAspect? this->camera->getStableTinyPerspectiveMatrix() : this->camera->getStablePerspectiveMatrix()));

		meshesInQuadTree = quadTree->getMeshes(&this->frustrum);


		GLfloat lightPos[3] = { this->lightCamera->getPos().x,this->lightCamera->getPos().y,this->lightCamera->getPos().z };
		GLfloat camPos[3] = { this->camera->getPos().x,this->camera->getPos().y,this->camera->getPos().z };

		this->Update();
		this->eventHandler();

		this->display->Clear(0.0f, 0.15f, 0.3f, 1.0f);



		this->depthShader->Bind(); // The depth shader returns the depth in Z for all fragments
		this->frameBuffer2->BindFrameBuffer(); // Framebuffer2 will store the depth map the depthshader draws
		this->depthShader->Update(*this->lightCamera); // We draw the depthmap from the lights perspective
		

		for (size_t i = 0; i < meshesInQuadTree.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(this->depthShader->getProgram(), "worldMatrix"), 1, GL_FALSE, &meshesInQuadTree[i]->getWorldMatrix()[0][0]);
			meshesInQuadTree[i]->Draw();
		}

		worldMatrix = glm::translate(this->terrain->getPos());
		glUniformMatrix4fv(glGetUniformLocation(this->depthShader->getProgram(), "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);
		this->terrain->getMesh()->Draw();
		this->frameBuffer2->UnbindFrameBuffer();


		this->frameBuffer->BindFrameBuffer();
		this->geoShader->Bind();
		this->frameBuffer2->BindTexturesToProgram(glGetUniformLocation(this->geoShader->getProgram(), "depth"), 0,3); // Send depth texture to geoShader to calculate the shadows
		this->geoShader->Update(*this->displayCamera);
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &this->lightCamera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &this->lightCamera->getPerspectiveMatrix()[0][0]);

		for (size_t i = 0; i < meshesInQuadTree.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(this->geoShader->getProgram(), "worldMatrix"), 1, GL_FALSE, &meshesInQuadTree[i]->getWorldMatrix()[0][0]);
			meshesInQuadTree[i]->Draw();
		}
		
		this->terrainShader->Bind();
		this->frameBuffer2->BindTexturesToProgram(glGetUniformLocation(this->terrainShader->getProgram(), "depth"), 0, 3);
		//GLuint DERP = glGetUniformLocation(this->terrainShader->getProgram(), "texture0");
		//GLuint DERP1 = glGetUniformLocation(this->terrainShader->getProgram(), "texture1");
		this->terrainShader->Update(*this->displayCamera);
		glUniformMatrix4fv(glGetUniformLocation(this->terrainShader->getProgram(), "lightViewMatrix"), 1, GL_FALSE, &this->lightCamera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(this->terrainShader->getProgram(), "lightPerspectiveMatrix"), 1, GL_FALSE, &this->lightCamera->getPerspectiveMatrix()[0][0]);

		worldMatrix = glm::translate(this->terrain->getPos());
		glUniformMatrix4fv(glGetUniformLocation(this->terrainShader->getProgram(), "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);
		this->terrain->getMesh()->Draw();
		this->frameBuffer->UnbindFrameBuffer();

		this->filterComputeShader->BindShader();
		this->frameBuffer->BindImageTexturesToProgram(this->filterComputeShader->GetUniformLocation("colorTexture"), 2); // Get the textur to apply the blur to
		this->filterComputeShader->DispatchCompute(1024 / 32, 768 / 32, 1,30); // Compute the blur several times for stronger effect
		// The compute shader directly changes the pixel data, so no need to get any output data

		this->lightShader->Bind();
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "positionTexture"), 0);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "normalTexture"), 1);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "colorTexture"), 2);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "specularTexture"), 3,4);
		glUniform3fv(glGetUniformLocation(this->lightShader->getProgram(), "lightPos"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(this->lightShader->getProgram(), "viewPos"), 1, camPos);
		this->RenderQuad();
		
		this->display->Update();
		
	}
}

void Scene::eventHandler() 
{
	this->cameraMove = glm::vec3();

	while (SDL_PollEvent(&this->sdlEvent))
	{
		if (this->sdlEvent.type == SDL_QUIT)
			isRunning = false;
		else
		{
			if (this->sdlEvent.type == SDL_KEYDOWN)
				this->keyBoardCheck();
			else if (this->sdlEvent.type == SDL_MOUSEMOTION)
				this->mouseCheck();
		}
	}
	if (this->cameraMove != glm::vec3())
	{
		this->camera->move(this->cameraMove.x, this->cameraMove.y, this->cameraMove.z, this->deltaTime);
	}
		
}

void Scene::Update()
{
	this->currentTime = SDL_GetTicks();
	this->deltaTime = (float)(this->currentTime - this->lastTime) / 1000;
	this->lastTime = this->currentTime;
}

void Scene::keyBoardCheck()
{
	//Forward & Backward
	if (this->sdlEvent.key.keysym.sym == SDLK_w)
		this->cameraMove.z = 1.0f;
	else if (this->sdlEvent.key.keysym.sym == SDLK_s)
		this->cameraMove.z = -1.0f;

	//Right & Left
	if (this->sdlEvent.key.keysym.sym == SDLK_a)
		this->cameraMove.x = 1.0f;
	else if (this->sdlEvent.key.keysym.sym == SDLK_d)
		this->cameraMove.x = -1.0f;

	//Up & Down
	if (this->sdlEvent.key.keysym.sym == SDLK_UP)
		this->cameraMove.y = 1.0f;
	else if (this->sdlEvent.key.keysym.sym == SDLK_DOWN)
		this->cameraMove.y = -1.0f;

	//Lock cursor for reals
	if (this->sdlEvent.key.keysym.sym == SDLK_ESCAPE)
	{
		if(this->mouseLocked)
			SDL_SetRelativeMouseMode(SDL_FALSE);
		else
			SDL_SetRelativeMouseMode(SDL_TRUE);
		this->mouseLocked = !this->mouseLocked;
	}
		
	if (this->sdlEvent.key.keysym.sym == SDLK_q) {
		if (displayCamera == camera) {
			displayCamera = lightCamera;
			camera->setVerticleRotationEnabled(false);
		}
			
		else {
			displayCamera = camera;
			camera->setVerticleRotationEnabled(true);
		}
			
	}
		//this->reduceAspect = !reduceAspect;
}

void Scene::mouseCheck()
{
	//std::cout << "x: " << this->sdlEvent.motion.xrel << " y: " << this->sdlEvent.motion.yrel << '\n';

	this->camera->look(this->sdlEvent.motion.xrel, this->sdlEvent.motion.yrel, this->deltaTime);
}

// renders a quad with uv coordinates
void Scene::RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}