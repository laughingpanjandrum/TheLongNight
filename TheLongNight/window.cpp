#include "window.h"
#include "libtcod.hpp"
#include "tileConsts.h"

window::window() : window("THE LONG NIGHT", 100, 70)
{
}

window::window(std::string name, int xsize, int ysize) {
	TCODSystem::setFps(30);
	//Setup console
	//mainConsole = new TCODConsole(xsize, ysize);
	//mainConsole->setCustomFont("fonts/large_final_10x10.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16, 32);
	TCODConsole::setCustomFont("fonts/large_final_10x10.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16, 32);
	//Go!
	TCODConsole::initRoot(xsize, ysize, name.c_str());
	//Custom font mapping
	int asc = 256;
	for (int y = 16; y < 32; y++) {
		TCODConsole::mapAsciiCodesToFont(asc, 16, 0, y);
		asc += 16;
	}
	//Default bg colour
	TCODConsole::root->setDefaultBackground(SCREEN_BACKGROUND_COLOR);
}

/*
DEBUG
*/

/*
Displays the font we're using, character by character.
*/
void window::drawFont() {
	int atx = 1;
	int aty = 1;
	for (int c = 1; c < 255; c++) {
		//Numeral
		write(atx, aty, std::to_string(c), TCODColor::lightGreen);
		//Character
		writec(atx + 3, aty++, c, TCODColor::green);
		if (c % 64 == 0) {
			atx += 10;
			aty = 1;
		}
	}
	//Refresh and wait
	refresh();
	TCOD_console_wait_for_keypress(false);
}




/*
WRITING
*/



/*
Put a character at the specified screen location.
Input: x, y coordinates in the window; a character; a foreground and background color.
*/
void window::writec(int x, int y, int c, TCODColor col, TCODColor bgcol, bool toAltConsole) {
	//TCODConsole::putCharEx(nullptr, x, y, c, col, bgcol);
	TCODConsole::root->putCharEx(x, y, c, col, bgcol);
}

/*
Put a string at the specified screen location.
Input: x, y coordinates in the window; a string of text; a foreground and background color.
Output: None.
*/
void window::write(int x, int y, std::string txt, TCODColor col, TCODColor bgcol, bool toAltConsole) {
	for (unsigned int i = 0; i < txt.size(); i++) {
		writec(x, y, txt.at(i), col, bgcol, toAltConsole);
		x++;
	}
}

/*
Puts a string at the specified location. Wraps string to given line length.
Returns the last y-coordinate we draw on.
*/
int window::writeWrapped(int x, int y, unsigned int lineLen, std::string txt, TCODColor col, TCODColor bgcol, bool toAltConsole)
{
	
	//Break string up into chunks, each of which (ideally) ends with a space.
	std::string word = "";
	std::string line = "";
	
	for (unsigned int i = 0; i < txt.size(); i++) {
	
		//Add new word
		word += txt.at(i);
		if (isPunctuator(txt.at(i))) {
		
			//End of word; will this exceed the max line length?
			std::string possibleLine = line + word;
			if (possibleLine.size() >= lineLen) {
			
				//Write last line and wrap
				write(x, y, line, col, bgcol);
				y++;
				line = word;
			
			}
			
			else {
				//Append to current line and continue
				line += word;
			}
			
			word = "";
		}
	}
	
	//Write the last line when we're done
	if (word.size())
		line += word;
	if (line.size())
		write(x, y, line, col, bgcol, toAltConsole);
	return y;
}

/*
Flush the root console.
Input: None.
Output: None.
*/
void window::refresh() 
{
	TCODConsole::flush();
}



/*
Clears only the specified region.
*/
void window::clearRegion(int lx, int ty, int w, int h)
{
	for (int x = lx; x < lx + w; x++) {
		for (int y = ty; y < ty + h; y++) {
			TCODConsole::root->putCharEx(x, y, 0, TCODColor::black, TCODColor::black);
		}
	}
}

/*
Clear the root console.
Input: None.
Output: None.
*/
void window::clear() {
	TCODConsole::root->clear();
	//mainConsole->clear();
}

/*
GETTING INPUT
*/

/*
This is a super klutzy function that constantly clears the whole screen
It's pretty much just for debugging purposes
*/
std::string window::getstr(int x, int y, std::string prompt)
{
	std::string txt = "";
	clear();
	write(x, y, prompt + "_", TCODColor::white);
	refresh();
	TCOD_key_t key = TCODConsole::waitForKeypress(false);
	while (key.vk != TCODK_ENTER || !key.pressed) {
		//SPECIAL KEYS
		if (key.pressed) {
			if (key.vk == TCODK_BACKSPACE) {
				if (txt.size())
					txt.pop_back();
			}
			else if (key.vk == TCODK_SHIFT || key.vk == TCODK_ENTER) {
				//do nothing
			}
			else
				txt += key.c;
		}
		//Show what we did
		clear();
		write(x, y, prompt + txt + '_', TCODColor::white);
		refresh();
		key = TCODConsole::waitForKeypress(false);
	}
	return txt;
}

/*
Get a single character from the console.
Input: None.
Output: Character input by the user.
*/
TCOD_key_t window::getkey() {
	return TCODConsole::checkForKeypress();
}

/*
Get a single character from a console. Freezes until input is recieved.
*/
TCOD_key_t window::waitForKeypress()
{
	TCODConsole::waitForKeypress(true);
	return TCODConsole::waitForKeypress(true);
}

/*
UTILITY
*/

TCODColor window::mixColors(TCODColor col1, TCODColor col2, float perc) {
	return TCODColor::lerp(col1, col2, perc);
}

bool window::isPunctuator(char c)
{
	return c == ' ' || c == ',' || c == '.' || c == ';' || c == ':';
}

/*
SPECIAL DRAWING FUNCTIONS
*/

void window::drawBox(int leftx, int topy, int width, int height, TCODColor col) 
{

	//Fill in background
	for (int x = leftx; x < leftx + width; x++) {
		for (int y = topy; y < topy + height; y++) {
			TCODConsole::root->setCharBackground(x, y, BOX_BACKGROUND_COLOR);
		}
	}
	
	//Corners
	writec(leftx, topy, TOP_LEFT_CORNER, BORDER_COLOR);
	writec(leftx + width, topy, TOP_RIGHT_CORNER, BORDER_COLOR);
	writec(leftx, topy + height, BOTTOM_LEFT_CORNER, BORDER_COLOR);
	writec(leftx + width, topy + height, BOTTOM_RIGHT_CORNER, BORDER_COLOR);
	
	//Horizontal sides
	for (int x = leftx + 1; x < leftx + width; x++) {
		writec(x, topy, HORIZONTAL_BAR, BORDER_COLOR);
		writec(x, topy + height, HORIZONTAL_BAR, BORDER_COLOR);
	}
	
	//Vertical sides
	for (int y = topy + 1; y < topy + height; y++) {
		writec(leftx, y, VERTICAL_BAR, BORDER_COLOR);
		writec(leftx + width, y, VERTICAL_BAR, BORDER_COLOR);
	}

}

/*
Draws a counter (as defined in counter.h).
Can place a title on top, and displays col1 for the section that's full, col2 for the empty section.
maxSize is the length of the bar we'll draw.
*/
void window::drawCounter(counter c, std::string title, int x, int y, TCODColor col1, TCODColor col2, int maxSize)
{
	//Draw title first
	write(x, y, title, col1);
	x += title.size();
	//Figure out how many tiles to draw 'full' and how many to draw 'empty'
	float frac = c.getPercent();
	int toDrawFull = maxSize * frac;
	int toDrawEmpty = maxSize - toDrawFull;
	//And then draw them!
	int i = 0;
	for (i; i < toDrawFull; i++) {
		writec(x + i, y, FULL_COUNTER_TILE, col1);
	}
	for (i; i < maxSize; i++) {
		writec(x + i, y, EMPTY_COUNTER_TILE, BOX_BACKGROUND_COLOR);
	}
}



/*
Blit an image onto the console.
*/
void window::drawImage(imageSharedPtr img, int atx, int aty)
{
	img->blitRect(TCODConsole::root, atx, aty);
}

void window::drawImage(imagePtr img, int atx, int aty)
{
	img->blitRect(TCODConsole::root, atx, aty);
}
