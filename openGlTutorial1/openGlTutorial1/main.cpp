#include <iostream>
#include <GL/glew.h>
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"

int main(int argc, char** argv) {

	Display display = Display(800, 600, "hello world");

	Shader shader("./res/basicShader");
	Vertex vertices[] = {Vertex(glm::vec3(-.5f, -.5f, 0)),
						 Vertex(glm::vec3(0, 0.5f, 0)),
						 Vertex(glm::vec3(0.5f, -0.5f, 0))};
		
	Mesh mesh = Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	while (!display.m_IsClosed()) {
		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);
		shader.Bind();

		mesh.Draw();
		display.Update();
	}

	return 0;
}