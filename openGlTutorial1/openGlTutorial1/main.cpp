#include <iostream>
#include <GL/glew.h>
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv) {

	Display display = Display(WIDTH, HEIGHT, "hello world");

	Shader shader("./res/basicShader");
	Vertex vertices[] = {Vertex(glm::vec3(-.5f, -.5f, 0)),
						 Vertex(glm::vec3(0, 0.5f, 0)),
						 Vertex(glm::vec3(0.5f, -0.5f, 0))};
		
	Mesh mesh = Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(0, 1, 0), glm::vec3(0, 0 , 1),
		glm::radians<float>(70) , (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

	while (!display.m_IsClosed()) {
		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);
		shader.Bind();
		shader.Update(camera);
		mesh.Draw();
		display.Update();
	}

	return 0;
}
