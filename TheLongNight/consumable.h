
#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "counter.h"
#include "spell.h"

class consumable : public item
{
public:

	//Constructors
	consumable() : item() {}
	consumable(std::string name, int tileCode, TCODColor color, std::string description);
	~consumable();

	//Quantity adjustments
	virtual const bool isStackable() { return true; }
	virtual const void add(int n = 1) { amount.increaseMaxValue(n); }
	void lose(int n = 1) { amount.decrease(n); }
	virtual int getAmountLeft() { return amount.getValue(); }
	virtual void const restoreToMax() { amount.restore(); }

	//Flava
	virtual std::string getMenuName();

	//Effects
	bool isRangedAttackItem() { return rangedAttack != nullptr; }
	effectVector getEffects() { return effectsApplied; }
	int getPotency() { return potency; }
	spellSharedPtr getRangedAttack() { return rangedAttack; }
	weaponBuff* getWeaponBuff() { return buff; }

	//Setting effects
	void addEffect(effect eff) { effectsApplied.push_back(eff); }
	void setPotency(int pot) { potency = pot; }
	void setRangedAttack(spellSharedPtr at) { rangedAttack = at; }
	void setWeaponBuff(weaponBuff* b) { buff = b; }

	//Flags
	bool consumeOnUse = true; //Doesn't have to get USED UP!
	bool addsPermanentBuff = false;

private:

	//Quantity!
	counter amount;

	//Effects and potency
	effectVector effectsApplied;
	int potency;

	//Ranged attack is treated as a spell, for ranged-attack consumables
	spellSharedPtr rangedAttack;

	//Weapon buff, for consumables that do that
	weaponBuff* buff;

};


typedef std::shared_ptr<consumable> consumableSharedPtr;
typedef std::vector<consumableSharedPtr> consumableVector;

/*
	Consumable objects
*/

consumableSharedPtr consumable_StarwaterDraught();
consumableSharedPtr consumable_InvigoratingTea();

consumableSharedPtr consumable_TinyRedFlower();
consumableSharedPtr consumable_TinyGreenFlower();
consumableSharedPtr consumable_BloodDrinkersEyes();
consumableSharedPtr consumable_IntoxicatingWine();
consumableSharedPtr consumable_BlackHoney();

consumableSharedPtr ranged_ThrowingKnives();
consumableSharedPtr ranged_HeavyJavelin();
consumableSharedPtr ranged_LaceratingKnives();
consumableSharedPtr ranged_PoisonThrowingKnives();
consumableSharedPtr ranged_CorrodingJar();
consumableSharedPtr ranged_VoidEssenceJar();
consumableSharedPtr ranged_PyromancersFlask();
consumableSharedPtr ranged_WitchsJar();
consumableSharedPtr ranged_LightingJavelin();
consumableSharedPtr ranged_FrostKnives();

consumableSharedPtr oil_PyromancersOil();
consumableSharedPtr oil_CorrosiveOil();
consumableSharedPtr oil_FrozenOil();
consumableSharedPtr oil_HolyWater();
consumableSharedPtr oil_CursedWater();
consumableSharedPtr oil_HangmansBlood();

consumableSharedPtr bell_VoidwalkersDancingBell();
consumableSharedPtr bell_VoidwalkersReturningBell();
consumableSharedPtr horn_HeraldsWhiteHorn();

#endif
