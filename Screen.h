/*
 * Screen.h
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#ifndef SCREEN_H_
#define SCREEN_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
namespace feedback {

class Screen {
public:
	Screen();
	virtual ~Screen();
	const int SCREEN_HEIGHT = 480;
	const int SCREEN_WIDTH = 800;
private:
	const int BACKGROUND_COLOR[3] = {50, 50, 0};
	bool isDestroyed = false;
	void destroy();
	void setPixel(int y, int x, int r, int g, int b);
	std::vector<Uint32> buffer;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
};

} /* namespace feedback */

#endif /* SCREEN_H_ */
