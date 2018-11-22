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
	miniFont = TTF_OpenFont("arial.ttf", miniFontSize);
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

    #ifndef DEBUG
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);   
    #endif


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
	TTF_CloseFont(miniFont);
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
	miniFontSize = s*MINI_SCALE;
	font = TTF_OpenFont("arial.ttf", fontSize);
	miniFont = TTF_OpenFont("arial.ttf", miniFontSize);
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
	SDL_RenderClear(renderer);
	SDL_UpdateTexture(texture, NULL, &buffer[0], SCREEN_WIDTH*sizeof(Uint32));
	SDL_RenderCopy(renderer, texture, NULL, NULL);
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

// Starts form 0
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
	menu("");
}
int Screen::boxWidth() {
	return (SCREEN_WIDTH-(subjects.size()+1)*SUBJECT_SPACING)/(1+(subjects.size()-1)*MINI_SCALE);
}
void Screen::clearBoxes() {
	for (int y = SUBJECT_Y_OFFSET; y < SUBJECT_Y_OFFSET + SUBJECT_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			setPixel(y, x, BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2]);
		}
	}
}
void Screen::printBoxes() {
	clearBoxes();
	int beforeSelection = selection;
	int afterSelection = subjects.size()-1-beforeSelection;
	int bW = boxWidth();
	int miniSize = bW*MINI_SCALE;
	for (int i = 0; i < beforeSelection; i++) {
			for (int y = MINI_Y_OFFSET; y < MINI_HEIGHT+MINI_Y_OFFSET; y++)  {
				for (int x = SUBJECT_SPACING + i*(SUBJECT_SPACING + miniSize); x < (i+1)*(SUBJECT_SPACING + miniSize); x++) {
					setPixel(y, x, BOX_COLOR[0],BOX_COLOR[1],BOX_COLOR[2]);
				}
			}
	}
	for (int y = SUBJECT_Y_OFFSET; y < SUBJECT_HEIGHT+SUBJECT_Y_OFFSET; y++)  {
		for (int x = SUBJECT_SPACING + beforeSelection*(SUBJECT_SPACING + miniSize); x < beforeSelection*(SUBJECT_SPACING + miniSize) + SUBJECT_SPACING + bW; x++) {
			setPixel(y, x, SELECTION_COLOR[0],SELECTION_COLOR[1],SELECTION_COLOR[2]);
		}
	}
	int leftPadding = beforeSelection*(SUBJECT_SPACING + miniSize) + SUBJECT_SPACING*2+bW;
	for (int i = 0; i < afterSelection; i++) {
		for (int y = MINI_Y_OFFSET; y < MINI_HEIGHT+MINI_Y_OFFSET; y++)  {
			for (int x = leftPadding + i*(SUBJECT_SPACING + miniSize); x < leftPadding + i*SUBJECT_SPACING + (i+1)*miniSize; x++) {
				setPixel(y, x, BOX_COLOR[0],BOX_COLOR[1],BOX_COLOR[2]);
			}
		}
	}

}
void Screen::printSubjects() {
	printBoxes();
	refreshFromBuffer();
	printTexts();
	SDL_RenderPresent(renderer);
}

void Screen::selectSubject(int i) {
	selection = i;
	printSubjects();
}
void Screen::selectNext() {
	selection = (selection+1)%subjects.size();
	printSubjects();
}

void Screen::printMiniText(int index) {
	int bW = boxWidth();
	int miniSize = bW*MINI_SCALE;
	int miniPad = TEXT_PADDING*MINI_SCALE;
	SDL_Rect rect;
	SDL_Surface* textSurf = TTF_RenderUTF8_Blended_Wrapped(miniFont, subjects[index].c_str(), TEXT_COLOR, (unsigned int)miniSize-2*miniPad);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, textSurf);
	int texW, texH = 0;
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	rect.w = texW;
	rect.h = texH;
	if (texH >= MINI_HEIGHT) {
		rect.y = MINI_Y_OFFSET + miniPad;
	} else rect.y = MINI_Y_OFFSET + (MINI_HEIGHT - texH)/2;
	if (index > (int)selection) {
		int leftPadding = selection*(SUBJECT_SPACING + miniSize) + SUBJECT_SPACING*2+bW;
		rect.x = leftPadding + (index-selection-1)*(SUBJECT_SPACING + miniSize) + (miniSize-texW)/2;

	} else {
		rect.x = SUBJECT_SPACING + (index)*(SUBJECT_SPACING + miniSize) + (miniSize-texW)/2;
	}
	SDL_RenderCopy(renderer, message, NULL, &rect);
	SDL_DestroyTexture(message);
	SDL_FreeSurface(textSurf);
}
void Screen::printSubjectText() {
	int bW = boxWidth();
	int miniSize = bW * MINI_SCALE;
	SDL_Rect rect;
	//SDL_Surface* textSurf = TTF_RenderText_Blended_Wrapped(font, subjects[i].c_str(), TEXT_COLOR, (unsigned int)SCREEN_WIDTH);
	SDL_Surface* textSurf = TTF_RenderUTF8_Blended_Wrapped(font, subjects[selection].c_str(), TEXT_COLOR, (unsigned int)bW-2*TEXT_PADDING);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, textSurf);
	int texW, texH = 0;
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	rect.w = texW;
	rect.h = texH;
	if (texH >= SUBJECT_HEIGHT) {
		rect.y = SUBJECT_Y_OFFSET + TEXT_PADDING;
	} else rect.y = SUBJECT_Y_OFFSET + (SUBJECT_HEIGHT - texH)/2;
	rect.x = SUBJECT_SPACING+ selection*(SUBJECT_SPACING+miniSize)+(bW - rect.w)/2;
	SDL_RenderCopy(renderer, message, NULL, &rect);
	SDL_DestroyTexture(message);
	SDL_FreeSurface(textSurf);
}
void Screen::printTexts() {
	int beforeSelection = selection;
	int afterSelection = subjects.size()-1-beforeSelection;
	printSubjectText();

	for (int i = 0; i < beforeSelection; i++) {
		printMiniText(i);
	}
	for (int i = 0; i < afterSelection; i++) {
		printMiniText(i+selection+1);
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
	text << "\nCurrent font: " << mainFontSize << "\n\nBindings: a->add subject, r->remove subjects, f->change font, q->quit\n             Enter->start gathering, ESC->back.\n\n";
	text << subject;
	string t = text.str();
	SDL_Rect rect;
	SDL_Surface* textSurf = TTF_RenderUTF8_Blended_Wrapped(font, t.c_str(), TEXT_COLOR, (unsigned int)SCREEN_WIDTH);
	//SDL_Surface* textSurf =  TTF_RenderUTF8_Blended(font, t.c_str(), TEXT_COLOR);
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
		if (x > 5*spacing + 8*rad && x < 5*spacing + 10*rad) return 4;
	}
	int miniSize = boxWidth()*MINI_SCALE;
	if (y > SUBJECT_Y_OFFSET && y < SUBJECT_Y_OFFSET+SUBJECT_HEIGHT) {
		int leftBorder = SUBJECT_SPACING;
		unsigned int added = 0;
		while (leftBorder < x) {
			if (x-leftBorder > SUBJECT_SPACING) {
				if (added != selection) {
					leftBorder += miniSize;
				} else leftBorder += boxWidth();
			if (leftBorder >= x) return added+5;
			else {
				added += 1;
				leftBorder += SUBJECT_SPACING;
			}
			} else return -1;
		}

	}
	return -1;
}


} /* namespace feedback */
