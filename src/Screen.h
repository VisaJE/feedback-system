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
#include <iostream>
namespace feedback {

class Screen {
public:
	Screen();
	Screen(int fontSize);
	virtual ~Screen();

	void changeFont(int s);

	const int SCREEN_HEIGHT = 480;
	const int SCREEN_WIDTH = 800;

	void printButtons();
	void pressButton(int i);
	void unpressButton(int i);
	void addSubject(std::string subject);
	void removeSubjects();
	void printSubjects();
	void selectSubject(int i);
	unsigned int getSelection();
	int mainFontSize =0;
	const int MENU_FONT = 20;
	void menu(std::string subject);
	int getButton(int y, int x);
private:
	const int BACKGROUND_COLOR[3] = {90, 90, 150};
	void printBackground();
	unsigned int selection = 0;
	void selectNext();
	std::vector<std::string> subjects;
	TTF_Font* font;
	bool isDestroyed = false;
	void destroy();
	void setPixel(int y, int x, int r, int g, int b);
	void refreshFromBuffer();
	typedef int32_t s32;

	const int butY = 320;
	const int rad = 70;
	const int spacing = (SCREEN_WIDTH - 10*rad)/6;
	int col[5][3] ={ {243, 68, 68}, {255, 147, 38}, {245, 230, 58}, {198, 244, 50},{112, 246, 76}};

	void colorSetP(int x, int y, int r, int g, int b, int rad);
	void printButton(const int y, const int rad, const int spacing, int i);
	void pressButton(const int y, const int rad, const int spacing, int i);
	void drawCircle(s32 _x, s32 _y, s32 radius, int r, int g, int b);
	std::vector<Uint32> buffer;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;

	const int SUBJECT_SPACING = 20;
	const int SUBJECT_Y_OFFSET = 40;
	const int SUBJECT_HEIGHT = 150;
	int fontSize = 50;
	const int BOX_COLOR[3] = {60, 60, 80};
	const int SELECTION_COLOR[3] = {70, 180, 80};
	SDL_Color TEXT_COLOR = {230, 230, 220};
	int boxWidth();
	void printBoxes();
	void printTexts();
};

} /* namespace feedback */

#endif /* SCREEN_H_ */
