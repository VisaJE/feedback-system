/*
 * EventHandler.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#include "EventHandler.h"
#include "SDL2/SDL.h"
#include <iostream>

using namespace std;
namespace feedback {

EventHandler::EventHandler() {
	// TODO Auto-generated constructor stub

}

EventHandler::~EventHandler() {
	// TODO Auto-generated destructor stub
}

void EventHandler::init(string filename, string subject) {
	bool quit = false;
	while (!quit) {
			while(SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT: {
						quit = true;
						break;
					}
				}
			}
	}

}



} /* namespace feedback */
