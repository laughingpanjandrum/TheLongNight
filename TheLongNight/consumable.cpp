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

consumable * consumable_StarwaterDraught()
{
	consumable* c = new consumable("Starwater Draught", VIAL_TILE, TCODColor::cyan,
		"Flask of water touched by starlight, which confers healing properties.");
	c->addEffect(RESTORE_HEALTH);
	c->setPotency(25);
	return c;
}

consumable * consumable_InvigoratingTea()
{
	consumable* c = new consumable("Invigorating Tea", VIAL_TILE, TCODColor::green,
		"A specially brewed tea that restores vigour.");
	c->addEffect(RESTORE_VIGOUR);
	c->setPotency(5);
	return c;
}

consumable * consumable_TinyRedFlower()
{
	consumable* c = new consumable("Tiny Red Flower", FLOWER_TILE, TCODColor::crimson,
		"A small flower of the kind Princess Yulia once wore in her hair. Clears Bleed buildup.");
	c->addEffect(REMOVE_BLEED);
	c->setPotency(1);
	return c;
}

consumable * ranged_ThrowingKnives()
{
	consumable* c = new consumable("Throwing Knives", DAGGER_TILE, TCODColor::lightGrey,
		"Short knives to chuck at your enemies.");
	c->setRangedAttack(new spell(5, 25));
	c->add(4); //Comes in a stack
	return c;
}

consumable * ranged_LaceratingKnives()
{
	consumable* c = new consumable("Lacerating Knives", DAGGER_TILE, TCODColor::lightCrimson,
		"Short knives with lacerating spines, designed to carve the flesh of your enemies.");
	c->setRangedAttack(new spell(5, 10, APPLY_BLEED_DAMAGE, 15));
	c->add(4);
	return c;
}

consumable * ranged_CorrodingJar()
{
	consumable* c = new consumable("Corroding Jar", VIAL_TILE, TCODColor::lime,
		"A jar filled with acid. Throw at armoured enemies to penetrate their defences.");
	c->setRangedAttack(new spell(5, 5, APPLY_ACID_DAMAGE, 20));
	c->add(3);
	return c;
}

consumable * ranged_PyromancersFlask()
{
	consumable* c = new consumable("Pyromancer's Flask", VIAL_TILE, TCODColor::flame,
		"A jar of volatile oil, ready to burst into flame on contact.");
	c->setRangedAttack(new spell(5, 5, APPLY_FIRE_DAMAGE, 20));
	c->add(3);
	return c;
}

consumable * oil_PyromancersOil()
{
	consumable* c = new consumable("Pyromancer's Oil", VIAL_TILE, TCODColor::darkFlame,
		"If properly treated, pyromancer's oil can be convinced to burn rather than explode.");
	c->setWeaponBuff(new weaponBuff(DAMAGE_FIRE, 25));
	return c;
}
