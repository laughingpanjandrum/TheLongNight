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
	Weapon types
*/

weapon * weapon_SplinteredSword()
{
	weapon* wp = new weapon("Splintered Sword", SWORD_TILE, TCODColor::grey);
	wp->setBasicAttributes(25, SPEED_NORMAL);
	//wp->addStatusEffect(EFFECT_BLEED, 20);
	return wp;
}
