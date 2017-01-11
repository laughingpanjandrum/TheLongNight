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
Based on our equipped body armour.
*/
int person::getMoveDelay()
{
	armour* a = getArmour();
	if (a != nullptr)
		return a->getMoveSpeed();
	return baseMoveSpeed;
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
	return baseAttackSpeed;
}

/*
Returns reduction to physical damage as an int out of 100%.
*/
int person::getDefence()
{
	//Base reduction
	int base = 0;
	//Bonus from armour
	armour* ar = getArmour();
	if (ar != nullptr)
		base += ar->getDefence();
	//Done!
	return base;
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
Add the given item to our items carried.
*/
void person::addItem(item * which)
{
	items.addItem(which);
}

/*
Returns our equipped weapon, or nullptr if we don't have one equipped.
*/
weapon * person::getWeapon()
{
	return items.getWeapon();
}

/*
Returns our equipped body armour.
*/
armour * person::getArmour()
{
	return items.getArmour();
}

/*
Returns a vector of all of our consumable slots.
*/
consumableVector person::getConsumableList()
{
	auto clist = items.getConsumables();
	//Pad out to full size with empty slots
	while (clist.size() < items.MAX_CONSUMABLE_SLOTS)
		clist.push_back(nullptr);
	return clist;
	/*consumableVector clist;
	auto equipmentList = items.getAllEquipped();
	for (auto i = equipmentList.begin(); i != equipmentList.end(); i++) {
		if ((*i).category == ITEM_CONSUMABLE) {
			consumable* cons = static_cast<consumable*>((*i).equipped);
			clist.push_back(cons);
		}
	}
	return clist;*/
}
