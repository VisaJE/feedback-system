/*
 * Main.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "JsonParser.h"
#include "EventHandler.h"

using namespace feedback;
using namespace std;

int main() {

	// Initialise SDL

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		cout << "FAIL" << endl;
		return 1;
	}

	JsonParser testParser("tes.json", "testi", false);
	testParser.addData(4);
	int values[5];
	testParser.getData(values);
	for (int i = 0; i < 5; i++) {
		cout << i+1 << ": " << values[i] << endl;
	}

	SDL_Quit();
	return 0;
}



