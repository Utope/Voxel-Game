#include "Display.h"
#include "GL\glew.h"
#include <iostream>
#include "Transform.h"
#include <vector>
#include "ShaderProgram_Interface.h"
#include "StaticShader.h"
#include "Renderer.h"
#include "Loader.h"
#include "Camera.h"
#include "DebugRenderer.h"
#include "glm\glm.hpp"
#include "noise\noise.h"
#include "FPS.h"
#include "GLSL_ShaderProgram.h"
#include "GLSL_ShaderManager.h"
#include "MasterRenderer.h"
#include "TextureManager.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "ChunkManager.h"

#define WIDTH 640
#define HEIGHT 480

void addline(glm::vec3, glm::vec3);

int main(int argc, char** argv) {


	Display display(WIDTH, HEIGHT, "Hey!");
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(display.getSDL_WindowPointer(), WIDTH / 2, HEIGHT / 2);

	MasterRenderer renderer;	
	ChunkManager manager(2,2);
	//Texture testing
	TextureManager textureManager;
	Texture* texture;
	texture = textureManager.loadTexture(".//res//bricks.jpg");
	std::cout << textureManager.getTexture("NOT SET")->getTextureName()<< std::endl;
	//End texture testing
	
	std::vector<glm::vec3> cornerTest;
	cornerTest.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cornerTest.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	cornerTest.push_back(glm::vec3(10.0f, 0.0f, 10.0f));
	cornerTest.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
	cornerTest.push_back(glm::vec3(0.0f, -10.0f, 0.0f));
	cornerTest.push_back(glm::vec3(10.0f, -10.0f, 0.0f));
	cornerTest.push_back(glm::vec3(10.0f, -10.0f, 10.0f));
	cornerTest.push_back(glm::vec3(0.0f, -10.0f, 10.0f));	

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),WIDTH, HEIGHT,70.0f,0.1f,1000.0f);

	FPS fps;
	fps.init();

	bool mouseDrag = false;
	SDL_Event e;

	int nbFrames = 0;
	double lastTime = SDL_GetTicks();
	double currentTime = SDL_GetTicks();
	bool blockupdate = false;

	std::vector<glm::vec3> positionTesting;
	positionTesting.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	positionTesting.push_back(glm::vec3(-10.0f, -10.0f, -10.0f));
	std::vector<glm::vec3> colorTesting;
	colorTesting.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
	colorTesting.push_back(glm::vec3(0.5f, 0.5f, 0.5f));

	while (!display.IsClosed()) {

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				display.Close();
			}
			else {
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_TAB) {
						display.Close();
					}
					else if (e.key.keysym.sym == SDLK_l) {
						
					}
				}
			}
			camera.input(e);
		}

		fps.limit();
		std::cout << fps.fpsData.fps << std::endl;
		camera.update();

			
		//renderer.getDebugRenderer()->addCube(cornerTest);
		renderer.getDebugRenderer()->drawAxis();
		//renderer.getDebugRenderer()->addLine(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(10.0f,10.0f,10.0f));
		renderer.render(camera);
		
		display.Update();

	}
	
	return 0;
}
