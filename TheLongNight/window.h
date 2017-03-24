
#include <string>
#include "libtcod.hpp"
#include "counter.h"
#include "images.h"


//Colour constants
#define SCREEN_BACKGROUND_COLOR TCODColor(12,17,18)
#define BOX_BACKGROUND_COLOR TCODColor(23,26,25)
#define BORDER_COLOR TCODColor(28,45,52)

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
	void writec(int x, int y, int c, TCODColor col, TCODColor bgcol = BOX_BACKGROUND_COLOR, bool toAltConsole = false);
	void write(int x, int y, std::string txt, TCODColor col, TCODColor bgcol = BOX_BACKGROUND_COLOR, bool toAltConsole = false);
	int writeWrapped(int x, int y, unsigned int lineLen, std::string txt, TCODColor col, TCODColor bgcol = BOX_BACKGROUND_COLOR,
		bool toAltConsole = false);
	
	//Clearing/refreshing
	void clearRegion(int x, int y, int w, int h);
	void clear();
	void refresh();
	
	//Utility
	TCODColor mixColors(TCODColor col1, TCODColor col2, float perc);
	bool isPunctuator(char c);
	
	//Input
	std::string getstr(int x, int y, std::string prompt = "~");
	TCOD_key_t getkey();
	TCOD_key_t waitForKeypress();
	
	//Special shape-drawing functions
	void drawBox(int leftx, int topy, int width, int height, TCODColor col = TCODColor::grey);
	void drawCounter(counter c, std::string title, int x, int y, TCODColor col1, TCODColor col2, int maxSize = 10);

	//Image drawing
	void drawImage(imageSharedPtr img, int atx, int aty);
	void drawImage(imagePtr img, int atx, int aty);

private:

	//Constants used in drawing
	int BOTTOM_LEFT_CORNER = 177;// 200;
	int TOP_LEFT_CORNER = 177;//201;
	int TOP_RIGHT_CORNER = 177;//187;
	int BOTTOM_RIGHT_CORNER = 177;//188;
	int VERTICAL_BAR = 177;//186;
	int HORIZONTAL_BAR = 177;//205;

};

#endif

