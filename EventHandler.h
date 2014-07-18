/*
 * EventHandler.h
 *
 *  Created on: 01.07.2014
 *      Author: valentin
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <SDL2/SDL.h>
#include "Renderer.h"

class EventHandler {
public:
	EventHandler();

	void poll(bool& exit, Renderer& renderer);

	virtual ~EventHandler();
};

#endif /* EVENTHANDLER_H_ */
