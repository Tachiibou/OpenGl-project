#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include <iostream>
#include <SDL2\SDL.h>
#include "GL/glew.h"
class Display
{
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool isClosed;
public:
	Display(int width, int height, const std::string& title);
	virtual ~Display();
	void Update();
	bool m_IsClosed();
	void Clear(float r, float g, float b, float a);
};

#endif