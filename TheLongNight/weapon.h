
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
	void makeOffhand() { category = ITEM_OFFHAND; } //Make this an offhand weapon

	//Getting
	int getDamage() { return damage; }
	int getAttackDelay() { return attackDelay; }
	int getDefence() { return defence; }
	spell* getSpecialAttack() { return specialAttack; }
	spellVector getSpells() { return spellstore; }
	bool canAddSpell() { return spellstore.size() < spellstoreSize; }
	bool hasSpellStored(spell* sp);
	int getSpellstoreSize() { return spellstoreSize; }
	int getSpellPower() { return spellPower; }

	//Status effects
	int getStatusEffectCount() { return statusEffectType.size(); } //Returns number of status effects we apply
	statusEffects getStatusEffectType(int idx) { return statusEffectType.at(idx); }
	int getStatusEffectDamage(int idx) { return statusEffectDamage.at(idx); }

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

	//Special effects
	statusEffectVector statusEffectType;
	std::vector<int> statusEffectDamage;

};

std::string getAttackSpeedName(int del);

//Weapons
weapon* weapon_SplinteredSword();
weapon* weapon_CrowKnightSword();
weapon* weapon_CrowKnife();

//Shields
weapon* shield_BatteredWoodenShield();

//Wands
weapon* wand_DriftwoodWand();

#endif