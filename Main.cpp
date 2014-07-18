/*
 * Main.cpp
 *
 *  Created on: 30.06.2014
 *      Author: valentin
 */

#include "Main.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "CLHandler.h"

#include <iostream>
#include <GL/gl.h>

using namespace std;

Main::Main() {
	mainWindow = NULL;
	mainGLContext = NULL;
}

FUNC_STATUS Main::init() {

	/* First, initialize SDL's video subsystem. */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		/* Failed, exit. */
		checkSDLError(__LINE__);
		return FS_CRITICAL_FAILURE;
	}

	checkSDLError(__LINE__);


	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */

	Uint32 flags = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
	int width = DEFAULT_WIDTH;
	int height = DEFAULT_HEIGHT;

	/* Create our window centered at 512x512 resolution */
	mainWindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, flags);

	checkSDLError(__LINE__);
	if (!mainWindow) {
		/* Failed, exit. */
		return FS_CRITICAL_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	mainGLContext = SDL_GL_CreateContext(mainWindow);
	checkSDLError(__LINE__);

	cout << glGetString(GL_VERSION) << endl;
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	return FS_SUCCESS;
}

FUNC_STATUS Main::mainLoop() {
	Renderer renderer;
	EventHandler eventHandler;
	renderer.glInit(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	bool exit = false;

	CLHandler clHandler;
	if (!clHandler.isValid()) {
		return FS_CRITICAL_FAILURE;
	}
	ParticleSystem psys(&clHandler,3001,1,.005);
	if (!psys.isValid()) {
		return FS_CRITICAL_FAILURE;
	}


	while (!exit) {
		eventHandler.poll(exit,renderer);
		renderer.render(&psys);
		SDL_GL_SwapWindow(mainWindow);
	}

	return FS_SUCCESS;
}

FUNC_STATUS Main::quit() {
	SDL_GL_DeleteContext(mainGLContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return FS_SUCCESS;
}

Main::~Main() {
	quit();
}

