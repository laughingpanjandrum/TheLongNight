
#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "spell.h"
#include "utility.h"

typedef std::vector<statusEffects> statusEffectVector;

class weapon : public item
{
public:

	//Basic constructor
	weapon(): weapon("", 0, TCODColor::white, "") {}
	weapon(std::string name, int tileCode, TCODColor color, std::string description);
	//Just for monsters: creates a weapon that applies a status effect
	weapon(int baseDamage, const int attackSpeed, statusEffects eType, int statusEffectDamage);
	//Just for monsters: a weapon with a special damage type
	weapon(int baseDamage, const int attackSpeed, damageType dtype, int specialDamage);
	//Destructor
	~weapon();

	//Override
	virtual std::string getMenuName();

	//Setting
	void setBasicAttributes(int damage, int attackDelay);
	void setDamagePenalty(int dp) { damagePenalty = dp; }
	void addDamageType(damageType dtype, int damage);
	void setDefence(int def) { defence = def; }
	void setBleedResist(int br) { bleedResist = br; }
	void addStatusEffect(statusEffects eType, int damage);
	void setSpecialAttack(spellSharedPtr sp) { specialAttack = sp; }
	void makeOffhand() { category = ITEM_OFFHAND; } //Make this an offhand weapon

	//Getting
	int getDamage();
	int getDamagePenalty() { return damagePenalty; }
	int getDamageOfType(damageType dtype);
	int getAttackDelay() { return attackDelay; }
	spellSharedPtr getSpecialAttack() { return specialAttack; }

	//Spell stuff: Getting
	spellVector getSpells() { return spellstore; }
	bool canAddSpell() { return spellstore.size() < spellstoreSize; }
	bool hasSpellStored(spellSharedPtr sp);
	int getSpellstoreSize() { return spellstoreSize; }
	int getSpellPower() { return spellPower; }
	int getDivinePower() { return divinePower; }

	//Spell stuff: Setting
	void setSpellstoreSize(int size) { spellstoreSize = size; }
	void addSpell(spellSharedPtr sp) { spellstore.push_back(sp); }
	void removeSpell(spellSharedPtr sp);
	void setSpellPower(int power) { spellPower = power; }
	void setDivinePower(int power) { divinePower = power; }

	//Status effects
	int getStatusEffectCount() { return statusEffectDamage.size(); } //Returns number of status effects we apply
	//statusEffects getStatusEffectType(int idx) { return statusEffectType.at(idx); }
	int getStatusEffectDamage(statusEffects etype);

	//Buffing
	void setBuff(weaponBuff buff) { currentBuff = buff; }
	void clearBuff(weaponBuff buff) { currentBuff = weaponBuff(); }
	weaponBuff getCurrentBuff() { return currentBuff; }
	void setRune(weaponRune* r) { rune = r; }
	weaponRune* getRune() { return rune; }

	//Scaling
	void addScalingType(statScaling st) { scaling.push_back(st); }
	int getScalingDamage(statScaling st);
	bool getRunestoneDamage(damageType dtype);
	bool isProfane = false; //Determines interactions with other profane stuff
	bool scaleWithDamage = false; //Gains bonus damage when hp is lower.

	//Special effects
	int getDamageToSelf() { return damageToSelf; }
	int getDamageReservoir() { return damageReservoir; }
	void addToDamageReservoir(int a) { damageReservoir += a; }
	void clearDamageReservoir() { damageReservoir = 0; }
	void setDamageToSelf(int d) { damageToSelf = d; }

private:

	//Weapon deetz
	int damage;
	int attackDelay;
	int damagePenalty; //Mostly applies to shields and such

	//Special attack
	spellSharedPtr specialAttack;

	//Spell storage
	spellVector spellstore;
	int spellstoreSize = 0;
	int spellPower = 0; //As a percent
	int divinePower = 0;

	//Special effects
	std::vector<int> damageTypes;
	std::vector<int> statusEffectDamage;
	int damageReservoir = 0; //Built-up damage is restored to you on a kill.
	int damageToSelf = 0;  //Damage dealt to attacker on hit.

	//Buffing
	weaponBuff currentBuff;
	weaponRune* rune;

	//Scaling
	statScaleVector scaling;

};


std::string getAttackSpeedName(int del);

typedef std::shared_ptr<weapon> weaponSharedPtr;

//	Weapons

//Starter weapons
weaponSharedPtr weapon_SplinteredSword();
weaponSharedPtr weapon_ThinKnife();
weaponSharedPtr weapon_StraightSword();
weaponSharedPtr weapon_Warhammer();

weaponSharedPtr weapon_CrowKnightSword();
weaponSharedPtr weapon_CrowKnife();
weaponSharedPtr weapon_CrowHalfsword();
weaponSharedPtr weapon_OldCrowsLongKnife();

weaponSharedPtr weapon_NotchedGreatsword();
weaponSharedPtr weapon_FishmansHarpoon();
weaponSharedPtr weapon_FishmansKnife();

weaponSharedPtr weapon_CityGuardianWarhammer();
weaponSharedPtr weapon_BloodDrinkersKnife();
weaponSharedPtr weapon_KythielsScythe();
weaponSharedPtr weapon_VoidTouchedKnife();
weaponSharedPtr weapon_VoidCrystalGreatsword();
weaponSharedPtr weapon_ArmOfTheDuke();

weaponSharedPtr weapon_DragonboneSword();

weaponSharedPtr weapon_SerpentsTooth();
weaponSharedPtr weapon_CrimsonNail();
weaponSharedPtr weapon_SpiderboneShard();

weaponSharedPtr weapon_SacrificialKnife();
weaponSharedPtr weapon_SlaveMastersWhip();
weaponSharedPtr weapon_LadyTvertsClaws();

weaponSharedPtr weapon_EtherealSword();
weaponSharedPtr weapon_SentinelsPike();

weaponSharedPtr weapon_CorensGreataxe();
weaponSharedPtr weapon_KhallesHolyScythe();

weaponSharedPtr weapon_SirPercivelsSword();

weaponSharedPtr weapon_VoidfireKnife();
weaponSharedPtr weapon_LightningWarhammer();
weaponSharedPtr weapon_IetrasFlameScythe();

weaponSharedPtr weapon_EtherealGreatsword();
weaponSharedPtr weapon_MoonpaleScythe();
weaponSharedPtr weapon_GravekeeperSpear();
weaponSharedPtr weapon_SelvixsHookblade();
weaponSharedPtr weapon_FarinsTorch();

weaponSharedPtr weapon_RatboneCleaver();

weaponSharedPtr weapon_ProfaneGreatsword();

//Shields
weaponSharedPtr shield_BatteredWoodenShield();
weaponSharedPtr shield_WoodenWyrdShield();
weaponSharedPtr shield_BatteredSteelShield();
weaponSharedPtr shield_CityGuardianShield();
weaponSharedPtr shield_VoidTouchedShield();
weaponSharedPtr shield_DragonboneShield();
weaponSharedPtr shield_EtherealShield();
weaponSharedPtr shield_KhallesBellShield();
weaponSharedPtr shield_GhostlyShield();
weaponSharedPtr shield_SirPercivelsShield();

//Wands
weaponSharedPtr wand_DriftwoodWand();
weaponSharedPtr wand_FishmansToadstaff();
weaponSharedPtr wand_EtherealWand();
weaponSharedPtr wand_BleachwoodWand();
weaponSharedPtr wand_SparrowsStaff();
weaponSharedPtr wand_MoshkasSingingStaff();
weaponSharedPtr wand_GottricsWand();

//Chimes
weaponSharedPtr chime_ClericsCrackedChime();
weaponSharedPtr chime_GreenKnightsChime();
weaponSharedPtr chime_RotbloodBell();
weaponSharedPtr chime_WyrdBellbranch();
weaponSharedPtr chime_OrsylsProfaneChime();

#endif