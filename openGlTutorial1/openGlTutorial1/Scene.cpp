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

	this->shader = new Shader("basic");
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
	
}

Scene::~Scene()
{
	delete this->camera;
	delete this->display;
	delete this->mesh;
	delete this->shader;
	delete this->terrain;
}

void Scene::Start() 
{
	while (isRunning)
	{
		this->Update();
		this->eventHandler();

		this->display->Clear(0.0f, 0.15f, 0.3f, 1.0f);
		this->shader->Bind();
		this->shader->Update(*this->camera);
		this->mesh->Draw();
		this->terrain->getMesh()->Draw();
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