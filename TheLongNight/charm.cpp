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


charmSharedPtr charm_ArcanaDrenchedCharm()
{
	charmSharedPtr c(new charm("Arcana-drenched Charm", CHARM_TILE, TCODColor::magenta,
		"No arcanist would be caught dead without this charm, which draws upon ancient and forgotten powers."));
	c->addEffect(INCREASE_SPELL_POWER, 25);
	return c;
}

charmSharedPtr charm_ClericsHolyPendant()
{
	charmSharedPtr c(new charm("Cleric's Holy Pendant", CHARM_TILE, TCODColor::lightYellow,
		"All clerics of the Rose wear this pendant, which draws upon the incomprehensible power of the old gods."));
	c->addEffect(INCREASE_PRAYER_POWER, 25);
	return c;
}

charmSharedPtr charm_BloodstainedCharm()
{
	charmSharedPtr c(new charm("Blood-stained Charm", CHARM_TILE, TCODColor::crimson,
		"A charm drenched in blood, which dissipates upon contact with air."));
	c->setBleedResist(15);
	c->setDefence(-10);
	return c;
}

charmSharedPtr charm_KhallesHeadband()
{
	charmSharedPtr c(new charm("Khalle's Headband", CHARM_TILE, TCODColor::lightestPink,
		"This band of flowers confers a curious sense of peace."));
	c->addEffect(GAIN_MAX_HEALTH, 20);
	return c;
}

charmSharedPtr charm_IdolOfPash()
{
	charmSharedPtr c(new charm("Idol of Thirst", CHARM_TILE, TCODColor::lightAmber,
		"A small statue depicting a gaping mouth. Increases fragments gained."));
	c->addEffect(CHANGE_FRAGMENT_PICKUP_MULT, 2);
	return c;
}

charmSharedPtr charm_EvisceratingRing()
{
	charmSharedPtr c(new charm("Eviscerating Ring", CHARM_TILE, TCODColor::darkRed,
		"This ring depicts the thirsty face of Pash, who is constantly seeking victims to devour."));
	c->addEffect(GAIN_HEALTH_ON_KILL, 15);
	return c;
}

charmSharedPtr charm_BloodDrinkersBand()
{
	charmSharedPtr c(new charm("Blood Drinker's Band", CHARM_TILE, TCODColor::crimson,
		"This band is covered in thorns to elicit perpetual bleeding. The devotees of Pash believe that this glorious agony \
will draw one closer to godhood."));
	c->addEffect(GAIN_BLEED_SCALING, 25);
	return c;
}

charmSharedPtr charm_WretchedFleshBand()
{
	charmSharedPtr c(new charm("Wretched Fleshband", CHARM_TILE, TCODColor::pink,
		"This band of quivering flesh confers a sense of profound unease. Of course, such is the fate of the wretched, who \
could go no further than Coruscating Beach."));
	c->addEffect(GAIN_HEALTH_ON_KILL, 10);
	return c;
}

charmSharedPtr charm_WretchedFleshmask()
{
	charmSharedPtr c(new charm("Wretched Fleshmask", CHARM_TILE, TCODColor::lightPink,
		"Drape a tattered skin across your face, and perhaps you will forget your troubles. Such was the motto of the wretched \
who fled the downfall of the Winter Court."));
	c->addEffect(HEALING_FACTOR, 25);
	return c;
}

charmSharedPtr charm_FrozenFlowerCharm()
{
	charmSharedPtr c(new charm("Frozen Flower Charm", CHARM_TILE, TCODColor::cyan,
		"This charm is made of unmelting ice shaped like a tiny flower."));
	c->addEffect(COLD_DAMAGE_ADDS_INFUSION, 25);
	return c;
}

charmSharedPtr charm_ToxicantsCharm()
{
	charmSharedPtr c(new charm("Toxicant's Charm", CHARM_TILE, TCODColor::lime,
		"This charm slowly dissolves toxins into the wearer's skin, enhancing your strength when poison \
grips you."));
	c->addEffect(PHYS_RESIST_WHILE_POISONED, 20);
	return c;
}
