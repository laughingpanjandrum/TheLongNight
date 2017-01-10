#include "monster.h"

monster::monster() : person()
{
}

monster::monster(std::string name, int tileCode, TCODColor color) : 
	person(name, tileCode, color)
{
	//We're not the player! That's kind of our defining feature
	isPlayer = false;
}

/*
Quick and easy way to set our basic melee stats.
Meant for monster constructors.
*/
void monster::setMeleeStats(int damage, int speed)
{
	this->baseMeleeDamage = damage;
	this->baseAttackSpeed = speed;
}

/*
Quick and easy way to set stats related to movement.
*/
void monster::setMoveStats(int speed)
{
	this->baseMoveSpeed = speed;
}

/*
Monster definitions
*/

monster * drownedDead()
{
	monster * m = new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::darkGreen);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setMoveStats(SPEED_NORMAL);
	return m;
}

