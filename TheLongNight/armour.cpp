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

armourSharedPtr headgear_CaptainsTricorn()
{
	armourSharedPtr h(new armour("Captain's Tricorn", HELMET_TILE, TCODColor::lightBlue, ITEM_HELMET,
		"Ornate tricorn hat, intended to make the stardrift captain more imposing to his subordinates."));
	h->setDefence(5);
	return h;
}

armourSharedPtr armour_RuinedUniform()
{
	armourSharedPtr a(new armour("Ruined Uniform", ARMOUR_TILE, TCODColor::lightBlue, ITEM_BODY_ARMOUR,
		"A relic of eras prior to the annihilation of time, when the stardrift sigil meant something."));
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armourSharedPtr headgear_RuinedKnightsHelm()
{
	armourSharedPtr h(new armour("Ruined Knight's Helm", HELMET_TILE, TCODColor::darkGreen, ITEM_HELMET,
		"This helmet has taken more than a few blows."));
	h->setDefence(10);
	h->setImage(IMAGE_RUINED_KNIGHT_HELM);
	return h;
}

armourSharedPtr armour_RuinedKnightsArmour()
{
	armourSharedPtr a(new armour("Ruined Knight's Armour", ARMOUR_TILE, TCODColor::darkGreen, ITEM_BODY_ARMOUR,
		"The heraldry on this armour is so old that it's illegible."));
	a->setDefence(20);
	a->setMoveSpeed(SPEED_NORMAL);
	a->setImage(IMAGE_RUINED_KNIGHT_ARMOUR);
	return a;
}

armourSharedPtr headgear_ClericsHood()
{
	armourSharedPtr a(new armour("Cleric's Hood", HOOD_TILE, TCODColor::lighterYellow, ITEM_HELMET,
		"It's important to remember your prayers."));
	a->setDefence(0);
	a->setDamageResist(DAMAGE_PROFANE, 5);
	a->setImage(IMAGE_CLERICS_HOOD);
	return a;
}

armourSharedPtr armour_ClericsVestments()
{
	armourSharedPtr a(new armour("Cleric's Vestments", ROBE_TILE, TCODColor::lighterYellow, ITEM_BODY_ARMOUR,
		"The clerics of the Lady of the Rose were blessed with certain protections against profane powers, though, of course, that did not save them."));
	a->setDefence(10);
	a->setDamageResist(DAMAGE_PROFANE, 10);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(IMAGE_CLERICS_VESTMENTS);
	return a;
}

armourSharedPtr headgear_CrowKnightsHood()
{
	armourSharedPtr a(new armour("Crow Knight's Hood", HOOD_TILE, TCODColor::lightGrey, ITEM_HELMET,
		"The hood of a crow knight is black, and interlaced with singular white feathers."));
	a->setDefence(5);
	a->setBleedResist(10);
	a->setImage(IMAGE_CROW_KNIGHT_HELM);
	return a;
}

armourSharedPtr armour_CrowKnightsArmour()
{
	armourSharedPtr a(new armour("Crow Knight's Armour", ARMOUR_TILE, TCODColor::lightGrey, ITEM_BODY_ARMOUR,
		"The feathered armour of the crow knights is thick and warm. Beady eyes are not required."));
	a->setDefence(10);
	a->setBleedResist(15);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(IMAGE_CROW_KNIGHT_ARMOUR);
	return a;
}

armourSharedPtr headgear_FishpriestHat()
{
	armourSharedPtr a(new armour("Fishpriest Hat", HOOD_TILE, TCODColor::lime, ITEM_HELMET,
		"Scaly hat of the preachers of doom, whose warnings about the obliteration of Wyrd were ignored."));
	a->setDefence(5);
	a->setBleedResist(5);
	a->setDamageResist(DAMAGE_ACID, 5);
	return a;
}

armourSharedPtr armour_FishscaleCoat()
{
	armourSharedPtr a(new armour("Fishscale Coat", ROBE_TILE, TCODColor::lightLime, ITEM_BODY_ARMOUR,
		"Scaly coat of the preachers of doom, whose pleas for the safety of Wyrd went unanswered."));
	a->setDefence(10);
	a->setBleedResist(10);
	a->setDamageResist(DAMAGE_ACID, 10);
	a->setMoveSpeed(SPEED_NORMAL);
	return a;
}

armourSharedPtr headgear_CityGuardHelm()
{
	armourSharedPtr a(new armour("City Guard Helm", HELMET_TILE, TCODColor::darkYellow, ITEM_HELMET,
		"In the final days of the Winter Court, the faces of the guards were stripped of flesh, and their bare bone was \
exposed to the devouring gaze of Pash."));
	a->setDefence(15);
	a->setDamageResist(DAMAGE_ELECTRIC, 10);
	a->setDamageResist(DAMAGE_FIRE, 10);
	a->setImage(IMAGE_CITY_GUARD_HELM);
	return a;
}

armourSharedPtr armour_CityGuardArmour()
{
	armourSharedPtr a(new armour("City Guard Armour", ARMOUR_TILE, TCODColor::darkYellow, ITEM_BODY_ARMOUR,
		"The guards of the city infused their armour to protect themselves against the profaned slaves of Pash."));
	a->setDefence(30);
	a->setDamageResist(DAMAGE_ELECTRIC, 20);
	a->setDamageResist(DAMAGE_FIRE, 25);
	a->setMoveSpeed(SPEED_SLOW);
	a->setImage(IMAGE_CITY_GUARD_ARMOUR);
	return a;
}

armourSharedPtr headgear_PashHood()
{
	armourSharedPtr a(new armour("Pash Hood", HOOD_TILE, TCODColor::lightPurple, ITEM_HELMET,
		"Hood that shields the eyes of the devotees of Pash. According to certain storytellers, in the final days of the Winter \
Court, the face of Pash itself appeared in the sky, devouring the stars."));
	a->setDefence(5);
	a->setDamageResist(DAMAGE_COLD, 10);
	a->setDamageResist(DAMAGE_BLESSED, 10);
	a->setImage(IMAGE_PASH_HOOD);
	return a;
}

armourSharedPtr armour_PashRobes()
{
	armourSharedPtr a(new armour("Pash Robes", ROBE_TILE, TCODColor::lightPurple, ITEM_BODY_ARMOUR,
		"Robe of the devotees of Pash. When the Void first reached the Winter Court, these devotees began to appear in the \
streets, though none knew their origins."));
	a->setDefence(5);
	a->setDamageResist(DAMAGE_COLD, 15);
	a->setDamageResist(DAMAGE_BLESSED, 15);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(IMAGE_PASH_ROBES);
	return a;
}

armourSharedPtr headgear_CursedKnightsHelm()
{
	armourSharedPtr a(new armour("Cursed Knight's Helm", HELMET_TILE, TCODColor::lightPurple, ITEM_HELMET,
		"Helm of the knights of the Winter Court, who stood guard over Kythiel until her final moments."));
	a->setDefence(15);
	a->setDamageResist(DAMAGE_BLESSED, 15);
	a->setImage(IMAGE_CURSED_KNIGHT_HELM);
	return a;
}

armourSharedPtr armour_CursedKnightsArmour()
{
	armourSharedPtr a(new armour("Cursed Knight's Armour", ARMOUR_TILE, TCODColor::lightPurple, ITEM_BODY_ARMOUR,
		"Armour of the knights of the Winter Court, who stood guard over Kythiel until her final moments."));
	a->setDefence(30);
	a->setDamageResist(DAMAGE_BLESSED, 30);
	a->setMoveSpeed(SPEED_SLOW);
	a->setImage(IMAGE_CURSED_KNIGHT_ARMOUR);
	return a;
}

armourSharedPtr headgear_GreyThiefsHood()
{
	armourSharedPtr a(new armour("Grey Thief's Hood", HOOD_TILE, TCODColor::lightGrey, ITEM_HELMET,
		"The thieves that haunt the Deadmarsh prefer to conceal their faces. Who knows \
what awful and well-deserved disfigurements they received at the hands of Hightower's sadistic jailors?"));
	a->setDefence(0);
	a->setBleedResist(10);
	a->setPoisonResist(10);
	return a;
}

armourSharedPtr armour_GreyThiefsRags()
{
	armourSharedPtr a(new armour("Grey Thief's Rags", ROBE_TILE, TCODColor::lightGrey, ITEM_BODY_ARMOUR,
		"Rags worn by a thief. These sallow-eyed villians escaped from the dungeons of Hightower, where they had recieved their \
well-deserved punishment."));
	a->setDefence(5);
	a->setBleedResist(20);
	a->setPoisonResist(20);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armourSharedPtr headgear_SilverPlatedHood()
{
	armourSharedPtr a(new armour("Silver-plated Hood", HOOD_TILE, TCODColor::silver, ITEM_HELMET,
		"The Silver Knights served in anonymity, and wore these hoods to conceal their features, so \
that they would not recieve undue praise."));
	a->setDefence(15);
	a->setDamageResist(DAMAGE_FIRE, 10);
	a->setDamageResist(DAMAGE_PROFANE, 20);
	return a;
}

armourSharedPtr armour_SilverPlatedArmour()
{
	armourSharedPtr a(new armour("Silver-plated Armour", ARMOUR_TILE, TCODColor::silver, ITEM_BODY_ARMOUR,
		"Holy armour of the knights serving under the Emissaries of Hightower. These blessed souls protected the Valley \
from the onslaught of night."));
	a->setDefence(40);
	a->setDamageResist(DAMAGE_COLD, 10);
	a->setDamageResist(DAMAGE_PROFANE, 30);
	a->setMoveSpeed(SPEED_SLOW);
	return a;
}

armourSharedPtr headgear_EtherealCrown()
{
	armourSharedPtr a(new armour("Ethereal Crown", HELMET_TILE, TCODColor::silver, ITEM_HELMET,
		"In time, the Sparrows who studied the arcane faded from the physical world, becoming mere shadows. \
This silver crown can hardly be seen, though it feels real enough."));
	a->setDefence(0);
	a->setDamageResist(DAMAGE_MAGIC, 25);
	return a;
}

armourSharedPtr armour_EtherealRobes()
{
	armourSharedPtr a(new armour("Ethereal Robes", ROBE_TILE, TCODColor::silver, ITEM_BODY_ARMOUR,
		"As the Sparrows sought the truth of the world, they became increasingly disconnected from it. \
Now they exist as mere shadows, gazing upon planes of existence beyond our own."));
	a->setDefence(0);
	a->setDamageResist(DAMAGE_MAGIC, 50);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armourSharedPtr headgear_SparrowKnightsHelm()
{
	armourSharedPtr a(new armour("Sparrow Knight's Helm", HELMET_TILE, TCODColor::darkMagenta, ITEM_HELMET,
		"Helm of the Sparrow Knights, sent to guard the mages of the tower. The eyes of the knights seem haunted - \
what do they see in the pale realm beyond the world?"));
	a->setDefence(5);
	a->setDamageResist(DAMAGE_MAGIC, 15);
	return a;
}

armourSharedPtr armour_SparrowKnightsArmour()
{
	armourSharedPtr a(new armour("Sparrow Knight's Armour", ARMOUR_TILE, TCODColor::darkMagenta, ITEM_BODY_ARMOUR,
		"Armour of the Sparrow Knights, who were called upon to protect the secret work of the Sparrows. Alas, these knights \
were slowly drawn into pale worlds of the imagination, until only their spectral forms remained."));
	a->setDefence(10);
	a->setDamageResist(DAMAGE_MAGIC, 25);
	a->setMoveSpeed(SPEED_NORMAL);
	return a;
}

armourSharedPtr headgear_SirPercivelsHelm()
{
	armourSharedPtr a(new armour("Sir Percivel's Helm", HELMET_TILE, TCODColor::lightGreen, ITEM_HELMET,
		"Sir Percivel sought the truth of the Rose's decline. His search brought him first to the depths of the Deadmarsh, where he learned \
obscure wisdom from the rotten folk that lurked in the mud."));
	a->setDefence(10);
	a->setPoisonResist(25);
	return a;
}
