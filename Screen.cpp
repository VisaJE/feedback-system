/*
 * Screen.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#include "Screen.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

using namespace std;
namespace feedback {

Screen::Screen() {
	TTF_Init();
	window = SDL_CreateWindow("DoomsdayTetris-2",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window==NULL)  {
		cout << "Window not initialised." << endl;
		destroy();
		throw 10;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture==NULL || renderer == NULL) {
		cout << "Initialising the renderer failed." << endl;
		destroy();
		throw 11;
	}
	buffer = vector<Uint32>(SCREEN_WIDTH*SCREEN_HEIGHT, 0);
	// To make the background prettier.
	for (int y =0; y < SCREEN_HEIGHT; y++) {
		for (int x =0; x < SCREEN_WIDTH; x++) {
			setPixel(y, x, BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2]);
		}
	}

}

Screen::~Screen() {
	destroy();
	TTF_Quit();
}

void Screen::destroy() {
	if (!isDestroyed) {
		if (texture != NULL) {
			SDL_DestroyTexture(texture);
			}
		if (renderer != NULL) {
				SDL_DestroyRenderer(renderer);
			}
		if (window != NULL) {
			SDL_DestroyWindow(window);
		}
	}
	isDestroyed = true;
}

void Screen::setPixel(int y, int x, int r, int g, int b) {
	Uint32 color = 0;
	color += r;
	color <<= 8;
	color += g;
	color <<= 8;
	color += b;
	color <<= 8;
	color += 0xFF;
	buffer[y*SCREEN_WIDTH+x] = color;
}

} /* namespace feedback */
