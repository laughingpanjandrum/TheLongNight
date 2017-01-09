
/*
This class manages the whole game.
It takes care of input handling, time passing, and calling everything else that needs to happen.
It also draws all the stuff that needs drawing.
*/


#ifndef GAME_H
#define GAME_H

#include "libtcod.hpp"
#include "window.h"
#include "map.h"

class game
{
public:
	game();
	~game();
	//Map stuff
	void setCurrentMap(map* newMapP);
	//Game running
	void mainGameLoop();
private:
	window win;
	//Character/state/etc
	map* currentMap;
	//Drawing functions
	void drawScreen();
	void drawMap(int atx, int aty);
	//Draw location constants
	const static int MAP_DRAW_X = 3;
	const static int MAP_DRAW_Y = 3;
};

#endif
