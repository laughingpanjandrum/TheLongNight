
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
	statline() {}
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

	typedef std::shared_ptr<person> personSharedPtr;

	//Constructors/destructors
	person(): person("Stardrift Pilgrim", PLAYER_TILE, TCODColor::white) {}
	person(std::string name, int tileCode, TCODColor color, std::string description = "");
	//Copy constructor
	person(const person& other);
	~person();

	//Stats (if we have any!)
	statline* stats;
	int getNextLevelCost() { return stats->level * 100; }
	int getScalingDamage(weaponSharedPtr wp);
	void increaseMaxHealth(int amount) { health.increaseMaxValue(amount, true); }
	void increaseMaxVigour(int amount) { vigour.increaseMaxValue(amount, true); }

	//Special attributes
	bool isProfane() { return profane; }
	bool isBlessed() { return blessed; }

	//Getters
	counter getHealth() { return health; }
	counter getVigour() { return vigour; }
	personSharedPtr getTarget() { return target; }
	int getMoveDelay();
	int getAttackDelay();
	int getBaseMeleeDamage() { return baseMeleeDamage; }
	int getDefence();
	int getDamageResist(damageType dr) { return damageResist.at(dr); }
	bool isWeakTo(damageType dr) { return damageWeakness.at(dr); }
	int getMeleeDamage();
	int getDamageOfType(damageType dtype);
	bool isStinkResistant() { return stinkResist > 0; }
	bool isImmuneToVoid() { return immuneToVoid > 0; }
	void makeImmuneToVoid() { immuneToVoid += 1; }

	//Setters
	void setTarget(personSharedPtr target) { this->target = target; }
	void clearTarget() { target = nullptr; }
	void setMaxHealth(int h) { health.setTo(h); }
	void setMaxVigour(int v) { vigour.setTo(v); }
	void addDamageResist(damageType dtype, int amount) { damageResist.at(dtype) += amount; }
	void addDefence(int amount) { baseDefence += amount; }
	void addWeakness(int dtype) { damageWeakness.at(dtype) = true; }

	//Taking damage and healing
	void addHealth(int amount);
	void takeDamage(int amount, damageType dtype = DAMAGE_UNTYPED);
	void applyDamageAffliction(int amount, damageType dtype);
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
	//Plague
	int getPlagueDamage() { return plagueDamage; }
	void clearPlague() { plagueBuildup.clear(); }
	//Blindness
	void blind(int duration) { blinding += duration; }
	bool isBlind() { return blinding > 0; }
	int getBlindnessDuration() { return blinding; }
	//Entangled
	void entangle(int duration);
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
	void gainFreeMoves(int f);
	void gainFlatDamageBuff(int d);
	bool hasFreeMoves() { return freeMoves > 0; }
	void useFreeMove() { freeMoves--; }
	int scaleNextAttack = 0; //Percent damage bonus damage added to next attack
	int killDamageBuff = 0; //Cumulative damage buff, added on kill; cleared on rest
	int attacksPerHit = 1; //Multiattack count, resets to 1 after use
	int healthTrickle = 0; //We get 1 point of this per tick until it runs out
	int healthOnKill = 0; //Health restored when we kill something
	int invisibility = 0;
	int bleedScaling = 0; //Extra damage dealt when we have bleed
	int defenceWhenPoisoned = 0; //Buff to defence when we're poisoned
	int attackWhenPoisoned = 0; //Buff to damage when we're poisoned
	int bleedDamageFactor = 1; //Multiplies both bleed damage taken and given
	int getFragmentPickupMult() { return fragmentPickupMultiplier; }
	bool immuneToEntangle = false;
	int divineRetribution = 0; //Melee attackers take damage in return
	int lowHealthDamageBuff = 0; //Gain buffs to damage when health reaches certain thresholds
	int instantSpellCast = 0; //If >0, our next spell takes no time to cast
	int percentBuffHolyDamage = 0; //Buff to holy damage dealt
	int deathfireInfusion = 0; //We explode, dealing fire damage, upon death
	int deathlink = 0; //If we die, we return to life with this much hp (so long as it's >0)
	int silence = 0; //If >0, can't cast spells
	int tempDefenceBuff = 0; //Defensive buff to next attack only
	int tempElementalDefenceBuff = 0; //Defensive buff to next elemental defence only

	//Elemental scaling
	int acidDamageScaling = 0;
	int coldDamageScaling = 0;
	int electricDamageScaling = 0;
	int fireDamageScaling = 0;

	//Debuffs
	int slowdown = 0;
	int damagePenalty = 0; //Percent reduction to melee damage.
	int fear = 0; //ais have to run away

	//Elemental effects
	int acidic = 0; //Acid; slow damage, long time
	int frozen = 0; //Cold; reduces attack
	int burning = 0; //Fire; big damage, short time
	int electrified = 0; //Electric; reduces defence
	int profaneAffliction = 0; //Profane; reduces profane resistance
	int holyAffliction = 0; //Holy; reduces holy resistance

	//Spell buffs
	int scaleNextSpell = 0; //Buff to next spell cast
	int scaleNextPrayer = 0; //Buff to next prayer cast
	int nextSpellCostAdjust = 0; //Change to the cost of your next spell
	int spellAcidInfusion = 0;
	int spellColdInfusion = 0;
	int coldDamageAppliesInfusion = 0; //If this is >0, taking cold damage gives us a cold infusion boost

	//Magic
	spellSharedPtr buffNextMelee; //This spell is automatically discharged onto the next thing we attack in melee.
	void addSpellKnown(spellSharedPtr sp) { spellsKnown.push_back(sp); }
	void removeSpellKnown(spellSharedPtr sp);
	spellVector getSpellsKnown() { return spellsKnown; }
	spellSharedPtr getCurrentSpell();
	void setCurrentSpell(spellSharedPtr sp);
	void setCurrentSpell(int sp);
	void cycleSelectedSpell();
	int getSpellPower();
	int getDivinePower();
	void paySpellCost(spellSharedPtr sp);

	//Equipping and unequipping items
	itemVector getItemsOfType(itemTypes category) { return items.getItemList(category); }
	itemVector getAllEquippedItems();
	void equipItem(itemSharedPtr which);
	void swapWeapon();
	void unequipItem(itemSharedPtr which);
	bool addItem(itemSharedPtr which);
	void loseItemForever(itemSharedPtr which);
	void doWeaponEquip(weaponSharedPtr it);
	void loseKey(std::string keyTag);
	bool isRunestoneEquipped(miscItemSharedPtr r);

	//Getting equipped items
	weaponSharedPtr getWeapon() { return items.getWeapon(); }
	weaponSharedPtr getOffhand() { return items.getOffhand(); }
	armourSharedPtr getArmour() { return items.getArmour(); }
	armourSharedPtr getHelmet() { return items.getHelmet(); }
	charmVector getCharms() { return items.getCharms(); }
	bool hasItemEquipped(itemSharedPtr it);

	//Consumables
	consumableSharedPtr getSelectedConsumable();
	consumableVector getConsumableList();
	void cycleConsumable() { items.cycleConsumable(); }
	void setCurrentConsumable(consumableSharedPtr c);
	void restoreItemsToMax(); //Replenishes consumables

	//Keys
	bool hasKey(std::string keyTag);
	bool hasPaleCrown(); //Returns whether we have this specific special item.

	//Targeting
	void checkTargetValidity();

	//Stuff that happens on a timer
	void applyStatusEffects();
	void tick();

	//Lighting
	void setLightEmitted(float e) { emitsLight = e; }
	float getLightEmitted() { return emitsLight; }
	int detectionRange = 12; //Max range at which we can be detected.

	//Flags
	bool canSwapWithAllies = false;
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
	std::vector<bool> damageWeakness;
	int stinkResist = 0;
	int immuneToVoid = 0;

	//Status effects
	int isBleeding = 0;
	counter bleedBuildup;
	int isPoisoned = 0;
	counter poisonBuildup;
	int plagueDamage = 0;
	counter plagueBuildup;
	int blinding = 0;
	int entangling = 0;

	//Buffs
	buffVector buffs;
	int freeMoves = 0; //While this is >0, our move delay is zero; this ticks down each time we move.
	int fragmentPickupMultiplier = 1; //For PC only; increases number of fragments gained.
	int healingFactor = 1;
	int flatDamageBuff = 0; //Accumulating damage buff, cleared on death

	//Equipment
	inventory items;

	//Magic
	spellVector spellsKnown; //List of spells we have stored in our weapon/wand/etc
	int selectedSpell = 0; //Highlighted spell
	int baseSpellPower = 0; //Flat bonus to spell power
	int baseDivinePower = 0; //Flat bonus to divine power

	//Current thing we're trying to kill
	personSharedPtr target;

	//Lighting
	float emitsLight = 0;

	//Constants
	const static int MAX_FLAT_DAMAGE_BUFF = 40;

};

typedef std::shared_ptr<person> personSharedPtr;
typedef std::vector<personSharedPtr> personVector;

#endif

