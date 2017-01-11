#include "consumable.h"



consumable::~consumable()
{
}

consumable * consumable_StarwaterDraught()
{
	consumable* c = new consumable("Starwater Draught", VIAL_TILE, TCODColor::cyan);
	return c;
}
