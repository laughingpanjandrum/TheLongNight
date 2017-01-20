#include "charm.h"



charm::~charm()
{
}

void charm::addEffect(effect e, int pot)
{
	effects.push_back(e);
	effectPotency.push_back(pot);
}




/*
	ITEMS
*/


charm * charm_BloodstainedCharm()
{
	charm* c = new charm("Blood-stained Charm", CHARM_TILE, TCODColor::crimson,
		"A charm drenched in blood, which dissipates upon contact with air.");
	c->setBleedResist(15);
	c->setDefence(-10);
	return c;
}
