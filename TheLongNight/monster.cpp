#include "monster.h"


/*
Monster definitions
*/

monster * drownedDead()
{
	return new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::darkGreen);
}
