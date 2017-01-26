
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
	spell* getRangedAttack() { return rangedAttack; }

	//Setting effects
	void addEffect(effect eff) { effectsApplied.push_back(eff); }
	void setPotency(int pot) { potency = pot; }
	void setRangedAttack(spell* at) { rangedAttack = at; }

private:

	//Quantity!
	counter amount;

	//Effects and potency
	effectVector effectsApplied;
	int potency;

	//Ranged attack is treated as a spell, for ranged-attack consumables
	spell* rangedAttack;

};

typedef std::vector<consumable*> consumableVector;

/*
	Consumable objects
*/

consumable* consumable_StarwaterDraught();
consumable* consumable_InvigoratingTea();

consumable* consumable_TinyRedFlower();

consumable* ranged_ThrowingKnives();
consumable* ranged_LaceratingKnives();
consumable* ranged_CorrodingJar();
consumable* ranged_PyromancersFlask();

#endif
