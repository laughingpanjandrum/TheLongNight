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

void monster::setHealth(int hp)
{
	this->health.setTo(hp);
}

/*
Monster definitions
*/

monster * monster_DrownedDead()
{
	monster * m = new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::darkGreen);
	m->setMeleeStats(10, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(10);
	//m->addSpellKnown(spell_MagicMissile());
	//m->setSpellPower(100);
	return m;
}

monster * monster_ThinWretch()
{
	monster * m = new monster("Thin Wretch", WRETCH_TILE, TCODColor::green);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(50);
	return m;
}

monster * monster_TimidWretch()
{
	monster * m = new monster("Timid Wretch", WRETCH_TILE, TCODColor::lightGreen);
	m->setMeleeStats(15, SPEED_FAST);
	m->setMoveStats(SPEED_SLOW);
	m->setHealth(50);
	return m;
}

monster * monster_BloatedWretch()
{
	monster * m = new monster("Bloated Wretch", WRETCH_TILE, TCODColor::darkLime);
	m->setMeleeStats(25, SPEED_SLOW);
	m->setMoveStats(SPEED_SLOW);
	m->setHealth(100);
	m->addSpellKnown(ability_Burst());
	return m;
}



/*
This giant nightmare is how monsters are defined in map files.
*/
monster * getMonsterByHandle(std::string handle)
{
	//Stardrift Wreckage
	if (handle == "drowned_dead")
		return monster_DrownedDead();
	//Coruscating Beach
	else if (handle == "thin_wretch")
		return monster_ThinWretch();
	else if (handle == "timid_wretch")
		return monster_TimidWretch();
	else if (handle == "bloated_wretch")
		return monster_BloatedWretch();
	return nullptr;
}
