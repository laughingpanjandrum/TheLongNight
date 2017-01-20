
#ifndef SPELL_H
#define SPELL_H

#include "item.h"


class spell : public item
{
public:

	//Constructors
	spell() : item() {}
	spell(std::string name, int tileCode, TCODColor color, std::string description = "");
	~spell();

	//Setting
	void setAttackType(attackType aType, int attackRange = 1);
	void addEffect(effect eff, int potency);
	void setVigourCost(int v) { vigourCost = v; }
	void setAttackRange(int r) { attackRange = r; }

	//Getting
	int getEffectsCount() { return effectsApplied.size(); }
	effect getEffectType(int idx) { return effectsApplied.at(idx); }
	int getEffectPotency(int idx) { return effectPotencies.at(idx); }
	attackType getAttackType() { return aType; }
	int getAttackRange() { return attackRange; }
	int getVigourCost() { return vigourCost; }

	//Flags
	bool usesSpellPower = false;
	bool usesDivinePower = false;

private:

	//What we actually do
	effectVector effectsApplied;
	std::vector<int> effectPotencies;

	//How we're applied
	attackType aType;
	int attackRange; //If not melee; otherwise defines either range OR aoe radius

	//Casting cost
	int vigourCost = 1;

};

//Weapon arts
spell* attack_Splintering();
spell* attack_Quickstep();
spell* attack_StrongAttack();
spell* attack_Knockback();

//Wand/chime powers
spell* ability_PowerSurge();
spell* ability_DivineFavour();

//Thrown weapon attacks
spell* attack_ThrowingKnives();
spell* attack_LaceratingKnives();

//Arcane spells
spell* spell_MagicMissile();
spell* spell_ArcaneRadiance();

//Prayers
spell* prayer_Restoration();

//Monster abilities
spell* ability_Charge();
spell* ability_Burst();
spell* ability_CrowArrow();

#endif