/*
 * EventHandler.h
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_
#include <iostream>
#include "SDL2/SDL.h"

namespace feedback {

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler();
	void init(std::string filename, std::string subject);
private:
	SDL_Event event;
};

} /* namespace feedback */

#endif /* EVENTHANDLER_H_ */
