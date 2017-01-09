#include "window.h"
#include "libtcod.hpp"

window::window() : window("THE LONG NIGHT", 100, 70)
{
}

window::window(std::string name, int xsize, int ysize) {
	TCODConsole::setCustomFont("fonts/terminal.png", TCOD_FONT_LAYOUT_ASCII_INCOL, 16, 16);
	TCODSystem::setFps(30);
	TCODConsole::initRoot(xsize, ysize, name.c_str());
	//drawFont();
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
	for (int c = 1; c < 254; c++) {
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
void window::writec(int x, int y, char c, TCODColor col, TCODColor bgcol) {
	TCODConsole::root->putCharEx(x, y, c, col, bgcol);
}

/*
Put a string at the specified screen location.
Input: x, y coordinates in the window; a string of text; a foreground and background color.
Output: None.
*/
void window::write(int x, int y, std::string txt, TCODColor col, TCODColor bgcol) {
	for (unsigned int i = 0; i < txt.size(); i++) {
		writec(x, y, txt.at(i), col, bgcol);
		x++;
	}
}

/*
Puts a string at the specified location. Wraps string to given line length.
Returns the last y-coordinate we draw on.
*/
int window::writeWrapped(int x, int y, unsigned int lineLen, std::string txt, TCODColor col, TCODColor bgcol)
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
			if (possibleLine.size() > lineLen) {
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
		write(x, y, line, col, bgcol);
	return y;
}

/*
Flush the root console.
Input: None.
Output: None.
*/
void window::refresh() {
	TCODConsole::flush();
}

/*
Clear the root console.
Input: None.
Output: None.
*/
void window::clear() {
	TCODConsole::root->clear();
}

/*
GETTING INPUT
*/

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

void window::drawBox(int leftx, int topy, int width, int height, TCODColor col) {
	//Stars in the four corners
	writec(leftx, topy, '*', col);
	writec(leftx + width, topy, '*', col);
	writec(leftx, topy + height, '*', col);
	writec(leftx + width, topy + height, '*', col);
	//Horizontal sides
	for (int x = leftx + 1; x < leftx + width; x++) {
		writec(x, topy, '-', col);
		writec(x, topy + height, '-', col);
	}
	//Vertical sides
	for (int y = topy + 1; y < topy + height; y++) {
		writec(leftx, y, '|', col);
		writec(leftx + width, y, '|', col);
	}
}