#include "spell.h"



spell::spell(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_SPELL, description)
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
	spell* sp = new spell("Splintering", SWORD_TILE, TCODColor::crimson,
		"Deals 20 bleed damage to target.");
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(APPLY_BLEED_DAMAGE, 20);
	sp->setVigourCost(1);
	sp->usesSpellPower = false;
	return sp;
}

//Spells

spell * spell_MagicMissile()
{
	spell* sp = new spell("Magic Missile", SPELL_TILE, TCODColor::magenta,
		"Attack deals magic damage.");
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 50);
	sp->setAttackRange(5);
	sp->setVigourCost(1);
	return sp;
}

spell * spell_ArcaneRadiance()
{
	spell* sp = new spell("Arcane Radiance", SPELL_TILE, TCODColor::darkMagenta,
		"A blast of magical energy strikes everything within 3 spaces.");
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(3);
	sp->setVigourCost(2);
	return sp;
}

//Monster abilities


spell * ability_Charge()
{
	spell* sp = new spell("Charge", SPELL_TILE, TCODColor::white);
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_FREE_MOVES, 3);
	sp->usesSpellPower = false;
	return sp;
}

spell* ability_Burst()
{
	spell* sp = new spell("Burst", SPELL_TILE, TCODColor::darkLime);
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(2);
	sp->usesSpellPower = false;
	return sp;
}

spell * ability_CrowArrow()
{
	spell* sp = new spell("Crow Arrow", SPELL_TILE, TCODColor::lightGrey);
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->usesSpellPower = false;
	return sp;
}
