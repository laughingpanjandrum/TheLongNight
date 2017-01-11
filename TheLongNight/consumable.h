
#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"


class consumable : public item
{
public:
	//Constructors
	consumable() : item() {}
	consumable(std::string name, int tileCode, TCODColor color) :
		item(name,tileCode,color,ITEM_CONSUMABLE) {}
	~consumable();
};

/*
	Consumable objects
*/

consumable* consumable_StarwaterDraught();

#endif
