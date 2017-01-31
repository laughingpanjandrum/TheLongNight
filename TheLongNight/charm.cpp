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

charm * charm_IdolOfPash()
{
	charm* c = new charm("Idol of Thirst", CHARM_TILE, TCODColor::lightAmber,
		"A small statue depicting a gaping mouth. Increases fragments gained.");
	c->addEffect(CHANGE_FRAGMENT_PICKUP_MULT, 2);
	return c;
}

charm * charm_EvisceratingRing()
{
	charm* c = new charm("Eviscerating Ring", CHARM_TILE, TCODColor::darkRed,
		"This ring depicts the thirsty face of Pash, who is constantly seeking victims to devour.");
	c->addEffect(GAIN_HEALTH_ON_KILL, 15);
	return c;
}

charm * charm_BloodDrinkersBand()
{
	charm* c = new charm("Blood Drinker's Band", CHARM_TILE, TCODColor::crimson,
		"This band is covered in thorns to elicit perpetual bleeding. The devotees of Pash believe that this glorious agony \
will draw one closer to godhood.");
	c->addEffect(GAIN_BLEED_SCALING, 25);
	return c;
}

charm * charm_WretchedFleshBand()
{
	charm* c = new charm("Wretched Fleshband", CHARM_TILE, TCODColor::pink,
		"This band of quivering flesh confers a sense of profound unease. Of course, such is the fate of the wretched, who \
could go no further than Coruscating Beach.");
	c->addEffect(GAIN_HEALTH_ON_KILL, 10);
	return c;
}

charm * charm_WretchedFleshmask()
{
	charm* c = new charm("Wretched Fleshmask", CHARM_TILE, TCODColor::lightPink,
		"Drape a tattered skin across your face, and perhaps you will forget your troubles. Such was the motto of the wretched \
who fled the downfall of the Winter Court.");
	c->addEffect(HEALING_FACTOR, 25);
	return c;
}
