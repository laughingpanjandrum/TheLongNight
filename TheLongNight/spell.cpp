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
	sp->usesSpellPower = false;
	return sp;
}

//Spells

spell * spell_MagicMissile()
{
	spell* sp = new spell("Magic Missile", SPELL_TILE, TCODColor::magenta);
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 50);
	sp->setAttackRange(5);
	sp->setVigourCost(1);
	return sp;
}

spell * spell_ArcaneRadiance()
{
	spell* sp = new spell("Arcane Radiance", SPELL_TILE, TCODColor::darkMagenta);
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(3);
	sp->setVigourCost(2);
	return sp;
}

//Monster abilities

spell* ability_Burst()
{
	spell* sp = new spell("Burst", SPELL_TILE, TCODColor::darkLime);
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(2);
	sp->usesSpellPower = false;
	return sp;
}
