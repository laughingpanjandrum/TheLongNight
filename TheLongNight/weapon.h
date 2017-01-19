
#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "spell.h"

typedef std::vector<statusEffects> statusEffectVector;
typedef std::vector<spell*> spellVector;

class weapon : public item
{
public:

	//Item categories
	weapon(): item() {}
	weapon(std::string name, int tileCode, TCODColor color, std::string description) :
		item(name, tileCode, color, ITEM_WEAPON, description) {}
	~weapon();

	//Setting
	void setBasicAttributes(int damage, int attackDelay);
	void setDefence(int def) { defence = def; }
	void addStatusEffect(statusEffects eType, int damage);
	void setSpecialAttack(spell* sp) { specialAttack = sp; }
	void setSpellstoreSize(int size) { spellstoreSize = size; }
	void addSpell(spell* sp) { spellstore.push_back(sp); }
	void setSpellPower(int power) { spellPower = power; }
	void setDivinePower(int power) { divinePower = power; }
	void makeOffhand() { category = ITEM_OFFHAND; } //Make this an offhand weapon

	//Getting
	int getDamage() { return damage; }
	int getAttackDelay() { return attackDelay; }
	int getDefence() { return defence; }
	spell* getSpecialAttack() { return specialAttack; }

	//Spell stuff
	spellVector getSpells() { return spellstore; }
	bool canAddSpell() { return spellstore.size() < spellstoreSize; }
	bool hasSpellStored(spell* sp);
	int getSpellstoreSize() { return spellstoreSize; }
	int getSpellPower() { return spellPower; }
	int getDivinePower() { return divinePower; }

	//Status effects
	int getStatusEffectCount() { return statusEffectType.size(); } //Returns number of status effects we apply
	statusEffects getStatusEffectType(int idx) { return statusEffectType.at(idx); }
	int getStatusEffectDamage(int idx) { return statusEffectDamage.at(idx); }

	//Scaling
	void addScalingType(statScaling st) { scaling.push_back(st); }
	int getScalingDamage(statScaling st);

private:

	//Weapon deetz
	int damage;
	int attackDelay;
	int defence = 0;

	//Special attack
	spell* specialAttack;

	//Spell storage
	spellVector spellstore;
	int spellstoreSize = 0;
	int spellPower = 0; //As a percent
	int divinePower = 0;

	//Special effects
	statusEffectVector statusEffectType;
	std::vector<int> statusEffectDamage;

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

//Shields
weapon* shield_BatteredWoodenShield();

//Wands/chimes
weapon* wand_DriftwoodWand();
weapon* chime_ClericsCrackedChime();

#endif