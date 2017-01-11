
#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "counter.h"

class consumable : public item
{
public:
	//Constructors
	consumable() : item() {}
	consumable(std::string name, int tileCode, TCODColor color);
	~consumable();
	//Quantity adjustments
	void add(int n = 1) { amount.increase(n); }
	void lose(int n = 1) { amount.decrease(n); }
	int getAmountLeft() { return amount.getValue(); }
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

/*
	Consumable objects
*/

consumable* consumable_StarwaterDraught();
consumable* consumable_InvigoratingTea();

#endif
