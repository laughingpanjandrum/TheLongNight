
#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "spell.h"
#include "utility.h"

typedef std::vector<statusEffects> statusEffectVector;
typedef std::vector<spell*> spellVector;

class weapon : public item
{
public:

	//Item categories
	weapon(): weapon("", 0, TCODColor::white, "") {}
	weapon(std::string name, int tileCode, TCODColor color, std::string description);
	//Just for monsters: creates a weapon that applies a status effect
	weapon(int baseDamage, const int attackSpeed, statusEffects eType, int statusEffectDamage);
	~weapon();

	//Override
	virtual std::string getMenuName();

	//Setting
	void setBasicAttributes(int damage, int attackDelay);
	void addDamageType(damageType dtype, int damage);
	void setDefence(int def) { defence = def; }
	void setBleedResist(int br) { bleedResist = br; }
	void addStatusEffect(statusEffects eType, int damage);
	void setSpecialAttack(spell* sp) { specialAttack = sp; }
	void makeOffhand() { category = ITEM_OFFHAND; } //Make this an offhand weapon

	//Getting
	int getDamage() { return damage; }
	int getDamageOfType(damageType dtype);
	int getAttackDelay() { return attackDelay; }
	spell* getSpecialAttack() { return specialAttack; }

	//Spell stuff: Getting
	spellVector getSpells() { return spellstore; }
	bool canAddSpell() { return spellstore.size() < spellstoreSize; }
	bool hasSpellStored(spell* sp);
	int getSpellstoreSize() { return spellstoreSize; }
	int getSpellPower() { return spellPower; }
	int getDivinePower() { return divinePower; }

	//Spell stuff: Setting
	void setSpellstoreSize(int size) { spellstoreSize = size; }
	void addSpell(spell* sp) { spellstore.push_back(sp); }
	void removeSpell(spell* sp);
	void setSpellPower(int power) { spellPower = power; }
	void setDivinePower(int power) { divinePower = power; }

	//Status effects
	int getStatusEffectCount() { return statusEffectType.size(); } //Returns number of status effects we apply
	statusEffects getStatusEffectType(int idx) { return statusEffectType.at(idx); }
	int getStatusEffectDamage(int idx) { return statusEffectDamage.at(idx); }

	//Buffing
	void setBuff(weaponBuff buff) { currentBuff = buff; }
	void clearBuff(weaponBuff buff) { currentBuff = weaponBuff(); }
	weaponBuff getCurrentBuff() { return currentBuff; }
	void setRune(weaponRune* r) { rune = r; }
	weaponRune* getRune() { return rune; }

	//Scaling
	void addScalingType(statScaling st) { scaling.push_back(st); }
	int getScalingDamage(statScaling st);

private:

	//Weapon deetz
	int damage;
	int attackDelay;

	//Special attack
	spell* specialAttack;

	//Spell storage
	spellVector spellstore;
	int spellstoreSize = 0;
	int spellPower = 0; //As a percent
	int divinePower = 0;

	//Special effects
	std::vector<int> damageTypes;
	statusEffectVector statusEffectType;
	std::vector<int> statusEffectDamage;

	//Buffing
	weaponBuff currentBuff;
	weaponRune* rune;

	//Scaling
	statScaleVector scaling;

};

std::string getAttackSpeedName(int del);

//	Weapons

//Starter weapons
weapon* weapon_SplinteredSword();
weapon* weapon_ThinKnife();
weapon* weapon_StraightSword();
weapon* weapon_Warhammer();

weapon* weapon_CrowKnightSword();
weapon* weapon_CrowKnife();
weapon* weapon_CrowHalfsword();
weapon* weapon_OldCrowsLongKnife();

weapon* weapon_NotchedGreatsword();
weapon* weapon_FishmansHarpoon();
weapon* weapon_FishmansKnife();

weapon* weapon_CityGuardianWarhammer();
weapon* weapon_BloodDrinkersKnife();

weapon* weapon_ProfaneGreatsword();

//Shields
weapon* shield_BatteredWoodenShield();
weapon* shield_WoodenWyrdShield();
weapon* shield_BatteredSteelShield();
weapon* shield_CityGuardianShield();

//Wands
weapon* wand_DriftwoodWand();
weapon* wand_FishmansToadstaff();

//Chimes
weapon* chime_ClericsCrackedChime();
weapon* chime_WyrdBellbranch();
weapon* chime_OrsylsProfaneChime();

#endif