
#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "spell.h"

typedef std::vector<statusEffects> statusEffectVector;

class weapon : public item
{
public:

	//Item categories
	weapon(): item() {}
	weapon(std::string name, int tileCode, TCODColor color) :
		item(name, tileCode, color, ITEM_WEAPON) {}
	~weapon();

	//Setting
	void setBasicAttributes(int damage, int attackDelay);
	void addStatusEffect(statusEffects eType, int damage);
	void setSpecialAttack(spell* sp) { specialAttack = sp; }

	//Getting
	int getDamage() { return damage; }
	int getAttackDelay() { return attackDelay; }
	spell* getSpecialAttack() { return specialAttack; }

	//Status effects
	int getStatusEffectCount() { return statusEffectType.size(); } //Returns number of status effects we apply
	statusEffects getStatusEffectType(int idx) { return statusEffectType.at(idx); }
	int getStatusEffectDamage(int idx) { return statusEffectDamage.at(idx); }

private:

	//Weapon deetz
	int damage;
	int attackDelay;

	//Special attack
	spell* specialAttack;

	//Special effects
	statusEffectVector statusEffectType;
	std::vector<int> statusEffectDamage;

};

//Weapon types
weapon* weapon_SplinteredSword();

#endif