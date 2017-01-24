#include "miscItem.h"



miscItem::~miscItem()
{
}


/*
	SPECIFIC ITEMS
*/

miscItem * key_WretchsKey()
{
	miscItem* m = new miscItem("Wretch's Key", KEY_TILE, TCODColor::green,
		"Key treasured by the wretches who fled the Winter Court, but could go no further than Coruscating Beach.");
	m->isKey = true;
	m->setKeyTag("wretched_door");
	return m;
}

miscItem * key_OldCrowsKey()
{
	miscItem* m = new miscItem("Old Crow's Key", KEY_TILE, TCODColor::lightGrey,
		"The Winter Court entrusted the crows with this key, and begged them to safeguard their lands against the scourge.");
	m->isKey = true;
	m->setKeyTag("crow_door");
	return m;
}

miscItem * key_RuinedTownshipKey()
{
	miscItem* m = new miscItem("Ruined Township Key", KEY_TILE, TCODColor::darkLime,
		"Someone entrusted the guardian golem with the safekeeping of this key, and it persisted even after the end of the world.");
	m->isKey = true;
	m->setKeyTag("ruined_township_hall_door");
	return m;
}



//Other



miscItem * misc_VoidSigil()
{
	miscItem* m = new miscItem("Void Sigil Key", SIGIL_TILE, TCODColor::lightPurple,
		"Sign of certain voidwalkers. Those who possess this key are permitted to use the void warping stones.");
	m->isKey = true;
	m->setKeyTag("warpstone");
	return m;
}
