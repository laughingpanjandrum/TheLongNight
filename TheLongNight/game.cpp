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
	currentMap->addPerson(player, 3, 3);
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
void game::drawMap(int leftx, int topy)
{
	//Draw entire map, left to right & top to bottom
	for (int x = 0; x < currentMap->getXSize(); x++) {
		for (int y = 0; y < currentMap->getYSize(); y++) {
			maptile* m = currentMap->getTile(x, y);
			win.writec(leftx + x, topy + y, m->tileCode, m->color, m->bgcolor);
		}
	}
	//Now draw stuff over top, i.e. characters, items, etc.
	for (auto p : currentMap->getAllPeople()) {
		win.writec(leftx + p->getx(), topy + p->gety(), p->getTileCode(), p->getColor());
	}
}
