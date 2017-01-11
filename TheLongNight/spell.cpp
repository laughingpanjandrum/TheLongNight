#include "spell.h"



spell::spell(std::string name, int tileCode, TCODColor color) :
	item(name, tileCode, color, ITEM_SPELL)
{
}

spell::~spell()
{
}

/*
	SETTING
*/

void spell::setAttackType(attackType aType, int attackRange)
{
	this->aType = aType;
	this->attackRange = attackRange;
}

void spell::addEffect(effect eff, int potency)
{
	effectsApplied.push_back(eff);
	effectPotencies.push_back(potency);
}



/*
Spell objects
*/

//Weapon arts

spell * attack_Splintering()
{
	spell* sp = new spell("Splintering", SWORD_TILE, TCODColor::crimson);
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(APPLY_BLEED_DAMAGE, 20);
	sp->setVigourCost(1);
	return sp;
}
