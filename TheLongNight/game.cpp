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
	drawMap();
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

void game::drawMap()
{
	win.clear();
	int atx = 10;
	int aty = 10;
	//Draw entire map, left to right
	for (int x = 0; x < currentMap->getXSize(); x++) {
		for (int y = 0; y < currentMap->getYSize(); y++) {
			maptile* m = currentMap->getTile(x, y);
			win.writec(atx + x, aty + y, m->tile, m->color, m->bgcolor);
		}
	}
	win.refresh();
}
