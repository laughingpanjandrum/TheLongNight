#include "game.h"



game::game()
{
	//We start by creating an empty map, just for now
	map* newmap = new map();
	setCurrentMap(newmap);
}


game::~game()
{
}

/*
	MAIN GAME LOOP
*/


void game::mainGameLoop()
{
	//Character create
	player = new person();
	//Draw
	drawScreen();
	win.waitForKeypress();
}

/*
	MAP STUFF
*/

/*
Sets the current map
*/
void game::setCurrentMap(map * newMapP)
{
	currentMap = newMapP;
}

/*
	DRAWING STUFF
*/

/*
Clears, draws the entire screen, then refreshes.
*/
void game::drawScreen() 
{
	win.clear();
	drawMap(MAP_DRAW_X, MAP_DRAW_Y);
	//win.drawFont();
	win.refresh();
}

/*
Just draws the current map.
Input: Coordinates to start drawing at (the top left corner of the map)
*/
void game::drawMap(int atx, int aty)
{	
	//Draw entire map, left to right & top to bottom
	for (int x = 0; x < currentMap->getXSize(); x++) {
		for (int y = 0; y < currentMap->getYSize(); y++) {
			maptile* m = currentMap->getTile(x, y);
			win.writec(atx + x, aty + y, m->tileCode, m->color, m->bgcolor);
		}
	}
}
