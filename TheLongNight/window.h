
#include <string>
#include "libtcod.hpp"

#ifndef WINDOW_H
#define WINDOW_H

class window
{
public:
	window();
	window(std::string name, int xsize, int ysize);
	//Debug
	void drawFont();
	//Writing
	void writec(int x, int y, char c, TCODColor col, TCODColor bgcol = TCODColor::black);
	void write(int x, int y, std::string txt, TCODColor col, TCODColor bgcol = TCODColor::black);
	int writeWrapped(int x, int y, unsigned int lineLen, std::string txt, TCODColor col, TCODColor bgcol = TCODColor::black);
	void clear();
	void refresh();
	//Utility
	TCODColor mixColors(TCODColor col1, TCODColor col2, float perc);
	bool isPunctuator(char c);
	//Input
	TCOD_key_t getkey();
	//Special shape-drawing functions
	void drawBox(int leftx, int topy, int width, int height, TCODColor col = TCODColor::grey);
};

#endif

