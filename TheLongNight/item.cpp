#include "item.h"


/*
Basic constructor you should always use!
*/
item::item(std::string name, int tileCode, TCODColor color, itemTypes category): item(name,tileCode,color,category)
{
	this->category = category;
}

item::~item()
{
}
