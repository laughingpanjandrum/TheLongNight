#include "game.h"



game::game()
{
	//We start by creating an empty map, just for now
	map* newmap = new map();
	setCurrentMap(newmap);
	//Add some random walls and shit
	currentMap->setTile(wall(), 5, 5);
	currentMap->setTile(wall(), 6, 5);
	currentMap->setTile(wall(), 8, 8);
	currentMap->setTile(wall(), 10, 8);
	//Character create
	player = new person();
	currentMap->addPerson(player, 3, 3);
}


game::~game()
{
}

/*
	MAIN GAME LOOP
*/


void game::mainGameLoop()
{
	//Draw
	while (!isGameOver) {
		drawScreen();
		processCommand();
	}
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
			win.writec(leftx + x, topy + y, m->getTileCode(), m->getColor(), m->getBgColor());
		}
	}
	//Now draw stuff over top, i.e. characters, items, etc.
	for (auto p : currentMap->getAllPeople()) {
		win.writec(leftx + p->getx(), topy + p->gety(), p->getTileCode(), p->getColor());
	}
}

/*
	COMMAND PROCESSING
*/

void game::processCommand()
{
	TCOD_key_t kp = win.getkey();
	if (kp.c == 'Q')
		isGameOver = true;
	//Movement
	else if (isMovementKey(kp))
		processMove(kp);
}

/*
Movement commands
*/
void game::processMove(TCOD_key_t kp)
{
	//Determine new position
	int xnew = player->getx();
	int ynew = player->gety();
	if (kp.vk == KEY_NORTH)
		ynew--;
	else if (kp.vk == KEY_SOUTH)
		ynew++;
	else if (kp.vk == KEY_EAST)
		xnew++;
	else if (kp.vk == KEY_WEST)
		xnew--;
	//And then move there!
	movePlayer(xnew, ynew);
}

/*
Returns whether the given key is a movement command
*/
bool game::isMovementKey(TCOD_key_t kp)
{
	return kp.vk == KEY_NORTH || kp.vk == KEY_EAST || kp.vk == KEY_SOUTH || kp.vk == KEY_WEST;
}

/*
Change player character's position, if the move is valid.
*/
void game::movePlayer(int xnew, int ynew)
{
	//In bounds?
	if (currentMap->inBounds(xnew, ynew)) {
		//Possible to walk on?
		if (currentMap->isWalkable(xnew, ynew)) {
			player->setPosition(xnew, ynew);
		}
	}
}
