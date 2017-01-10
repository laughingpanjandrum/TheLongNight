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
