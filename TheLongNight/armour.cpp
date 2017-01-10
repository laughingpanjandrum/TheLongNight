#include "armour.h"




armour::armour(std::string name, int tileCode, TCODColor color, itemTypes slot) :
	item(name, tileCode, color, slot)
{
}

armour::~armour()
{
}

/*
	ARMOUR OBJECTS
*/

armour * armour_RuinedUniform()
{
	armour* a = new armour("Ruined Uniform", ARMOUR_TILE, TCODColor::lightBlue, ITEM_BODY_ARMOUR);
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}
