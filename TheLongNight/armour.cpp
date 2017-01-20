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

armour * headgear_RuinedKnightsHelm()
{
	armour* h = new armour("Ruined Knight's Helm", HELMET_TILE, TCODColor::darkGreen, ITEM_HELMET,
		"This helmet has taken more than a few blows.");
	h->setDefence(10);
	return h;
}

armour * armour_RuinedKnightsArmour()
{
	armour* a = new armour("Ruined Knight's Armour", ARMOUR_TILE, TCODColor::darkGreen, ITEM_BODY_ARMOUR,
		"The heraldry on this armour is so old that it's illegible.");
	a->setDefence(20);
	a->setMoveSpeed(SPEED_NORMAL);
	return a;
}

armour * headgear_ClericsHood()
{
	armour* a = new armour("Cleric's Hood", HELMET_TILE, TCODColor::darkYellow, ITEM_HELMET,
		"It's important to remember your prayers.");
	a->setDefence(0);
	a->setDamageResist(DAMAGE_PROFANE, 5);
	return a;
}

armour * armour_ClericsVestments()
{
	armour* a = new armour("Cleric's Vestments", ARMOUR_TILE, TCODColor::darkYellow, ITEM_BODY_ARMOUR,
		"The clerics of the Lady of the Rose were blessed with certain protections against profane powers, though, of course, that did not save them.");
	a->setDefence(10);
	a->setDamageResist(DAMAGE_PROFANE, 10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armour * headgear_CrowKnightsHood()
{
	armour* a = new armour("Crow Knight's Hood", HELMET_TILE, TCODColor::lightGrey, ITEM_HELMET,
		"The hood of a crow knight is black, and interlaced with singular white feathers.");
	a->setDefence(5);
	a->setBleedResist(10);
	return a;
}

armour * armour_CrowKnightsArmour()
{
	armour* a = new armour("Crow Knight's Armour", ARMOUR_TILE, TCODColor::lightGrey, ITEM_BODY_ARMOUR,
		"The feathered armour of the crow knights is thick and warm. Beady eyes are not required.");
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	a->setBleedResist(15);
	return a;
}
