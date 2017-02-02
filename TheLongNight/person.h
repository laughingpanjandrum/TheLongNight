
#ifndef PERSON_H
#define PERSON_H

#include "element.h"
#include "counter.h"

#include "inventory.h"
#include "categories.h"
#include "weapon.h"
#include "consumable.h"
#include "charm.h"


/*
Buff struct.
This mostly exists to ensure that buffs can be REMOVED properly
and so that we can't stack identical buffs.
*/
struct buff {
	buff(std::string name, TCODColor color, effect effectApplied, int potency) :
		name(name), color(color), effectApplied(effectApplied), potency(potency) {}
	std::string name; //Buffs with the same name won't stack.
	TCODColor color;
	effect effectApplied;
	int potency;
};

typedef std::vector<buff*> buffVector;


/*
Struct for keeping track of a character with a statblock.
i.e. the PC.
These alter stats; don't use them for monsters with hardcoded stats.
*/
struct statline {
	statline(int h, int v, int s, int d, int a, int dv, int lvl = 0) : 
		health(h), vigour(v), strength(s), dexterity(d), arcana(a), devotion(dv), level(lvl) {}
	int health;
	int vigour;
	int strength;
	int dexterity;
	int arcana;
	int devotion;
	int level;
};

class person: public element
{
public:

	//Constructors/destructors
	person(): person("Player", PLAYER_TILE, TCODColor::white) {}
	person(std::string name, int tileCode, TCODColor color, std::string description = "");
	~person();

	//Stats (if we have any!)
	statline* stats;
	int getNextLevelCost() { return stats->level * 100; }
	int getScalingDamage(weapon* wp);
	void increaseMaxHealth(int amount) { health.increaseMaxValue(amount, true); }
	void increaseMaxVigour(int amount) { vigour.increaseMaxValue(amount, true); }

	//Special attributes
	bool isProfane() { return profane; }
	bool isBlessed() { return blessed; }

	//Getters
	counter getHealth() { return health; }
	counter getVigour() { return vigour; }
	person* getTarget() { return target; }
	int getMoveDelay();
	int getAttackDelay();
	int getBaseMeleeDamage() { return baseMeleeDamage; }
	int getDefence();
	int getDamageResist(damageType dr) { return damageResist.at(dr); }
	int getMeleeDamage();
	int getDamageOfType(damageType dtype);

	//Setters
	void setTarget(person* target) { this->target = target; }
	void clearTarget() { target = nullptr; }
	void setMaxHealth(int h) { health.setTo(h); }
	void setMaxVigour(int v) { vigour.setTo(v); }
	void addDamageResist(damageType dtype, int amount) { damageResist.at(dtype) += amount; }
	void addDefence(int amount) { baseDefence += amount; }

	//Taking damage and healing
	void addHealth(int amount);
	void takeDamage(int amount, damageType dtype = DAMAGE_UNTYPED);
	void addVigour(int amount) { vigour.increase(amount); }
	void loseVigour(int amount) { vigour.decrease(amount); }
	void fullRestore();
	void die();

	//	Special damage types
	void takeStatusEffectDamage(statusEffects eType, int damage);
	counter* getSpecialEffectBuildup(statusEffects eff);
	//Bleed
	int getBleedDuration() { return isBleeding; }
	void clearBleed() { bleedBuildup.clear(); }
	//Poison
	int getPoisonDuration() { return isPoisoned; }
	void clearPoison() { poisonBuildup.clear(); }
	//Blindness
	void blind(int duration) { blinding += duration; }
	bool isBlind() { return blinding > 0; }
	int getBlindnessDuration() { return blinding; }
	//Entangled
	void entangle(int duration) { entangling += duration; }
	bool isEntangled() { return entangling > 0; }
	int getEntangleDuration() { return entangling; }
	void struggle() { entangling--; }

	//Permanent buffs
	bool canAddBuff(buff* b);
	bool addBuff(buff* b) { buffs.push_back(b); return true; }
	bool addBuff(std::string name, TCODColor color, effect eType, int potency);
	void removeBuff(buff* b);
	void clearBuffs() { buffs.clear(); }
	buffVector getAllBuffs() { return buffs; }

	//Effects
	void applyEffect(effect eff, int potency);

	//Other buffs
	void clearFreeMoves() { freeMoves = 0; }
	void gainFreeMoves(int f) { freeMoves += f; }
	bool hasFreeMoves() { return freeMoves > 0; }
	void useFreeMove() { freeMoves--; }
	int scaleNextAttack = 0; //Percent damage bonus damage added to next attack
	int attacksPerHit = 1; //Multiattack count, resets to 1 after use
	int healthTrickle = 0; //We get 1 point of this per tick until it runs out
	int healthOnKill = 0; //Health restored when we kill something
	int invisibility = 0;
	int bleedScaling = 0; //Extra damage dealt when we have bleed
	int bleedDamageFactor = 1; //Multiplies both bleed damage taken and given
	int getFragmentPickupMult() { return fragmentPickupMultiplier; }

	//Debuffs
	int slowdown = 0;
	int damagePenalty = 0; //Percent reduction to melee damage.

	//Spell buffs
	int scaleNextSpell = 0; //Buff to next spell cast
	int scaleNextPrayer = 0; //Buff to next prayer cast
	int spellAcidInfusion = 0;
	int spellColdInfusion = 0;
	int coldDamageAppliesInfusion = 0; //If this is >0, taking cold damage gives us a cold infusion boost

	//Magic
	spell* buffNextMelee; //This spell is automatically discharged onto the next thing we attack in melee.
	void addSpellKnown(spell* sp) { spellsKnown.push_back(sp); }
	void removeSpellKnown(spell* sp);
	spellVector getSpellsKnown() { return spellsKnown; }
	spell* getCurrentSpell();
	void setCurrentSpell(spell* sp);
	void setCurrentSpell(int sp);
	void cycleSelectedSpell();
	int getSpellPower();
	int getDivinePower();
	void paySpellCost(spell* sp);

	//Equipping and unequipping items
	std::vector<item*> getItemsOfType(itemTypes category) { return items.getItemList(category); }
	void equipItem(item* which);
	void swapWeapon();
	void unequipItem(item* which);
	bool addItem(item* which);
	void doWeaponEquip(weapon* it);

	//Getting equipped items
	weapon* getWeapon() { return items.getWeapon(); }
	weapon* getOffhand() { return items.getOffhand(); }
	armour* getArmour() { return items.getArmour(); }
	armour* getHelmet() { return items.getHelmet(); }
	charm* getCharm() { return items.getCharm(); }
	bool hasItemEquipped(item* it);

	//Consumables
	consumable* getSelectedConsumable();
	consumableVector getConsumableList();
	void cycleConsumable() { items.cycleConsumable(); }
	void setCurrentConsumable(consumable* c);
	void restoreItemsToMax(); //Replenishes consumables

	//Keys
	bool hasKey(std::string keyTag);

	//Targeting
	void checkTargetValidity();

	//Stuff that happens on a timer
	void applyStatusEffects();
	void tick();

	//Flags
	bool isDead = false;
	bool isPlayer = true;
	bool isHostile = true;
	bool isShopkeeper = false;

protected:

	//Attributes
	counter health;
	counter vigour;
	int baseMeleeDamage = 1; //how much damage we do bare-handed
	int baseAttackSpeed = SPEED_NORMAL; //how quickly we attack bare-handed
	int baseMoveSpeed = SPEED_FAST; //how quickly we move if unarmoured

	bool profane = false;
	bool blessed = false;

	//Resistances/defence
	int baseDefence = 0;
	std::vector<int> damageResist;

	//Status effects
	int isBleeding = 0;
	counter bleedBuildup;
	int isPoisoned = 0;
	counter poisonBuildup;
	int blinding = 0;
	int entangling = 0;

	//Buffs
	buffVector buffs;
	int freeMoves = 0; //While this is >0, our move delay is zero; this ticks down each time we move.
	int fragmentPickupMultiplier = 1; //For PC only; increases number of fragments gained.
	int healingFactor = 1;

	//Equipment
	inventory items;

	//Magic
	spellVector spellsKnown; //List of spells we have stored in our weapon/wand/etc
	int selectedSpell = 0; //Highlighted spell
	int baseSpellPower = 0; //Flat bonus to spell power
	int baseDivinePower = 0; //Flat bonus to divine power

	//Current thing we're trying to kill
	person* target;

};

#endif

