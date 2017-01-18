
#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "counter.h"

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
	effectVector getEffects() { return effectsApplied; }
	int getPotency() { return potency; }

	//Setting effects
	void addEffect(effect eff) { effectsApplied.push_back(eff); }
	void setPotency(int pot) { potency = pot; }

private:

	//Quantity!
	counter amount;

	//Effects and potency
	effectVector effectsApplied;
	int potency;

};

typedef std::vector<consumable*> consumableVector;

/*
	Consumable objects
*/

consumable* consumable_StarwaterDraught();
consumable* consumable_InvigoratingTea();

#endif
