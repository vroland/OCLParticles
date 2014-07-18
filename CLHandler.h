/*
 * CLHandler.h
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#ifndef CLHANDLER_H_
#define CLHANDLER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <CL/cl.hpp>

class CLHandler {
public:
	CLHandler();

	bool isValid();

	cl::Context context;
	std::vector<cl::Device> devices;

	virtual ~CLHandler();

private:
	bool valid;
};

#endif /* CLHANDLER_H_ */
