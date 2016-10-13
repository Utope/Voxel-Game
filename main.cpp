#include "Display.h"
#include "GL\glew.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "Transform.h"
#include <vector>
#include "ShaderProgram_Interface.h"
#include "StaticShader.h"
#include "Renderer.h"
#include "Loader.h"
#include "Camera.h"
#include "Chunk.h"
#include "DebugRenderer.h"
#include "ChunkRenderer.h"
#include "glm\glm.hpp"
#include "noise\noise.h"
#include "FPS.h"
#include "GLSL_ShaderProgram.h"
#include "GLSL_ShaderManager.h"
#include "MasterRenderer.h"
#include "tinyxml2.h"

#define WIDTH 640
#define HEIGHT 480

void addline(glm::vec3, glm::vec3);

int main(int argc, char** argv) {

	Display display(WIDTH, HEIGHT, "Hey!");

	
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(display.getSDL_WindowPointer(), WIDTH / 2, HEIGHT / 2);

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
	positions.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	positions.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
	positions.push_back(glm::vec3(0.5f, 0.5f, 0.0f));

	std::vector<glm::vec2> textureCoords;
	textureCoords.push_back(glm::vec2(0.0f, 1.0f));
	textureCoords.push_back(glm::vec2(0.0f, 0.0f));
	textureCoords.push_back(glm::vec2(1.0f, 0.0f));
	textureCoords.push_back(glm::vec2(1.0f, 1.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	MasterRenderer renderer;

	Chunk chunk(*renderer.getChunkRenderer(),glm::vec3(0.0f,0.0f,0.0f));
	chunk.activateAll();
	chunk.updateMesh();
	
	
	Loader loader;
///	Renderer renderer;
	
	RawModel rawModel = loader.loadToVAO(positions, textureCoords, indices);
	ModelTexture modelTexture = loader.loadTexture(".\\res\\bricks.jpg");
	TexturedModel texturedModel(rawModel, modelTexture);
	Entity entity(texturedModel,glm::vec3(0.0f,0.0f,0.0f),0,0,0,glm::vec3(1.0f,1.0f,1.0f));
	
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),WIDTH, HEIGHT,70.0f,0.1f,1000.0f);

	FPS fps;
	fps.init();

	bool mouseDrag = false;
	SDL_Event e;

	int nbFrames = 0;
	double lastTime = SDL_GetTicks();
	double currentTime = SDL_GetTicks();
	bool blockupdate = false;
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
						chunk.deactivateBlock(rand() % 16, rand() % 16, rand() % 16);
						blockupdate = true;
					}
				}
			}
			camera.input(e);
		}

		fps.limit();

		camera.update();
			
		renderer.getDebugRenderer()->drawAxis();
		renderer.getDebugRenderer()->addLine(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(10.0f,10.0f,10.0f));
		renderer.render(camera);
			
		display.Update();

		if (blockupdate) {
			chunk.updateMesh();
			blockupdate = false;
		}
	}

	loader.cleanUp();
	return 0;
}
