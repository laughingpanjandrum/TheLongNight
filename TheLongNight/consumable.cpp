#include "consumable.h"



consumable::consumable(std::string name, int tileCode, TCODColor color) :
	item(name, tileCode, color, ITEM_CONSUMABLE)
{
	amount.setTo(1);
}

consumable::~consumable()
{
}

/*
Returns amount left combined with name.
*/
std::string consumable::getMenuName()
{
	return getName() + " x" + std::to_string(getAmountLeft());
}

consumable * consumable_StarwaterDraught()
{
	consumable* c = new consumable("Starwater Draught", VIAL_TILE, TCODColor::cyan);
	return c;
}

consumable * consumable_InvigoratingTea()
{
	consumable* c = new consumable("Invigorating Tea", VIAL_TILE, TCODColor::green);
	return c;
}
