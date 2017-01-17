
#ifndef PERSON_H
#define PERSON_H

#include "element.h"
#include "counter.h"

#include "inventory.h"
#include "categories.h"
#include "weapon.h"
#include "consumable.h"

typedef std::vector<consumable*> consumableVector;

class person: public element
{
public:

	//Constructors/destructors
	person(): person("Player", PLAYER_TILE, TCODColor::white) {}
	person(std::string name, int tileCode, TCODColor color);
	~person();

	//Getters
	counter getHealth() { return health; }
	counter getVigour() { return vigour; }
	person* getTarget() { return target; }
	int getMoveDelay();
	int getAttackDelay();
	int getBaseMeleeDamage() { return baseMeleeDamage; }
	int getDefence();

	//Setters
	void setTarget(person* target) { this->target = target; }
	void clearTarget() { target = nullptr; }

	//Damage and healing
	void addHealth(int amount);
	void takeDamage(int amount);
	void addVigour(int amount) { vigour.increase(amount); }
	void loseVigour(int amount) { vigour.decrease(amount); }
	void fullRestore();
	void die();

	//Special damage types
	void takeStatusEffectDamage(statusEffects eType, int damage);
	counter* getSpecialEffectBuildup(statusEffects eff);
	int getBleedDuration() { return isBleeding; }

	//Buffing
	void gainFreeMoves(int f) { freeMoves += f; }
	bool hasFreeMoves() { return freeMoves > 0; }
	void useFreeMove() { freeMoves--; }

	//Magic
	spell* buffNextMelee; //This spell is automatically discharged onto the next thing we attack in melee.
	void addSpellKnown(spell* sp) { spellsKnown.push_back(sp); }
	void removeSpellKnown(spell* sp);
	spellVector getSpellsKnown() { return spellsKnown; }
	spell* getCurrentSpell();
	void cycleSelectedSpell();
	int getSpellPower();

	//Equipment
	std::vector<item*> getItemsOfType(itemTypes category) { return items.getItemList(category); }
	void equipItem(item* which);
	void unequipItem(item* which);
	void addItem(item* which);
	void cycleConsumable() { items.cycleConsumable(); }
	weapon* getWeapon() { return items.getWeapon(); }
	weapon* getOffhand() { return items.getOffhand(); }
	armour* getArmour() { return items.getArmour(); }
	armour* getHelmet() { return items.getHelmet(); }
	consumable* getSelectedConsumable();
	consumableVector getConsumableList();
	void restoreItemsToMax();
	bool hasItemEquipped(item* it);
	bool hasKey(std::string keyTag);

	//Targeting
	void checkTargetValidity();

	//Stuff that happens on a timer
	void applyStatusEffects();
	void tick();

	//Flags
	bool isDead = false;
	bool isPlayer = true;

protected:

	//Attributes
	counter health;
	counter vigour;
	int baseMeleeDamage = 1; //how much damage we do bare-handed
	int baseAttackSpeed = SPEED_NORMAL; //how quickly we attack bare-handed
	int baseMoveSpeed = SPEED_FAST; //how quickly we move if unarmoured

	//Resistances

	//Status effects
	int isBleeding = 0;
	counter bleedBuildup;

	//Buffs
	int freeMoves = 0; //While this is >0, our move delay is zero; this ticks down each time we move.

	//Equipment
	inventory items;

	//Magic
	spellVector spellsKnown; //List of spells we have stored in our weapon/wand/etc
	int selectedSpell = 0; //Highlighted spell
	int baseSpellPower = 0; //Flat bonus to spell power

	//Current thing we're trying to kill
	person* target;

};

#endif

