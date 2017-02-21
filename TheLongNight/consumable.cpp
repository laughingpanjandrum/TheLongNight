#include "consumable.h"



consumable::consumable(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_CONSUMABLE, description)
{
	amount.setTo(1);
}

consumable::~consumable()
{
}

/*
Returns amount left combined with name.
*/
std::string consumable::getMenuName()
{
	return getName() + " x" + std::to_string(getAmountLeft());
}

consumableSharedPtr consumable_StarwaterDraught()
{
	consumableSharedPtr c(new consumable("Starwater Draught", VIAL_TILE, TCODColor::cyan,
		"Flask of water touched by starlight, which confers healing properties."));
	c->addEffect(RESTORE_HEALTH);
	c->setPotency(25);
	return c;
}

consumableSharedPtr consumable_InvigoratingTea()
{
	consumableSharedPtr c(new consumable("Invigorating Tea", VIAL_TILE, TCODColor::green,
		"A specially brewed tea that restores vigour."));
	c->addEffect(RESTORE_VIGOUR);
	c->setPotency(5);
	return c;
}

consumableSharedPtr consumable_TinyRedFlower()
{
	consumableSharedPtr c(new consumable("Tiny Red Flower", FLOWER_TILE, TCODColor::crimson,
		"A small flower of the kind Princess Yulia once wore in her hair. Clears Bleed buildup."));
	c->addEffect(REMOVE_BLEED);
	c->setPotency(1);
	return c;
}

consumableSharedPtr consumable_TinyGreenFlower()
{
	consumableSharedPtr c(new consumable("Tiny Green Flower", FLOWER_TILE, TCODColor::green,
		"These gentle green flowers purge toxins from the body."));
	c->addEffect(REMOVE_POISON);
	c->setPotency(1);
	return c;
}

consumableSharedPtr consumable_BloodDrinkersEyes()
{
	consumableSharedPtr c(new consumable("Blood Drinker's Eyes", CHARM_TILE, TCODColor::crimson,
		"The reddened eyes of one who has consumed too much blood. According to the profane scriptures of Pash, these eyes \
contain profound power when eaten."));
	c->addEffect(BLEED_DAMAGE_FACTOR);
	c->setPotency(2);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_IntoxicatingWine()
{
	consumableSharedPtr c(new consumable("Intoxicating Wine", VIAL_TILE, TCODColor::darkRed,
		"This heady draught is said to purge the mind of material desires while enhancing one's will."));
	c->addEffect(INCREASE_PRAYER_POWER);
	c->addEffect(APPLY_DAMAGE_PENALTY);
	c->setPotency(25);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_WitchwaterFlask()
{
	consumableSharedPtr c(new consumable("Witchwater Flask", VIAL_TILE, TCODColor::fuchsia,
		"A flask of invigorating witchwater, said to break down the barriers between our world and reality. Moshka \
was a great believer in witchwater, and consumed it in copious amounts."));
	c->addEffect(INCREASE_SPELL_POWER);
	c->addEffect(APPLY_DAMAGE_PENALTY);
	c->setPotency(25);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_BlackHoney()
{
	consumableSharedPtr c(new consumable("Black Honey", VIAL_TILE, TCODColor::darkPurple,
		"Black honey, once the principle export of the village, was said to induce vivid dreams. But if one's life is filled \
with bloodshed, one's dreams are unlikely to bring peace."));
	c->addEffect(APPLY_DAMAGE_PENALTY);
	c->addEffect(INCREASE_SPELL_POWER);
	c->addEffect(INCREASE_PRAYER_POWER);
	c->setPotency(-10);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr ranged_ThrowingKnives()
{
	consumableSharedPtr c(new consumable("Throwing Knives", DAGGER_TILE, TCODColor::lightGrey,
		"Short knives to chuck at your enemies."));
	c->setRangedAttack(spellSharedPtr(new spell("Throwing Knife", c->getColor(), 5, 25)));
	c->add(4); //Comes in a stack
	return c;
}

consumableSharedPtr ranged_HeavyJavelin()
{
	consumableSharedPtr c(new consumable("Heavy Javelin", SPEAR_TILE, TCODColor::sepia,
		"Deadly javelin for throwing at your nemesis."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 8, 75)));
	c->add(1);
	return c;
}

consumableSharedPtr ranged_LaceratingKnives()
{
	consumableSharedPtr c(new consumable("Lacerating Knives", DAGGER_TILE, TCODColor::lightCrimson,
		"Short knives with lacerating spines, designed to carve the flesh of your enemies."));
	c->setRangedAttack(spellSharedPtr(new spell("Lacerating Knife", c->getColor(), 5, 10, APPLY_BLEED_DAMAGE, 15)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_PoisonThrowingKnives()
{
	consumableSharedPtr c(new consumable("Poison Throwing Knives", DAGGER_TILE, TCODColor::green,
		"Knives coated in poison. Preferred weapon of the grey thieves that escaped from Hightower."));
	c->setRangedAttack(spellSharedPtr(new spell("Poison Throwing Knives", c->getColor(), 5, 10, APPLY_POISON_DAMAGE, 15)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_CorrodingJar()
{
	consumableSharedPtr c(new consumable("Corroding Jar", VIAL_TILE, TCODColor::lime,
		"A jar filled with acid. Throw at armoured enemies to penetrate their defences."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_ACID_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_VoidEssenceJar()
{
	consumableSharedPtr c(new consumable("Void-Essence Jar", VIAL_TILE, TCODColor::purple,
		"A jar filled with cursed oil, designed to shatter on contact."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_PROFANE_DAMAGE, 45)));
	c->add(2);
	return c;
}

consumableSharedPtr ranged_PyromancersFlask()
{
	consumableSharedPtr c(new consumable("Pyromancer's Flask", VIAL_TILE, TCODColor::flame,
		"A jar of volatile oil, ready to burst into flame on contact."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_FIRE_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_WitchsJar()
{
	consumableSharedPtr c(new consumable("Witch's Jar", VIAL_TILE, TCODColor::magenta,
		"Jars of magical oil, often carried by wizards for use as a last resort."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_MAGIC_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_LightingJavelin()
{
	consumableSharedPtr c(new consumable("Lightning Javelin", SPEAR_TILE, TCODColor::yellow,
		"Javelin tipped with electric oil."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 25, APPLY_ELECTRIC_DAMAGE, 25)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_FrostKnives()
{
	consumableSharedPtr c(new consumable("Frost Knives", DAGGER_TILE, TCODColor::cyan,
		"Knives tipped with frozen oil to chill the bones."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_COLD_DAMAGE, 20)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_WarpingJavelin()
{
	consumableSharedPtr c(new consumable("Warping Javelin", SPEAR_TILE, TCODColor::lighterBlue,
		"These javelins, infused with the power of the void, phase in and out of reality, and always hit their target."));
	c->setRangedAttack(ability_WarpingJavelin());
	c->add(4);
	return c;
}

consumableSharedPtr oil_PyromancersOil()
{
	consumableSharedPtr c(new consumable("Pyromancer's Oil", VIAL_TILE, TCODColor::darkFlame,
		"If properly treated, pyromancer's oil can be convinced to burn rather than explode."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_FIRE, 15));
	return c;
}

consumableSharedPtr oil_CorrosiveOil()
{
	consumableSharedPtr c(new consumable("Corrosive Oil", VIAL_TILE, TCODColor::lime,
		"Somehow, rather than corrode the blade, this oil coats targets in caustic acid."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_ACID, 15));
	return c;
}

consumableSharedPtr oil_FrozenOil()
{
	consumableSharedPtr c(new consumable("Frozen Oil", VIAL_TILE, TCODColor::lightBlue,
		"The alchemists of the Winter Court created this curious oil, which coats a blade in ice."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_COLD, 15));
	return c;
}

consumableSharedPtr oil_HolyWater()
{
	consumableSharedPtr c(new consumable("Holy Water", VIAL_TILE, TCODColor::lightestBlue,
		"This vial of blessed water adds a sheen of pure holy energy to a blade."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_BLESSED, 15));
	return c;
}

consumableSharedPtr oil_CursedWater()
{
	consumableSharedPtr c(new consumable("Cursed Water", VIAL_TILE, TCODColor::lightPurple,
		"Vial of water cursed by profane priests, which now bubbles with unholy ichor."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_PROFANE, 15));
	return c;
}

consumableSharedPtr oil_HangmansBlood()
{
	consumableSharedPtr c(new consumable("Hangman's Blood", VIAL_TILE, TCODColor::darkerRed,
		"The pure blood of a hangman is enriched by those he kills, and in death, a hangman's blood \
is said to carry the stench of a thousand graves."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_PHYSICAL, 15));
	return c;
}

consumableSharedPtr oil_WitchsOoze()
{
	consumableSharedPtr c(new consumable("Witch's Ooze", VIAL_TILE, TCODColor::magenta,
		"The blood of those who dabble in the arcane slowly thickens, and congeals into a magic-drenched paste."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_MAGIC, 15));
	return c;
}

consumableSharedPtr oil_LightningOil()
{
	consumable* c = new consumable("Lightning Oil", VIAL_TILE, TCODColor::lightPurple,
		"Oil infused with lightning, used by the bone-whisperers of the deep.");
	c->setWeaponBuff(new weaponBuff(DAMAGE_ELECTRIC, 15));
	return consumableSharedPtr(c);
}

consumableSharedPtr oil_DeepRedOil()
{
	consumableSharedPtr c(new consumable("Deep Red Oil", VIAL_TILE, TCODColor::crimson,
		"Oil of a deep red shade. Not made from blood, but certainly has an affinity for it."));
	c->setWeaponBuff(new weaponBuff(EFFECT_BLEED, 10));
	return c;
}

consumableSharedPtr oil_RotbloodOil()
{
	consumableSharedPtr c(new consumable("Rotblood Oil", VIAL_TILE, TCODColor::sepia,
		"Oil made from the rotten blood of the vile dead. Use of this oil is forbidden in any land the sunlight touched."));
	c->setWeaponBuff(new weaponBuff(EFFECT_PLAGUE, 10));
	return c;
}

consumableSharedPtr bell_VoidwalkersDancingBell()
{
	consumableSharedPtr c(new consumable("Voidwalker's Dancing Bell", BELL_TILE, TCODColor::purple,
		"This bell pulses with the energy of the Void. You sense that to use it would be a dire sin."));
	c->consumeOnUse = false;
	c->addEffect(TELEPORT_TO_VOID);
	return c;
}

consumableSharedPtr bell_VoidwalkersReturningBell()
{
	consumableSharedPtr c(new consumable("Voidwalker's Returning Bell", BELL_TILE, TCODColor::lightPurple,
		"This bell radiates warmth. From within the void, it whispers to every walker of the dark, promising them that \
they will return home someday."));
	c->consumeOnUse = false;
	c->addEffect(TELEPORT_BACK_FROM_VOID);
	return c;
}

consumableSharedPtr horn_HeraldsWhiteHorn()
{
	consumableSharedPtr c(new consumable("Herald's White Horn", BELL_TILE, TCODColor::white,
		"The silver horn of the Herald, who comes in the White Fog. According to certain storytellers, his presence signifies the coming of the \
Lord of Fallow Fields, who rises from the realms of the old gods to bring final judgement upon the world."));
	c->consumeOnUse = false;
	c->addEffect(CREATE_FOG);
	c->setPotency(5);
	return c;
}
