#include "armour.h"




armour::armour(std::string name, int tileCode, TCODColor color, itemTypes slot, std::string description) :
	item(name, tileCode, color, slot, description)
{
}

armour::~armour()
{
}

/*
	ARMOUR OBJECTS
*/

armour * headgear_CaptainsTricorn()
{
	armour* h = new armour("Captain's Tricorn", HELMET_TILE, TCODColor::lightBlue, ITEM_HELMET,
		"Ornate tricorn hat, intended to make the stardrift captain more imposing to his subordinates.");
	h->setDefence(5);
	return h;
}

armour * armour_RuinedUniform()
{
	armour* a = new armour("Ruined Uniform", ARMOUR_TILE, TCODColor::lightBlue, ITEM_BODY_ARMOUR,
		"A relic of eras prior to the annihilation of time, when the stardrift sigil meant something.");
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armour * headgear_CrowKnightsHood()
{
	armour* a = new armour("Crow Knight's Hood", HELMET_TILE, TCODColor::lightGrey, ITEM_HELMET,
		"The hood of a crow knight is black, and interlaced with singular white feathers.");
	a->setDefence(5);
	return a;
}

armour * armour_CrowKnightsArmour()
{
	armour* a = new armour("Crow Knight's Armour", ARMOUR_TILE, TCODColor::lightGrey, ITEM_BODY_ARMOUR,
		"The feathered armour of the crow knights is thick and warm. Beady eyes are not required.");
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}
