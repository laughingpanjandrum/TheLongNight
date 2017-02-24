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
	h->setImage(loadArmourImage("H_captian's uniform"));
	return h;
}

armourSharedPtr armour_RuinedUniform()
{
	armourSharedPtr a(new armour("Ruined Uniform", ARMOUR_TILE, TCODColor::lightBlue, ITEM_BODY_ARMOUR,
		"A relic of eras prior to the annihilation of time, when the stardrift sigil meant something."));
	a->setDefence(10);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(loadArmourImage("B_captian's uniform"));
	return a;
}

armourSharedPtr headgear_RuinedKnightsHelm()
{
	armourSharedPtr h(new armour("Ruined Knight's Helm", HELMET_TILE, TCODColor::darkGreen, ITEM_HELMET,
		"This helmet has taken more than a few blows."));
	h->setDefence(10);
	h->setImage(loadArmourImage("H_ruined knight"));
	return h;
}

armourSharedPtr armour_RuinedKnightsArmour()
{
	armourSharedPtr a(new armour("Ruined Knight's Armour", ARMOUR_TILE, TCODColor::darkGreen, ITEM_BODY_ARMOUR,
		"The heraldry on this armour is so old that it's illegible."));
	a->setDefence(20);
	a->setMoveSpeed(SPEED_NORMAL);
	a->setImage(loadArmourImage("B_ruined knight"));
	return a;
}

armourSharedPtr headgear_ClericsHood()
{
	armourSharedPtr a(new armour("Cleric's Hood", HOOD_TILE, TCODColor::lighterYellow, ITEM_HELMET,
		"It's important to remember your prayers."));
	a->setDefence(0);
	a->setDamageResist(DAMAGE_PROFANE, 5);
	a->setImage(loadArmourImage("H_vestments"));
	return a;
}

armourSharedPtr armour_ClericsVestments()
{
	armourSharedPtr a(new armour("Cleric's Vestments", ROBE_TILE, TCODColor::lighterYellow, ITEM_BODY_ARMOUR,
		"The clerics of the Lady of the Rose were blessed with certain protections against profane powers, though, of course, that did not save them."));
	a->setDefence(10);
	a->setDamageResist(DAMAGE_PROFANE, 10);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(loadArmourImage("B_vestments"));
	return a;
}

armourSharedPtr headgear_CrowKnightsHood()
{
	armourSharedPtr a(new armour("Crow Knight's Hood", HOOD_TILE, TCODColor::lightGrey, ITEM_HELMET,
		"The hood of a crow knight is black, and interlaced with singular white feathers."));
	a->setDefence(5);
	a->setBleedResist(10);
	a->setImage(loadArmourImage("B_crow knight"));
	return a;
}

armourSharedPtr armour_CrowKnightsArmour()
{
	armourSharedPtr a(new armour("Crow Knight's Armour", ARMOUR_TILE, TCODColor::lightGrey, ITEM_BODY_ARMOUR,
		"The feathered armour of the crow knights is thick and warm. Beady eyes are not required."));
	a->setDefence(10);
	a->setBleedResist(15);
	a->setMoveSpeed(SPEED_FAST);
	a->setImage(loadArmourImage("H_crow knight"));
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
	a->setImage(loadArmourImage("H_city guard"));
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
	a->setImage(loadArmourImage("B_city guard"));
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
	a->setImage(loadArmourImage("H_evil priest"));
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
	a->setImage(loadArmourImage("B_evil priest"));
	return a;
}

armourSharedPtr headgear_CursedKnightsHelm()
{
	armourSharedPtr a(new armour("Cursed Knight's Helm", HELMET_TILE, TCODColor::lightPurple, ITEM_HELMET,
		"Helm of the knights of the Winter Court, who stood guard over Kythiel until her final moments."));
	a->setDefence(15);
	a->setDamageResist(DAMAGE_BLESSED, 15);
	a->setImage(loadArmourImage("H_cursed knight"));
	return a;
}

armourSharedPtr armour_CursedKnightsArmour()
{
	armourSharedPtr a(new armour("Cursed Knight's Armour", ARMOUR_TILE, TCODColor::lightPurple, ITEM_BODY_ARMOUR,
		"Armour of the knights of the Winter Court, who stood guard over Kythiel until her final moments."));
	a->setDefence(30);
	a->setDamageResist(DAMAGE_BLESSED, 30);
	a->setMoveSpeed(SPEED_SLOW);
	a->setImage(loadArmourImage("B_cursed knight"));
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
	a->setImage(loadArmourImage("H_grey thief"));
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
	a->setImage(loadArmourImage("B_grey thief"));
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
	a->setImage(loadArmourImage("H_silver plated"));
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
	a->setImage(loadArmourImage("B_silver plated"));
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

armourSharedPtr headgear_MoshkasHeadpiece()
{
	armourSharedPtr a(new armour("Moshka's Headpiece", HOOD_TILE, TCODColor::fuchsia, ITEM_HELMET,
		"Headgear with an eyepiece meant to peer into unseen truths. Once worn by Moshka, whose observatory was meant \
to look into other realms."));
	a->setDamageResist(DAMAGE_MAGIC, 10);
	a->setDamageResist(DAMAGE_FIRE, 10);
	a->setDamageResist(DAMAGE_ACID, 10);
	a->setDamageResist(DAMAGE_COLD, 10);
	return a;
}

armourSharedPtr armour_MoshkasRobes()
{
	armourSharedPtr a(new armour("Moshka's Robes", ROBE_TILE, TCODColor::fuchsia, ITEM_BODY_ARMOUR,
		"Robes of Moshka, who peered into the mysteries beyond the world. He believed that the arcane symbols stitched \
into his robes protected him from doom."));
	a->setDamageResist(DAMAGE_MAGIC, 20);
	a->setDamageResist(DAMAGE_FIRE, 20);
	a->setDamageResist(DAMAGE_ACID, 20);
	a->setDamageResist(DAMAGE_COLD, 20);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armourSharedPtr headgear_SentinelsHelm()
{
	armourSharedPtr a(new armour("Sentinel's Helm", HELMET_TILE, TCODColor::lightBlue, ITEM_HELMET,
		"Slitted helm of the Sentinels. It permitted almost no light to enter, which led some to suspect \
that the Sentinels were not entirely human."));
	a->setDefence(15);
	a->setDamageResist(DAMAGE_ELECTRIC, 15);
	a->setDamageResist(DAMAGE_MAGIC, 15);
	return a;
}

armourSharedPtr armour_SentinelsArmour()
{
	armourSharedPtr a(new armour("Sentinel's Armour", ARMOUR_TILE, TCODColor::lightBlue, ITEM_BODY_ARMOUR,
		"Heavy armour of the Sentinels, decorated with the sigils of the Moon-Pale King. They never flagged in \
their ancient duty, and indeed, some began to suspect that they were not even human."));
	a->setDefence(45);
	a->setDamageResist(DAMAGE_ELECTRIC, 35);
	a->setDamageResist(DAMAGE_MAGIC, 15);
	a->setMoveSpeed(SPEED_SLOW);
	return a;
}

armourSharedPtr headgear_GhostlyHelm()
{
	armourSharedPtr a(new armour("Ghostly Helm", HELMET_TILE, TCODColor::lightestPink, ITEM_HELMET,
		"Half-unreal helm that glitters with arcane essence. Once worn by the knights of the Moon-Pale King, \
who slowly faded from existence."));
	a->setDamageResist(DAMAGE_MAGIC, 15);
	a->setDamageResist(DAMAGE_COLD, 10);
	a->setDamageResist(DAMAGE_ACID, 10);
	return a;
}

armourSharedPtr armour_GhostlyArmour()
{
	armourSharedPtr a(new armour("Ghostly Armour", ARMOUR_TILE, TCODColor::lightestPink, ITEM_BODY_ARMOUR,
		"Half-unreal armour that glistens with arcane essence. The warriors of the Moon-Pale King wore this armour, \
until they slowly faded from time."));
	a->setDamageResist(DAMAGE_MAGIC, 30);
	a->setDamageResist(DAMAGE_COLD, 20);
	a->setDamageResist(DAMAGE_ACID, 20);
	a->setMoveSpeed(SPEED_FAST);
	return a;
}

armourSharedPtr headgear_SirPercivelsHelm()
{
	armourSharedPtr a(new armour("Sir Percivel's Helm", HELMET_TILE, TCODColor::lightGreen, ITEM_HELMET,
		"Sir Percivel sought the truth of the Rose's decline. His search brought him first to the depths of the Deadmarsh, where he learned \
obscure wisdom from the rotten folk that lurked in the mud."));
	a->setDefence(10);
	a->setPoisonResist(25);
	a->inSirPercivelsSet = true;
	a->setImage(loadArmourImage("H_green knight"));
	return a;
}

armourSharedPtr armour_SirPercivelsArmour()
{
	armourSharedPtr a(new armour("Sir Percivel's Armour", ARMOUR_TILE, TCODColor::lightGreen, ITEM_BODY_ARMOUR,
		"Sir Percivel sought the truth of the Rose's decline. The legend of Khalle and the silence of the gods drew \
him to her forsaken mausoleum, where he uncovered an awful truth."));
	a->setDefence(20);
	a->setDamageResist(DAMAGE_ELECTRIC, 20);
	a->setMoveSpeed(SPEED_NORMAL);
	a->inSirPercivelsSet = true;
	a->setImage(loadArmourImage("B_green knight"));
	return a;
}

armourSharedPtr headgear_GravekeepersHood()
{
	armourSharedPtr a(new armour("Gravekeeper's Hood", HOOD_TILE, TCODColor::darkAmber, ITEM_HELMET,
		"Putrid hood of the gravekeepers, who keep watch over the dead. Under these hoods, little flesh remains."));
	a->setPlagueResist(10);
	a->setDamageResist(DAMAGE_ACID, 10);
	return a;
}

armourSharedPtr armour_GravekeepersRags()
{
	armourSharedPtr a(new armour("Gravekeeper's Rags", ROBE_TILE, TCODColor::darkAmber, ITEM_BODY_ARMOUR,
		"Rotted rags of the gravekeepers, charged by Farin to keep watch over the dead. According to certain storytellers, \
Farin kept a secret even from his most trusted gravekeepers, and took it with him when he walked out of the world."));
	a->setPlagueResist(40);
	a->setDamageResist(DAMAGE_ACID, 20);
	return a;
}

armourSharedPtr headgear_PilgrimsHood()
{
	armourSharedPtr a(new armour("Pilgrim's Hood", HOOD_TILE, TCODColor::lightSepia, ITEM_HELMET,
		"A simple pilgrim's hood, woven through with charms of guidance and protection."));
	a->setBleedResist(5);
	a->setPoisonResist(5);
	a->setPlagueResist(5);
	a->setDamageResist(DAMAGE_COLD, 10);
	a->setDamageResist(DAMAGE_ACID, 10);
	a->setDamageResist(DAMAGE_FIRE, 10);
	a->setDamageResist(DAMAGE_ELECTRIC, 10);
	return a;
}

armourSharedPtr armour_PilgrimsCoat()
{
	armourSharedPtr a(new armour("Pilgrim's Coat", ROBE_TILE, TCODColor::lightSepia, ITEM_BODY_ARMOUR,
		"A simple pilgrim's coat, woven through with charms of guidance and protection. Many pilgrims walk a long and \
dangerous road."));
	a->setBleedResist(10);
	a->setPoisonResist(10);
	a->setPlagueResist(10);
	a->setDamageResist(DAMAGE_COLD, 20);
	a->setDamageResist(DAMAGE_ACID, 20);
	a->setDamageResist(DAMAGE_FIRE, 20);
	a->setDamageResist(DAMAGE_ELECTRIC, 20);
	a->setMoveSpeed(SPEED_NORMAL);
	return a;
}

armourSharedPtr headgear_DragonboneHelm()
{
	armourSharedPtr a(new armour("Dragonbone Helm", HELMET_TILE, TCODColor::lightFlame, ITEM_HELMET,
		"The bones of dragons are said to be singularly resistant to fire, and indeed this helm - carved in \
the shape of a dragon's head - is always cool to touch."));
	a->setDefence(10);
	a->setDamageResist(DAMAGE_FIRE, 15);
	a->setDamageResist(DAMAGE_ELECTRIC, 10);
	return a;
}

armourSharedPtr armour_DragonboneArmour()
{
	armourSharedPtr a(new armour("Dragonbone Armour", ARMOUR_TILE, TCODColor::lightFlame, ITEM_BODY_ARMOUR,
		"The bones of dragons were said to be strongly attuned to fire, and indeed, these singularly heavy armour \
bears those properties."));
	a->setDefence(30);
	a->setDamageResist(DAMAGE_FIRE, 40);
	a->setDamageResist(DAMAGE_ACID, 20);
	a->setDamageResist(DAMAGE_ELECTRIC, 20);
	a->setMoveSpeed(SPEED_SLOW);
	return a;
}

armourSharedPtr headgear_StarweaversHood()
{
	armourSharedPtr a(new armour("Starweaver's Hood", HOOD_TILE, TCODColor::pink, ITEM_HELMET,
		"Majestic hood of a starweaver - the lost artificers of Atalundra, who knit universes together with their minds."));
	a->setDefence(0);
	a->setDamageResist(DAMAGE_MAGIC, 40);
	return a;
}

armourSharedPtr armour_StarweaversRobe()
{
	armourSharedPtr a(new armour("Starweaver's Robe", ROBE_TILE, TCODColor::pink, ITEM_BODY_ARMOUR,
		"Majestic robes of a starweaver - the lost artificers of Atalundra, who knit universes together with their minds."));
	a->setDamageResist(DAMAGE_MAGIC, 60);
	return a;
}

armourSharedPtr headgear_MoonPaleCrown()
{
	armourSharedPtr a(new armour("Moon-Pale Crown", HELMET_TILE, TCODColor::lighterBlue, ITEM_HELMET,
		"Crown of the Moon-Pale King. According to certain storytellers, the ancestral crown was cursed, causing his \
lands to drift endlessly in the void. But the king refused to remove it from his head."));
	a->setDamageResist(DAMAGE_MAGIC, 25);
	return a;
}
