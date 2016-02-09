#include "Scene.h"



void Scene::Update()
{
}

Scene::Scene()
{
	this->isRunning = true;
	this->display = new Display(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
	this->camera = new Camera(CAM_POS, CAM_UP, CAM_FORWARD, CAM_FOV, CAM_ASPECT, CAM_ZNEAR, CAM_ZFAR);

	this->shader = new Shader("saidnasind");
	vertices[0] = Vertex(glm::vec3(-.5f, -.5f, 0));
	vertices[1] = Vertex(glm::vec3(0, 0.5f, 0));
	vertices[2] = Vertex(glm::vec3(0.5f, -0.5f, 0));

	this->mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
}


Scene::~Scene()
{
	delete this->camera;
	delete this->display;
	delete this->mesh;
	delete this->shader;
}

void Scene::Start() {
	while (isRunning) {
		
		eventHandler();

		this->display->Clear(0.0f, 0.15f, 0.3f, 1.0f);
		this->shader->Bind();
		this->shader->Update(*this->camera);
		this->mesh->Draw();
		
		this->display->Update();
		
	}
}


void Scene::eventHandler() {
	
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_QUIT)
			isRunning = false;

		if (sdlEvent.type == SDL_KEYDOWN) {
			printf("Key press detected\n");
		}
	}
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		//switch (sdlEvent.type) {
		//	case SDL_KEYDOWN:
			//switch (event.key.keysym.sym) {
			//	case SDLK_w:
			//		printf("W press detected\n");
			//		break;
			//	case SDLK_s:
			//		printf("S press detected\n");
			//		break;
			//	case SDLK_a:
			//		printf("A press detected\n");
			//		break;
			//	case SDLK_d:
			//		printf("D press detected\n");
			//		break;
			//}
	//		printf("Key press detected\n");
	//		break;

	//	case SDL_KEYUP:
	//		printf("Key release detected\n");
	//		break;

	//	default:
	//		break;
	//	}
	//}
}