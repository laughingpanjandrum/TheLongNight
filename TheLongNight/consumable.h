
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
private:
	//Quantity!
	counter amount;
};

/*
	Consumable objects
*/

consumable* consumable_StarwaterDraught();

#endif
