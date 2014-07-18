/*
 * EventHandler.cpp
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#include "EventHandler.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

EventHandler::EventHandler() {
	// TODO Auto-generated constructor stub

}

void EventHandler::poll(bool& exit, Renderer& renderer) {
	SDL_Event event;
	while (SDL_PollEvent (&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit = true;
			break;

		case SDL_MOUSEWHEEL:
			renderer.zoom +=event.wheel.y*.1;
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
				case SDL_BUTTON_RIGHT:
					//do sth.
				break;
			}
			break;
		case SDL_MOUSEMOTION:

			if (event.button.button == SDL_BUTTON_LEFT) {
				renderer.rotationX += event.motion.xrel*.5;
				renderer.rotationY += event.motion.yrel*.5;
			}
			if (event.button.button == SDL_BUTTON_MIDDLE) {
				vec4 invec = vec4(event.motion.xrel * .1,
						- event.motion.yrel * .1, 0, 0);
				mat4 modelview = mat4(1.0);

				modelview = rotate(modelview, renderer.rotationY,
						vec3(1.0, 0.0, 0.0));
				modelview = rotate(modelview, renderer.rotationX,
						vec3(0.0, 1.0, 0.0));

				modelview = transpose(modelview);
				vec4 outvec = modelview * invec;
				renderer.cam_pos += vec3(outvec.x, outvec.y, outvec.z);
			}
			break;

		default:
			break;
		}
	}
}

EventHandler::~EventHandler() {
	// TODO Auto-generated destructor stub
}

