/*
 * EventHandler.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#include "EventHandler.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include "Screen.h"
#include <chrono>
//#include <time.h>
#include <iomanip>
#include "JsonParser.h"

using namespace std;
namespace feedback {

EventHandler::EventHandler(): screen(Screen()) {


}

EventHandler::~EventHandler() {

}

void EventHandler::menu() {
	quit = false;
	screen.mainFontSize = 50;
	screen.menu("");
	while (!quit) {
			while(SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT: {
						quit = true;
						break;
					}
					case SDL_KEYDOWN: {
						switch (event.key.keysym.sym) {
						case SDLK_q: {
							quit = true;
							break;
						}
						case SDLK_r: {
							while (subjects.size() > 0) {
								subjects.pop_back();
								parsers.pop_back();
							}
							screen.removeSubjects();
							break;
						}

						case SDLK_f: {
							getFont();
							break;
						}
						case SDLK_RETURN : {
							if (subjects.size() > 0) init();
						}
						}
						break;
					}
					case SDL_KEYUP:
						switch (event.key.keysym.sym) {
						case SDLK_a:
							getSubject();
							break;

						}
						break;
				}
			}
	}
}

void EventHandler::getSubject() {
	bool done = false;
	string t;
	string text ="Subject: ";
	int len = 0;
	screen.menu(text);
	SDL_StartTextInput();
		while (!quit && !done) {
				while(SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_QUIT:
						quit = true;
						break;
	                case SDL_TEXTINPUT: {
	                    /* Add new text onto the end of our text */
	                	t.append(event.text.text);
	                	++len;
	                	string temp = text;
	                	screen.menu(temp.append(t));
	                }
	                	// @suppress("No break at end of case")
	                case SDL_KEYDOWN: {
	                	switch (event.key.keysym.sym) {
	                	case SDLK_BACKSPACE:
	                		if (len > 0) {
	                			--len;
	                			t.pop_back();
	                			string temp = text;
	                			screen.menu(temp.append(t));
	                		}
	                		break;
	                	case SDLK_RETURN :
	                		if (len > 0) {
	                			screen.addSubject(t);
	                			subjects.push_back(t);
	                			time_t n = chrono::system_clock::to_time_t(chrono::system_clock::now());
	                			stringstream ss;
	                			ss << t;
	                			ss << put_time(localtime(&n), "_%d-%m_%H%M");
	                			parsers.push_back(JsonParser(ss.str(), t, false));
	                			done = true;
	                		}
	                		else {
	                			done = true;
	                		}
	                		break;
	                	case SDLK_ESCAPE :
	                		done = true;
							break;
	                	}
						}
					}
				}
		}
	SDL_StopTextInput();
	screen.menu("");
}

void EventHandler::getFont() {
	bool done = false;
	string t;
	string text ="Font size: ";
	int len = 0;
	screen.menu(text);
	SDL_StartTextInput();
		while (!quit && !done) {
				while(SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_QUIT:
						quit = true;
						break;
	                case SDL_TEXTINPUT: {
	                    /* Add new text onto the end of our text */
	                	if (isdigit(event.text.text[0])) {
							t.append(event.text.text);
							len = t.length();
							string temp = text;
							screen.menu(temp.append(t));
	                	}
	                }
	                	// @suppress("No break at end of case")
	                case SDL_KEYDOWN: {
	                	switch (event.key.keysym.sym) {
	                	case SDLK_BACKSPACE:
	                		if (len > 0) {
	                			len = t.length();
	                			t.pop_back();
	                			string temp = text;
	                			screen.menu(temp.append(t));
	                		}
	                		break;
	                	case SDLK_RETURN :
	                		if (len > 0) {
	                			screen.mainFontSize = stoi(t);
	                			done = true;
	                		}
	                		else {
	                			done = true;
	                		}
	                		break;
	                	case SDLK_ESCAPE :
	                		done = true;
							break;
	                	}
						}
					}
				}
		}
	SDL_StopTextInput();
	screen.menu("");
}


void EventHandler::press(int y, int x) {
	int ind = screen.getButton(y, x);
	if (ind != -1) {
		if (ind < 5) {
			if (!pressed[ind]) {
				screen.pressButton(ind);
				pressed[ind] = true;
			}
		} else {
			screen.selectSubject(ind-5);
		}
	}
}

void EventHandler::unpress(int y, int x) {
	int ind = screen.getButton(y, x);
	if (ind != -1) {
		if (x < 5) {
			if (pressed[ind]) {
				parsers[screen.getSelection()].addData(5);
				screen.unpressButton(ind);
				pressed[ind] = false;
			} else unpressAll();
		}
	}
}

void EventHandler::unpressAll() {
	for (int i = 0; i < 5; i++) {
		screen.unpressButton(i);
	}
}

void EventHandler::init() {
	quit = false;
	bool done = false;
	screen.changeFont(screen.mainFontSize);
	screen.printButtons();
	screen.selectSubject(0);
	while (!quit && !done) {
			while(SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT: {
						quit = true;
						break;
					}
					case SDL_KEYDOWN: {
						switch (event.key.keysym.sym) {
						case SDLK_ESCAPE: {
							done = true;
							screen.changeFont(screen.MENU_FONT);
							screen.menu("");
							break;
						}
						case SDLK_5: {
							screen.pressButton(4);
							break;
						}
						}
						break;
					}
					case SDL_KEYUP: {
						switch(event.key.keysym.sym) {
						case SDLK_5: {
							parsers[screen.getSelection()].addData(5);
							screen.unpressButton(4);
							break;
						}
						}
						break;
					}
					case SDL_FINGERDOWN :
						press(event.tfinger.y*screen.SCREEN_HEIGHT, event.tfinger.x*screen.SCREEN_WIDTH);
						break;
					case  SDL_FINGERUP :
						unpress(event.tfinger.y*screen.SCREEN_HEIGHT, event.tfinger.x*screen.SCREEN_WIDTH);
						break;
				}
			}
	}
	screen.changeFont(screen.MENU_FONT);

}



} /* namespace feedback */
