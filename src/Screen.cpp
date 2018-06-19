/*
 * Screen.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#include "Screen.h"
#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;
namespace feedback {

Screen::Screen(int fontSize): fontSize(fontSize) {
	TTF_Init();
	font = TTF_OpenFont("arial.ttf", fontSize);
	if (font == NULL) {
		cout << "Font not found" << endl;
		throw 20;
	}

	window = SDL_CreateWindow("Feedback",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window==NULL)  {
		cout << "Window not initialised." << endl;
		destroy();
		throw 10;
	}
	// Only to be used in the final screen with correct dimensions.
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture==NULL || renderer == NULL) {
		cout << "Initialising the renderer failed." << endl;
		destroy();
		throw 11;
	}
	buffer = vector<Uint32>(SCREEN_WIDTH*SCREEN_HEIGHT, 0);
	// To make the background prettier.
	printBackground();
	refreshFromBuffer();

}
Screen::Screen(): Screen(20) {
}
Screen::~Screen() {
	destroy();
	TTF_CloseFont(font);
	TTF_Quit();
}
void Screen::printBackground() {
	for (int y =0; y < SCREEN_HEIGHT; y++) {
			for (int x =0; x < SCREEN_WIDTH; x++) {
				setPixel(y, x, BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2]);
			}
	}
}

void Screen::changeFont(int s) {
	TTF_CloseFont(font);
	fontSize = s;
	font = TTF_OpenFont("arial.ttf", fontSize);
	if (font == NULL) {
		cout << "Font not found" << endl;
		throw 20;
	}
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

void Screen::refreshFromBuffer() {
	SDL_UpdateTexture(texture, NULL, &buffer[0], SCREEN_WIDTH*sizeof(Uint32));
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Screen::colorSetP(int x, int y, int r, int g, int b, int rad) {
	setPixel(y, x, r, g, b);
}

typedef int32_t s32;

void Screen::drawCircle(s32 _x, s32 _y, s32 radius, int r, int g, int b)
{
   s32 x = radius - 1;
   s32 y = 0;
   s32 tx = 1;
   s32 ty = 1;
   s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
                                 // doubles the value. == tx - diameter
   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      colorSetP(_x + x, _y - y, r, g, b, radius);
      colorSetP(_x + x, _y + y, r, g, b, radius);
      colorSetP(_x - x, _y - y, r, g, b, radius);
      colorSetP(_x - x, _y + y, r, g, b, radius);
      colorSetP(_x + y, _y - x, r, g, b, radius);
      colorSetP(_x + y, _y + x, r, g, b, radius);
      colorSetP(_x - y, _y - x, r, g, b, radius);
      colorSetP(_x - y, _y + x, r, g, b, radius);

      if (err <= 0)
      {
         y++;
         err += ty;
         ty += 2;
      }
      else if (err > 0)
      {
         x--;
         tx += 2;
         err += tx - (radius << 1);
      }
   }
}


void Screen::printButtons() {


	for (int i = 0; i < 5; i++) {
		printButton(butY, rad, spacing, i);
	}
	refreshFromBuffer();
}

void Screen::printButton(const int y, const int rad, const int spacing, int i) {
	int x = spacing+rad + (spacing+2*rad)*i;
	for (int r = 0; r <= rad; r++) {
		double ind = (1.6-pow((double)r/rad,4))/1.6;
		drawCircle(x, y, r, ind*col[i][0], ind*col[i][1], ind*col[i][2]);
	}
}

void Screen::pressButton(const int y, const int rad, const int spacing, int i) {
	int x = spacing+rad + (spacing+2*rad)*i;
	for (int r = 0; r <= rad; r++) {
		double ind = (1.6-pow((double)r/rad,4))/3.0;
		drawCircle(x, y, r, ind*col[i][0], ind*col[i][1], ind*col[i][2]);
	}
}

void Screen::pressButton(int i) {
	pressButton(butY, rad, spacing, i);
	printSubjects();
}

void Screen::unpressButton(int i) {
	printButton(butY, rad, spacing, i);
	selectNext();
}
void Screen::addSubject(std::string subject) {
	subjects.push_back(subject);
}
void Screen::removeSubjects() {
	subjects = vector<string>();
}
int Screen::boxWidth() {
	return (SCREEN_WIDTH-(subjects.size()+1)*SUBJECT_SPACING)/subjects.size();;
}

void Screen::printBoxes() {
	for (unsigned int i = 0; i < subjects.size(); i++) {
		if (i != selection) {
			for (int y = SUBJECT_Y_OFFSET; y < SUBJECT_HEIGHT+SUBJECT_Y_OFFSET; y++)  {
				for (unsigned int x = SUBJECT_SPACING + i*(SUBJECT_SPACING + boxWidth()); x < (i+1)*(SUBJECT_SPACING + boxWidth()); x++) {
					setPixel(y, x, BOX_COLOR[0],BOX_COLOR[1],BOX_COLOR[2]);
				}
			}
		}
	}
	for (int y = SUBJECT_Y_OFFSET; y < SUBJECT_HEIGHT+SUBJECT_Y_OFFSET; y++)  {
		for (unsigned int x = SUBJECT_SPACING + selection*(SUBJECT_SPACING + boxWidth()); x < (selection+1)*(SUBJECT_SPACING + boxWidth()); x++) {
			setPixel(y, x, SELECTION_COLOR[0],SELECTION_COLOR[1],SELECTION_COLOR[2]);
		}
	}


}
void Screen::printSubjects() {
	printBoxes();
	refreshFromBuffer();
	printTexts();
}

void Screen::selectSubject(int i) {
	selection = i;
	printBoxes();
	refreshFromBuffer();
	printTexts();
}
void Screen::selectNext() {
	selection = (selection+1)%subjects.size();
	printSubjects();
}

void Screen::printTexts() {

	for (unsigned int i = 0; i < subjects.size(); i++) {
		SDL_Rect rect;
		SDL_Surface* textSurf = TTF_RenderText_Blended_Wrapped(font, subjects[i].c_str(), TEXT_COLOR, (unsigned int)SCREEN_WIDTH);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, textSurf);
		int texW, texH = 0;
		SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
		rect.w = texW;
		rect.h = texH;
		rect.y = SUBJECT_Y_OFFSET + (SUBJECT_HEIGHT-texH)/2;
		rect.x = SUBJECT_SPACING+ i*(SUBJECT_SPACING+boxWidth())+(boxWidth() - rect.w)/2;
		SDL_RenderCopy(renderer, message, NULL, &rect);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(message);
		SDL_FreeSurface(textSurf);
	}
}
unsigned int Screen::getSelection() {
	return selection;
}

void Screen::menu(string subject) {
	printBackground();
	stringstream text;
	text << "Current subjects: ";
	for (unsigned int i = 0; i < subjects.size(); i++) {
		text << subjects[i] << "  ";
	}
	text << "\nCurrent font: " << mainFontSize << "\n\nBindings: a->add subject, r->remove subjects, f->change font\n             Enter->start gathering, ESC->back.\n\n";
	text << subject;
	string t = text.str();
	SDL_Rect rect;
	SDL_Surface* textSurf = TTF_RenderText_Blended_Wrapped(font, t.c_str(), TEXT_COLOR, (unsigned int)SCREEN_WIDTH);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, textSurf);
	int texW, texH = 0;
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	rect.w = texW;
	rect.h = texH;
	rect.y = SUBJECT_Y_OFFSET;
	rect.x = SUBJECT_Y_OFFSET;
	SDL_RenderClear(renderer);
	refreshFromBuffer();
	SDL_RenderCopy(renderer, message, NULL, &rect);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(message);
	SDL_FreeSurface(textSurf);
}

/*
 * Buttons are indexed: 0-4 are the score buttons. Anything over that points to a subject in their order in the subjects vector. -1 is returned for no button.
 */
int Screen::getButton(int y, int x) {
	if (y<0 || x<0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return -1;
	if (y > butY-rad && y < butY+rad) {
		if (x > spacing && x < spacing + 2*rad) return 0;
		if (x > 2*(spacing + rad) && x < 2*spacing + 4*rad) return 1;
		if (x > 3*spacing + 4*rad && x < 3*spacing + 6*rad) return 2;
		if (x > 4*spacing + 6*rad && x < 4*spacing + 8*rad) return 3;
		if (x > 5*spacing + 8*rad && x < 5*spacing + 10*rad) return 3;
	}
	if (y > SUBJECT_Y_OFFSET && y < SUBJECT_Y_OFFSET+SUBJECT_HEIGHT) {
		double xOff = x*1.0/(boxWidth()+SUBJECT_SPACING);
		if (xOff > subjects.size() || x < SUBJECT_SPACING) return -1;
		if ((int)(xOff - SUBJECT_SPACING/(SUBJECT_SPACING + boxWidth())) == (int)xOff) return 5+ (int)xOff;

	}
	return -1;
}


} /* namespace feedback */
