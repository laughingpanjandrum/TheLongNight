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
