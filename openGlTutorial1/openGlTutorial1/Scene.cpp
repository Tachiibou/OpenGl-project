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
	this->depthShader = new Shader("depth");
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

	this->camera = new Camera(glm::vec3(0, 25, 10), CAM_UP, CAM_FORWARD, CAM_FOV, CAM_ASPECT, CAM_ZNEAR, CAM_ZFAR, nullptr);
	this->lightCamera = new Camera(glm::vec3(10, 30, 10), glm::vec3(1,0,0), glm::vec3(10,20,10),-10,10,-10,10, -10, 200);

	this->frameBuffer = new FrameBuffer();
	this->frameBuffer->CreateFrameBuffer(4,1024,768);
	this->frameBuffer->UnbindFrameBuffer();

	this->frameBuffer2 = new FrameBuffer();
	this->frameBuffer2->CreateFrameBuffer(1, 1024, 768);
	this->frameBuffer2->UnbindFrameBuffer();
	
	//this->filterComputeShader = new FilterComputeShader("derp");
	////this->filterComputeShader->LoadShader("./res/blur.glsl");
	//this->filterComputeShader->CreateShader(filterComputeShader->LoadShader("./res/blur.glsl"));
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
	delete this->geoShader;
	delete this->lightShader;
	//delete this->filterComputeShader;
	delete this->depthShader;
	delete this->lightCamera;
	delete this->frameBuffer2;
}

void Scene::Start() 
{
	glm::vec3 spherePos = glm::vec3(10, 15, 10);
	glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), spherePos);
	glm::mat4 modelMatrix2 = glm::mat4();
	GLint modelUniform = glGetUniformLocation(this->depthShader->getProgram(), "modelMatrix");
	GLint depthUniform = glGetUniformLocation(this->geoShader->getProgram(), "depth");
	GLint viewUniform = glGetUniformLocation(this->geoShader->getProgram(), "lightViewMatrix");
	GLint projectionUniform = glGetUniformLocation(this->geoShader->getProgram(), "lightPerspectiveMatrix");

	//glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	//glm::vec3 rotVec = glm::vec3(0, 1, 0);
	//float rot = 0;
	

	
	while (isRunning)
	{
		//rot += 0.001;
		//modelMatrix2 = glm::rotate(rot, rotVec);
		this->Update();
		this->eventHandler();

		this->display->Clear(0.0f, 0.15f, 0.3f, 1.0f);
		
		// Get depth map
		this->frameBuffer2->BindFrameBuffer();
		this->depthShader->Bind();
		this->depthShader->Update(*this->lightCamera);

		//modelMatrix1 = glm::translate(glm::mat4(1.0f), spherePos);
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &modelMatrix1[0][0]);
		this->mesh->Draw();

		//modelMatrix1 = glm::mat4();
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &modelMatrix2[0][0]);
		this->terrain->getMesh()->Draw();

		this->frameBuffer2->UnbindFrameBuffer();

		// Geometry pass
		this->frameBuffer->BindFrameBuffer();
		this->geoShader->Bind();
		this->frameBuffer2->BindTexturesToProgram(depthUniform, 0,3);
		this->geoShader->Update(*this->camera);
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &this->lightCamera->getViewMatrix()[0][0]);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &this->lightCamera->getPerspectiveMatrix()[0][0]);

		//modelMatrix1 = glm::translate(glm::mat4(1.0f), spherePos);
		glUniformMatrix4fv(glGetUniformLocation(this->geoShader->getProgram(), "modelMatrix"), 1, GL_FALSE, &modelMatrix1[0][0]);
		this->mesh->Draw();

		//modelMatrix1 = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(this->geoShader->getProgram(), "modelMatrix"), 1, GL_FALSE, &modelMatrix2[0][0]);
		this->terrain->getMesh()->Draw();

		this->frameBuffer->UnbindFrameBuffer();

		// Blur
		/*this->filterComputeShader->BindShader();
		this->frameBuffer->BindImageTexturesToProgram(this->filterComputeShader->GetUniformLocation("destTex"), 2);
		this->filterComputeShader->UniformVec3("colorVector", glm::vec3(0.0f, 0.0f, 1.0f));
		this->filterComputeShader->Uniform1f("number", 1.0f);
		this->filterComputeShader->DispatchCompute(1024 / 32, 768 / 32, 1);*/

		// Light pass
		this->lightShader->Bind();
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture"),0);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture2"), 1);
		this->frameBuffer->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture3"), 2);
		this->frameBuffer2->BindTexturesToProgram(glGetUniformLocation(this->lightShader->getProgram(), "renderedTexture4"), 0,3);
		glUniform3fv(glGetUniformLocation(this->lightShader->getProgram(), "lightPos"), 1, &this->lightCamera->getPos()[0]);
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