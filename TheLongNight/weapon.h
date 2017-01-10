
#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"

class weapon : public item
{
public:
	//Item categories
	weapon(): item() {}
	weapon(std::string name, int tileCode, TCODColor color) :
		item(name, tileCode, color, ITEM_WEAPON) {}
	~weapon();
};

#endif