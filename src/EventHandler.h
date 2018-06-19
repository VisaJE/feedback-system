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
#include "Screen.h"
#include <vector>
#include "JsonParser.h"


namespace feedback {

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler();
	void menu();
	void getSubject();
	void getFont();
	void init();
private:
	bool quit = false;
	bool pressed[5] = {false, false, false, false, false};
	void press(int y, int x);
	void unpress(int y, int x);
	void unpressAll();
	std::vector<std::string> subjects;
	std::vector<JsonParser> parsers;
	SDL_Event event;
	Screen screen;
};

} /* namespace feedback */

#endif /* EVENTHANDLER_H_ */
