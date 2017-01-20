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
	return sp;
}

spell * attack_Quickstep()
{
	spell* sp = new spell("Quickstep", SWORD_TILE, TCODColor::yellow,
		"Quickly leap one space.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->setVigourCost(1);
	sp->addEffect(GAIN_FREE_MOVES, 1);
	return sp;
}

spell * attack_StrongAttack()
{
	spell* sp = new spell("Strong Attack", SWORD_TILE, TCODColor::darkRed,
		"Next attack deals double damage.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_ATTACK, 100);
	sp->setVigourCost(1);
	return sp;
}

spell * attack_Knockback()
{
	spell* sp = new spell("Knockback", SWORD_TILE, TCODColor::lightBlue,
		"Next attack knocks target back.");
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(KNOCKBACK_TARGET, 2);
	sp->setVigourCost(1);
	return sp;
}

//Wand/chime powers

spell * ability_PowerSurge()
{
	spell* sp = new spell("Power Surge", SWORD_TILE, TCODColor::magenta,
		"Your next spell gains +50% power.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 50);
	sp->setVigourCost(1);
	return sp;
}

spell * ability_DivineFavour()
{
	spell* sp = new spell("Divine Favour", SWORD_TILE, TCODColor::darkYellow,
		"Slowly restore some health.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(ADD_HEALTH_TRICKLE, 15);
	sp->setVigourCost(1);
	return sp;
}

//Thrown weapon spells

spell * attack_ThrowingKnives()
{
	spell* sp = new spell("Throwing Knives", DAGGER_TILE, TCODColor::lightGrey);
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(5);
	return sp;
}

spell * attack_LaceratingKnives()
{
	spell* sp = new spell("Lacerating Knives", DAGGER_TILE, TCODColor::lightCrimson);
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_BLEED_DAMAGE, 20);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 10);
	sp->setAttackRange(5);
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
	sp->usesSpellPower = true;
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
	sp->usesSpellPower = true;
	return sp;
}

//Prayers

spell * prayer_Restoration()
{
	spell* sp = new spell("Restoration", SPELL_TILE, TCODColor::darkYellow,
		"Call upon the favour of the old gods to restore life essence.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(RESTORE_HEALTH, 25);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
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
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 10);
	sp->usesSpellPower = false;
	return sp;
}
