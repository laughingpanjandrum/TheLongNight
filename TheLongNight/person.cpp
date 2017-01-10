#include "person.h"

/*
Constructor you should probably use.
Also sets a default health value, which you'll probably want to change!
*/
person::person(std::string name, int tileCode, TCODColor color) : element(name, tileCode, color)
{
	health.setTo(100);
}

person::~person()
{
}

/*
	GETTING ATTRIBUTES
*/

/*
How long our movement takes.
*/
int person::getMoveDelay()
{
	return SPEED_NORMAL;
}

/*
How long a melee attack takes.
Based on our equipped weapon.
*/
int person::getAttackDelay()
{
	weapon* wp = getWeapon();
	if (wp != nullptr)
		return wp->getAttackDelay();
	return SPEED_NORMAL;
}

/*
	DAMAGE AND HEALING
*/

void person::takeDamage(int amount)
{
	health.decrease(amount);
	if (health.isEmpty())
		die();
}

void person::die()
{
	isDead = true;
}

/*
	INVENTORY, EQUIPMENT
*/

/*
Equip the given item, if possible.
*/
void person::equipItem(item * which)
{
	items.equipItem(which);
}

/*
Returns our equipped weapon, or nullptr if we don't have one equipped.
*/
weapon * person::getWeapon()
{
	return items.getWeapon();
}
