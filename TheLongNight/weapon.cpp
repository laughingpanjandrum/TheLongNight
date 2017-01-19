#include "weapon.h"




weapon::~weapon()
{
}

/*
	SETTING
*/

void weapon::setBasicAttributes(int damage, int attackDelay)
{
	this->damage = damage;
	this->attackDelay = attackDelay;
}

/*
Add a status effect, e.g. bleed, to the weapon's standard attack
*/
void weapon::addStatusEffect(statusEffects eType, int damage)
{
	statusEffectType.push_back(eType);
	statusEffectDamage.push_back(damage);
}

/*
Returns whether we're currently holding the given spell.
*/
bool weapon::hasSpellStored(spell * sp)
{
	auto it = std::find(spellstore.begin(), spellstore.end(), sp);
	return it != spellstore.end();
}


/*
Returns how much bonus damage per stat point we deal for the given stat.
We do more scaling damage the fewer stats we scale with.
*/
int weapon::getScalingDamage(statScaling st)
{
	auto it = std::find(scaling.begin(), scaling.end(), st);
	if (it != scaling.end())
		return 5 - scaling.size();
	return 0;
}


/*
For descriptive reasons.
*/
std::string getAttackSpeedName(int del)
{
	switch (del) {
	case(SPEED_FAST): return "Fast";
	case(SPEED_NORMAL): return "Normal";
	case(SPEED_SLOW): return "Slow";
	}
	return "Unknown";
}


/*
	ALL ITEMS
*/

//		Weapons

weapon * weapon_SplinteredSword()
{
	weapon* wp = new weapon("Splintered Sword", SWORD_TILE, TCODColor::grey,
		"Once a treasured heirloom. Now it's just a broken sword, decent at serration, but certainly unfit for a stardrift captain.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addScalingType(SCALE_STR);
	return wp;
}

weapon * weapon_ThinKnife()
{
	weapon* wp = new weapon("Thin Knife", DAGGER_TILE, TCODColor::lightGrey,
		"A weapon for small, quick folks.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_StraightSword()
{
	weapon* wp = new weapon("Straight Sword", SWORD_TILE, TCODColor::lightGrey,
		"It's just a simple sword, but the haft has a nice little dragon face carved in it.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->setSpecialAttack(attack_StrongAttack());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_Warhammer()
{
	weapon* wp = new weapon("Warhammer", HAMMER_TILE, TCODColor::lightSepia,
		"A big ole hammer for smashing things.");
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->setSpecialAttack(attack_Knockback());
	wp->addScalingType(SCALE_STR);
	return wp;
}

weapon * weapon_CrowKnightSword()
{
	weapon* wp = new weapon("Crow Knight Sword", SWORD_TILE, TCODColor::lightGrey,
		"The hilt is carved in the shape of a crow's head, and the blade is tinted black. The crow knights are certainly dedicated to a theme.");
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_CrowKnife()
{
	weapon* wp = new weapon("Crow Knife", SWORD_TILE, TCODColor::lightGrey,
		"The short blade of this knife is barbed to inflict bleeding.");
	wp->setBasicAttributes(15, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_CrowHalfsword()
{
	weapon* wp = new weapon("Crow Half-sword", SWORD_TILE, TCODColor::darkCrimson,
		"Serrated half-sword of a crow knight, thirsting for blood.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 25);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

//		Shields

weapon * shield_BatteredWoodenShield()
{
	weapon* wp = new weapon("Battered Wooden Shield", SHIELD_TILE, TCODColor::sepia,
		"It's just a scrap of wood, but it might still save your life.");
	wp->setDefence(10);
	wp->makeOffhand();
	return wp;
}

//		Wands

weapon * wand_DriftwoodWand()
{
	weapon* wp = new weapon("Driftwood Wand", SWORD_TILE, TCODColor::sepia,
		"Someone made this chunk of wand in a desparate hurry.");
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setSpellPower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_PowerSurge());
	return wp;
}

weapon * chime_ClericsCrackedChime()
{
	weapon* wp = new weapon("Cleric's Cracked Chime", SWORD_TILE, TCODColor::darkYellow,
		"Ring this broken chime to politely ask favours of the gods.");
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(1);
	wp->setDivinePower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_DivineFavour());
	return wp;
}
