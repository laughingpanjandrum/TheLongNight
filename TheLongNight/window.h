
#include <string>
#include "libtcod.hpp"
#include "counter.h"

#ifndef WINDOW_H
#define WINDOW_H

class window
{
public:

	window();
	window(std::string name, int xsize, int ysize);

	//Debug
	void drawFont();

	//Console
	TCODConsole* mainConsole;
	
	//Writing
	void writec(int x, int y, int c, TCODColor col, TCODColor bgcol = TCODColor::black, bool toAltConsole = false);
	void write(int x, int y, std::string txt, TCODColor col, TCODColor bgcol = TCODColor::black, bool toAltConsole = false);
	int writeWrapped(int x, int y, unsigned int lineLen, std::string txt, TCODColor col, TCODColor bgcol = TCODColor::black,
		bool toAltConsole = false);
	
	//Clearing/refreshing
	void clear();
	void refresh();
	
	//Utility
	TCODColor mixColors(TCODColor col1, TCODColor col2, float perc);
	bool isPunctuator(char c);
	
	//Input
	std::string getstr(int x, int y);
	TCOD_key_t getkey();
	TCOD_key_t waitForKeypress();
	
	//Special shape-drawing functions
	void drawBox(int leftx, int topy, int width, int height, TCODColor col = TCODColor::grey);
	void drawCounter(counter c, std::string title, int x, int y, TCODColor col1, TCODColor col2, int maxSize = 10);

};

#endif

