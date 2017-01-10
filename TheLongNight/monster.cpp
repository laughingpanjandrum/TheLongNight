#include "monster.h"

monster::monster() : person()
{
}

monster::monster(std::string name, int tileCode, TCODColor color) : 
	person(name, tileCode, color)
{
	//We're not the player! That's kind of our defining feature
	isPlayer = false;
}

/*
Monster definitions
*/

monster * drownedDead()
{
	return new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::darkGreen);
}

