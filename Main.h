/*
 * Main.h
 *
 *  Created on: 30.06.2014
 *      Author: valentin
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "utils.h"
#include "constants.h"

#include <SDL2/SDL.h>
#include <GL/glut.h>

class Main {
public:
	Main();

	FUNC_STATUS init();
	FUNC_STATUS mainLoop();
	FUNC_STATUS quit();

	virtual ~Main();
private:
	SDL_Window *mainWindow; /* Our window handle */
	SDL_GLContext mainGLContext; /* Our opengl context handle */
};

int main(int argc, char* argv[]) {
	Main prgm;
	glutInit(&argc,argv);

	FUNC_STATUS status = prgm.init();
	if (status == FS_CRITICAL_FAILURE) {
		return status;
	}

	status = prgm.mainLoop();
	if (status == FS_CRITICAL_FAILURE) {
		return status;
	}

	prgm.quit();
}
#endif /* MAIN_H_ */
