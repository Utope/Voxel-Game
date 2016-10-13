#pragma once

#include "SDL\SDL.h"
#include <string>
#include <iostream>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float b, float g, float a);
	void Update();
	bool IsClosed();
	inline void GetDisplayDimensions(int* w, int* h) { 
		SDL_GetWindowSize(m_window, w, h);
		
	};
	inline void Close() { m_isClosed = true; };
	inline SDL_Window* getSDL_WindowPointer() { return m_window; };
	~Display();
protected:
private:
	int height;
	int width;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
};

