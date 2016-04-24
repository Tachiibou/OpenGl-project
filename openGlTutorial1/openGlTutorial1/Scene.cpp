#include "Scene.h"

Scene::Scene()
{

	ResourceLoader r = ResourceLoader("obj/sphere1.obj");
	
	this->isRunning = true;
	this->display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

	
	this->mouseWarp = false;
	this->deltaTime = 0.0f;
	this->currentTime = 0.0f;
	this->lastTime = 0.0f;

	this->shader = new Shader("backFace", true);
	this->shader2 = new Shader("texture");
	this->geoShader = new Shader("geoPass");
	this->lightShader = new Shader("lightPass");
	this->depthShader = new Shader("depthMap");
	this->debugShadowShader = new Shader("debugShadow");

	vertices[0] = Vertex(glm::vec3(-.5f, -.5f, 0));
	vertices[1] = Vertex(glm::vec3(0, 0.5f, 0));
	vertices[2] = Vertex(glm::vec3(.5f, -0.5f, 0));
	vertices[3] = Vertex(glm::vec3(1.5f, 0.5f, 0));

	int indices[]{ 0,1,2,
					1,2,3};

	//this->mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	this->terrain = new Terrain();
	this->terrain->loadTerrain("./res/heightmap.png", 15);
	this->mesh = r.getMesh();

	this->camera = new Camera(CAM_POS, CAM_UP, CAM_FORWARD, CAM_FOV, CAM_ASPECT, CAM_ZNEAR, CAM_ZFAR, this->terrain);

	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->CreateFrameBuffer(3);
	this->frameBuffer->UnbindFrameBuffer();

	this->shadowMapFB = new FrameBuffer();
	this->shadowMapFB->AddDepthMap();
	this->shadowMapFB->UnbindFrameBuffer();
	
	this->lightPos = glm::vec3(0, 1, 0);
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, CAM_ZNEAR, CAM_ZFAR);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->lightSpaceMatrix = lightProjection * lightView;
	this->depthShader->Bind();
	glUniformMatrix4fv(glGetUniformLocation(this->depthShader->getProgram(), "lightSpaceMatrix"), 1, GL_FALSE, &this->lightSpaceMatrix[0][0]);
}

Scene::~Scene()
{
	delete this->camera;
	delete this->display;
	delete this->mesh;
	delete this->shader;
	delete this->terrain;
	delete this->shader2;
	delete this->frameBuffer;
	delete this->shadowMapFB;
	delete this->geoShader;
	delete this->lightShader;
	delete this->depthShader;
	delete this->debugShadowShader;
}

void Scene::Start() 
{
	GLuint texID, texID2;
	GLfloat derp[4] = { 1,0,0,1 };

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GLfloat nearPlane = CAM_ZNEAR, farPlane = CAM_ZFAR;

	while (isRunning)
	{
		this->Update();
		this->eventHandler();

		this->depthShader->Bind();
		glUniformMatrix4fv(glGetUniformLocation(this->depthShader->getProgram(), "lightSpaceMatrix"), 1, GL_FALSE, &this->lightSpaceMatrix[0][0]);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		this->shadowMapFB->BindDepthMapFB();

		glClear(GL_DEPTH_BUFFER_BIT);
		this->mesh->Draw();
		this->terrain->getMesh()->Draw();

		this->shadowMapFB->UnbindFrameBuffer();

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->debugShadowShader->Bind();
		glUniform1f(glGetUniformLocation(this->depthShader->getProgram(), "nearPlane"), nearPlane);
		glUniform1f(glGetUniformLocation(this->depthShader->getProgram(), "farPlane"), farPlane);
		this->shadowMapFB->BindDepthMapToProgram(glGetUniformLocation(this->debugShadowShader->getProgram(), "depthMap"));

		/*this->display->Clear(0.0f, 0.15f, 0.3f, 1.0f);
		this->geoShader->Bind();
		this->frameBuffer->BindFrameBuffer();
		this->geoShader->Update(*this->camera);
		this->mesh->Draw();
		this->terrain->getMesh()->Draw();
		this->frameBuffer->UnbindFrameBuffer();

		this->lightShader->Bind();
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture"),0);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture2"), 1);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture3"), 2);
		glUniform3fv(glGetUniformLocation(this->lightShader->getProgram(), "lightPos"), 1, &this->lightPos[0]);*/
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
			else if (this->sdlEvent.type == SDL_MOUSEMOTION && !this->ignoreMouseMotion)
				this->mouseCheck();
			else if (this->ignoreMouseMotion)
				this->ignoreMouseMotion = false;
		}
	}
	if (this->cameraMove != glm::vec3())
		this->camera->move(this->cameraMove.x, this->cameraMove.y, this->cameraMove.z, this->deltaTime);
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

	//Lock cursor
	if (this->sdlEvent.key.keysym.sym == SDLK_l && !this->mouseWarp)
	{
		this->mouseWarp = true;
		this->ignoreMouseMotion = false;
	}
	else if (this->sdlEvent.key.keysym.sym == SDLK_l && this->mouseWarp)
		this->mouseWarp = false;
}

void Scene::mouseCheck()
{
	//std::cout << "x: " << this->sdlEvent.motion.xrel << " y: " << this->sdlEvent.motion.yrel << '\n';
	this->camera->look(this->sdlEvent.motion.xrel, this->sdlEvent.motion.yrel, this->deltaTime);
	if (this->mouseWarp)
	{
		SDL_WarpMouseInWindow(NULL, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		this->ignoreMouseMotion = true;
	}
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