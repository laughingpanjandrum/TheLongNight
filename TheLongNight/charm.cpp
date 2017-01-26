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

charm * charm_KhallesHeadband()
{
	charm* c = new charm("Khalle's Headband", CHARM_TILE, TCODColor::lightestPink,
		"This band of flowers confers a curious sense of peace.");
	c->addEffect(GAIN_MAX_HEALTH, 20);
	return c;
}
