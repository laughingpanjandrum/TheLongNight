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

consumable * consumable_TinyGreenFlower()
{
	consumable* c = new consumable("Tiny Green Flower", FLOWER_TILE, TCODColor::green,
		"These gentle green flowers purge toxins from the body.");
	c->addEffect(REMOVE_POISON);
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

consumable * ranged_HeavyJavelin()
{
	consumable* c = new consumable("Heavy Javelin", DAGGER_TILE, TCODColor::sepia,
		"Deadly javelin for throwing at your nemesis.");
	c->setRangedAttack(new spell(8, 75));
	c->add(1);
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
	c->setRangedAttack(new spell(5, 5, APPLY_ACID_DAMAGE, 40));
	c->add(3);
	return c;
}

consumable * ranged_PyromancersFlask()
{
	consumable* c = new consumable("Pyromancer's Flask", VIAL_TILE, TCODColor::flame,
		"A jar of volatile oil, ready to burst into flame on contact.");
	c->setRangedAttack(new spell(5, 5, APPLY_FIRE_DAMAGE, 40));
	c->add(3);
	return c;
}

consumable * ranged_WitchsJar()
{
	consumable* c = new consumable("Witch's Jar", VIAL_TILE, TCODColor::magenta,
		"Jars of magical oil, often carried by wizards for use as a last resort.");
	c->setRangedAttack(new spell(5, 5, APPLY_MAGIC_DAMAGE, 40));
	c->add(3);
	return c;
}

consumable * oil_PyromancersOil()
{
	consumable* c = new consumable("Pyromancer's Oil", VIAL_TILE, TCODColor::darkFlame,
		"If properly treated, pyromancer's oil can be convinced to burn rather than explode.");
	c->setWeaponBuff(new weaponBuff(DAMAGE_FIRE, 15));
	return c;
}

consumable * oil_CorrosiveOil()
{
	consumable* c = new consumable("Corrosive Oil", VIAL_TILE, TCODColor::lime,
		"Somehow, rather than corrode the blade, this oil coats targets in caustic acid.");
	c->setWeaponBuff(new weaponBuff(DAMAGE_ACID, 15));
	return c;
}

consumable * bell_VoidwalkersDancingBell()
{
	consumable* c = new consumable("Voidwalker's Dancing Bell", BELL_TILE, TCODColor::purple,
		"This bell pulses with the energy of the Void. You sense that to use it would be a dire sin.");
	c->consumeOnUse = false;
	c->addEffect(TELEPORT_TO_VOID);
	return c;
}

consumable * bell_VoidwalkersReturningBell()
{
	consumable* c = new consumable("Voidwalker's Returning Bell", BELL_TILE, TCODColor::lightPurple,
		"This bell radiates warmth. From within the void, it whispers to every walker of the dark, promising them that \
they will return home someday.");
	c->consumeOnUse = false;
	c->addEffect(TELEPORT_BACK_FROM_VOID);
	return c;
}
