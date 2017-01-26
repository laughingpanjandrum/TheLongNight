
#ifndef SPELL_H
#define SPELL_H

#include "item.h"


class spell : public item
{
public:

	//Constructors
	spell() : item() {}
	spell(std::string name, int tileCode, TCODColor color, std::string description = "");
	//This is for quickly constructing spells for ranged weapon attacks
	spell(int attackRange, int physDamage, effect otherEffect = NULL_EFFECT, int otherEffectPotency = 0);
	~spell();

	//Setting
	void setAttackType(attackType aType, int attackRange = 1);
	void addEffect(effect eff, int potency);
	void setVigourCost(int v) { vigourCost = v; }
	void setAttackRange(int r) { attackRange = r; }
	void setBuffApplied(damageType dtype, int damage) { buffApplied = weaponBuff(dtype, damage); }

	//Getting
	int getEffectsCount() { return effectsApplied.size(); }
	effect getEffectType(int idx) { return effectsApplied.at(idx); }
	int getEffectPotency(int idx) { return effectPotencies.at(idx); }
	weaponBuff getWeaponBuff() { return buffApplied; }
	attackType getAttackType() { return aType; }
	int getAttackRange() { return attackRange; }
	int getVigourCost() { return vigourCost; }

	//Flags
	bool usesSpellPower = false;
	bool usesDivinePower = false;
	bool addPermanentBuff = false;
	bool isAcidSpell = false; //Determines whether acid infusions are super-effective

private:

	//What we actually do
	effectVector effectsApplied;
	std::vector<int> effectPotencies;

	//How we're applied
	attackType aType;
	int attackRange; //If not melee; otherwise defines either range OR aoe radius

	//Special effects for buff spells
	weaponBuff buffApplied;

	//Casting cost
	int vigourCost = 1;

};

//Weapon arts
spell* attack_Splintering();
spell* attack_Quickstep();
spell* attack_StrongAttack();
spell* attack_Knockback();
spell* attack_MightyCleave();
spell* attack_Spearfishing();
spell* attack_Fillet();

//Wand/chime powers
spell* ability_PowerSurge();
spell* ability_AcidInfusion();
spell* ability_DivineFavour();
spell* ability_WyrdChannel();

//Arcane spells
spell* spell_MagicMissile();
spell* spell_ArcaneRadiance();
spell* spell_ArcaneBlade();
spell* spell_GottricsArcaneProtection();
spell* spell_Frostbolt();
spell* spell_AcidSpit();
spell* spell_AcidBlade();
spell* spell_AcidBurst();

//Prayers
spell* prayer_Restoration();
spell* prayer_WyrdChantOfStrength();
spell* prayer_ProfaneRadiance();

//Monster abilities
spell* ability_Charge();
spell* ability_Burst();
spell* ability_CrowArrow();
spell* ability_FeedingFrenzy();
spell* ability_SinkBeneath();

#endif