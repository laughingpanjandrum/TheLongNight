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
	armour* a = new armour("Cleric's Hood", HOOD_TILE, TCODColor::lighterYellow, ITEM_HELMET,
		"It's important to remember your prayers.");
	a->setDefence(0);
	a->setDamageResist(DAMAGE_PROFANE, 5);
	return a;
}

armour * armour_ClericsVestments()
{
	armour* a = new armour("Cleric's Vestments", ROBE_TILE, TCODColor::lighterYellow, ITEM_BODY_ARMOUR,
		"The clerics of the Lady of the Rose were blessed with certain protections against profane powers, though, of course, that did not save them.");
	a->setDefence(10);
	a->setDamageResist(DAMAGE_PROFANE, 10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armour * headgear_CrowKnightsHood()
{
	armour* a = new armour("Crow Knight's Hood", HOOD_TILE, TCODColor::lightGrey, ITEM_HELMET,
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
	a->setBleedResist(15);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armour * headgear_FishpriestHat()
{
	armour* a = new armour("Fishpriest Hat", HOOD_TILE, TCODColor::lime, ITEM_HELMET,
		"Scaly hat of the preachers of doom, whose warnings about the obliteration of Wyrd were ignored.");
	a->setDefence(5);
	a->setBleedResist(5);
	a->setDamageResist(DAMAGE_ACID, 5);
	return a;
}

armour * armour_FishscaleCoat()
{
	armour* a = new armour("Fishscale Coat", ROBE_TILE, TCODColor::lightLime, ITEM_BODY_ARMOUR,
		"Scaly coat of the preachers of doom, whose pleas for the safety of Wyrd went unanswered.");
	a->setDefence(10);
	a->setBleedResist(10);
	a->setDamageResist(DAMAGE_ACID, 10);
	a->setMoveSpeed(SPEED_NORMAL);
	return a;
}

armour * headgear_CityGuardHelm()
{
	armour* a = new armour("City Guard Helm", HELMET_TILE, TCODColor::darkYellow, ITEM_HELMET,
		"In the final days of the Winter Court, the faces of the guards were stripped of flesh, and their bare bone was \
exposed to the devouring gaze of Pash.");
	a->setDefence(15);
	a->setDamageResist(DAMAGE_ELECTRIC, 10);
	a->setDamageResist(DAMAGE_FIRE, 10);
	return a;
}

armour * armour_CityGuardArmour()
{
	armour* a = new armour("City Guard Armour", ARMOUR_TILE, TCODColor::darkYellow, ITEM_BODY_ARMOUR,
		"The guards of the city infused their armour to protect themselves against the profaned slaves of Pash.");
	a->setDefence(30);
	a->setDamageResist(DAMAGE_ELECTRIC, 20);
	a->setDamageResist(DAMAGE_FIRE, 25);
	a->setMoveSpeed(SPEED_SLOW);
	return a;
}

armour * headgear_PashHood()
{
	armour* a = new armour("Pash Hood", HOOD_TILE, TCODColor::lightPurple, ITEM_HELMET,
		"Hood that shields the eyes of the devotees of Pash. According to certain storytellers, in the final days of the Winter \
Court, the face of Pash itself appeared in the sky, devouring the stars.");
	a->setDefence(5);
	a->setDamageResist(DAMAGE_COLD, 10);
	a->setDamageResist(DAMAGE_BLESSED, 10);
	return a;
}

armour * armour_PashRobes()
{
	armour* a = new armour("Pash Robes", ROBE_TILE, TCODColor::lightPurple, ITEM_BODY_ARMOUR,
		"Robe of the devotees of Pash. When the Void first reached the Winter Court, these devotees began to appear in the \
streets, though none knew their origins.");
	a->setDefence(5);
	a->setDamageResist(DAMAGE_COLD, 15);
	a->setDamageResist(DAMAGE_BLESSED, 15);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}
