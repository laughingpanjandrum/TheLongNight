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
	wp->setSpecialAttack(attack_Splintering());
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
	wp->addSpell(spell_MagicMissile());
	wp->setSpellPower(100);
	wp->makeOffhand();
	return wp;
}