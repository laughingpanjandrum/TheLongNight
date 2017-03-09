
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
	spell(std::string name, TCODColor color, int attackRange, int physDamage, 
		effect otherEffect = NULL_EFFECT, int otherEffectPotency = 0);
	//This is for quickly constructing any spell with a single effect
	spell(std::string name, TCODColor color, attackType aType, int range, effect mainEff, int potency);
	//Dual-effect spell
	spell(std::string name, TCODColor color, attackType aType, int range, effect eff1, int pot1, effect eff2, int pot2);
	~spell();

	//Setting
	void setAttackType(attackType aType, int attackRange = 1);
	void addEffect(effect eff, int potency);
	void setVigourCost(int v) { vigourCost = v; }
	void setAttackRange(int r) { attackRange = r; }
	void setBuffApplied(damageType dtype, int damage) { buffApplied = weaponBuff(dtype, damage); }
	void setStatusEffectBuff(statusEffects etype, int damage) { buffApplied = weaponBuff(etype, damage); }
	void setDamageToCaster(int d) { damageToCaster = d; }

	//Getting
	bool hasEffect(effect eff);
	int getEffectsCount() { return effectsApplied.size(); }
	effect getEffectType(int idx) { return effectsApplied.at(idx); }
	int getEffectPotency(int idx) { return effectPotencies.at(idx); }
	weaponBuff getWeaponBuff() { return buffApplied; }
	attackType getAttackType() { return aType; }
	int getAttackRange() { return attackRange; }
	int getVigourCost() { return vigourCost; }
	int getDamageToCaster() { return damageToCaster; }

	//Animation data
	bool useAlternateAnimation = false;

	//Flags
	bool usesSpellPower = false;
	bool usesDivinePower = false;
	bool addPermanentBuff = false;
	bool isAcidSpell = false; //Determines whether acid infusions are super-effective
	bool isColdSpell = false;
	bool isProfane = false; //Buffed by profane items, bad for blessed people
	bool alwaysHitTarget = false; //Doesn't hit intermediate targets
	bool isStenchBased = false; //Stench resistance negates the effects
	bool inverseHealthScaling = false; //Power increases as the caster's health lowers

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
	int damageToCaster = 0;

};



typedef std::shared_ptr<spell> spellSharedPtr;
typedef std::vector<spellSharedPtr> spellVector;


//Weapon arts
spellSharedPtr attack_Splintering();
spellSharedPtr attack_Quickstep();
spellSharedPtr attack_StrongAttack();
spellSharedPtr attack_Knockback();
spellSharedPtr attack_MightyCleave();
spellSharedPtr attack_Spearfishing();
spellSharedPtr attack_Fillet();
spellSharedPtr attack_DivineSmite();
spellSharedPtr attack_BloodFeast();
spellSharedPtr attack_HolySummons();
spellSharedPtr attack_VoidSlash();
spellSharedPtr attack_VoidSurge();
spellSharedPtr attack_FlameSwathe();
spellSharedPtr attack_DripVenom();
spellSharedPtr attack_SuckOutLife();
spellSharedPtr attack_SpiderStrike();
spellSharedPtr attack_DeepCut();
spellSharedPtr attack_Lash();
spellSharedPtr attack_EtherealStrike();
spellSharedPtr attack_Frenzy();
spellSharedPtr attack_PercivelsFire();
spellSharedPtr attack_KnifeCast();
spellSharedPtr attack_LightningSlam();
spellSharedPtr attack_BillowingFlames();
spellSharedPtr attack_Penetrate();
spellSharedPtr attack_EtherealRay();
spellSharedPtr attack_PaleSlash();
spellSharedPtr attack_PoisonHook();
spellSharedPtr attack_FlameBreath();
spellSharedPtr attack_RatCleave();
spellSharedPtr attack_BlightingInfusion();
spellSharedPtr attack_Deathstrike();

//Wand/chime powers
spellSharedPtr ability_PowerSurge();
spellSharedPtr ability_AcidInfusion();
spellSharedPtr ability_EtherealSurge();
spellSharedPtr ability_TransitorySurge();
spellSharedPtr ability_Metamagic();
spellSharedPtr ability_GottricsPartingCall();
spellSharedPtr ability_StrengthOfMind();
spellSharedPtr ability_DivineFavour();
spellSharedPtr ability_WyrdChannel();
spellSharedPtr ability_Rotcall();
spellSharedPtr ability_MemoryOfLight();

spellSharedPtr ability_FogCloud();

//	Arcane spells
spellSharedPtr spell_MagicMissile();
spellSharedPtr spell_ArcaneRadiance();
//Utric, basic
spellSharedPtr spell_ArcaneBlade();
spellSharedPtr spell_GottricsArcaneProtection();
spellSharedPtr spell_Frostbolt();
//Utric, with waterlogged writings
spellSharedPtr spell_AcidSpit();
spellSharedPtr spell_AcidBlade();
spellSharedPtr spell_AcidBurst();
//Utric, with profaned writings
spellSharedPtr spell_ProfanedBlade();
spellSharedPtr spell_VoidJaunt();
spellSharedPtr spell_DevouringVoidCloud();
//Utric, with frostbitten writings
spellSharedPtr spell_FrostBlast();
spellSharedPtr spell_FrozenBlade();
spellSharedPtr spell_Chillbite();
//Utric, with singed writings
spellSharedPtr spell_LightningStrike();
spellSharedPtr spell_ElectricBlade();
spellSharedPtr spell_ArcLightning();
//Atalundra magic
spellSharedPtr spell_AtalundraArcaneSpear();
spellSharedPtr spell_AtalundraDeathSurge();
spellSharedPtr spell_AtalundraSoulBlade();

spellSharedPtr spell_SpitFire();
spellSharedPtr spell_StormLash();
spellSharedPtr spell_Firestorm();
spellSharedPtr spell_LightOfFarin();

//	Prayers
spellSharedPtr prayer_Restoration();
//Sold by Gorem
spellSharedPtr prayer_WyrdChantOfStrength();
//Muira, basic
spellSharedPtr prayer_RayOfLight();
spellSharedPtr prayer_BlessedRadiance();
spellSharedPtr prayer_RemovePoison();
//Orsyl's Tome of Prayer
spellSharedPtr prayer_DrawOutTheBlood();
spellSharedPtr prayer_ProfaneRadiance();
spellSharedPtr prayer_Intoxify();
//Divine Moonspark Tome
spellSharedPtr prayer_DivineRetribution();
spellSharedPtr prayer_YutriasDivineSpark();
spellSharedPtr prayer_YutriasProtection();
//Divine Tome of the Emissary
spellSharedPtr prayer_DivineRestoration();
spellSharedPtr prayer_DivineJudgement();
//Divine Nightmare Tome
spellSharedPtr prayer_NightmarePrayer();
spellSharedPtr prayer_WordOfUnmaking();
//Sold by Ydella
spellSharedPtr prayer_SinkBeneath();
spellSharedPtr prayer_Deathlink();
spellSharedPtr prayer_ProfoundStill();
spellSharedPtr prayer_SpidersPrayer();
spellSharedPtr prayer_TouchOfDoom();
//Misc
spellSharedPtr prayer_AstralPrison();
spellSharedPtr prayer_ProfaneRay();

//Monster abilities
spellSharedPtr ability_Charge();
spellSharedPtr ability_Burst();
spellSharedPtr ability_CrowArrow();
spellSharedPtr ability_FeedingFrenzy();
spellSharedPtr ability_DropOoze();
spellSharedPtr ability_ShadowWalk();
spellSharedPtr ability_WinterBlast();
spellSharedPtr ability_WreatheInFlame();
spellSharedPtr ability_DragBelow();
spellSharedPtr ability_SpitWeb();
spellSharedPtr attack_RavenousHunger();
spellSharedPtr ability_ShredSkin();
spellSharedPtr ability_Gnash();
spellSharedPtr ability_DevouringGaze();
spellSharedPtr ability_Deathwatch();

//Duke Vorten
spellSharedPtr ability_TentacleSwarm();
spellSharedPtr ability_Throw();
spellSharedPtr ability_VoidStep();

//Special
spellSharedPtr ability_WarpingJavelin();

#endif