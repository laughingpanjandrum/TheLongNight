#include "consumable.h"



consumable::consumable(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_CONSUMABLE, description)
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
	consumable* c = new consumable("Starwater Draught", VIAL_TILE, TCODColor::cyan,
		"Flask of water touched by starlight, which confers healing properties.");
	c->addEffect(RESTORE_HEALTH);
	c->setPotency(25);
	return c;
}

consumable * consumable_InvigoratingTea()
{
	consumable* c = new consumable("Invigorating Tea", VIAL_TILE, TCODColor::green,
		"A specially brewed tea that restores vigour.");
	c->addEffect(RESTORE_VIGOUR);
	c->setPotency(5);
	return c;
}
