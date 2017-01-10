#include "item.h"


/*
Basic constructor you should always use!
*/
item::item(std::string name, int tileCode, TCODColor color, itemTypes category): item(name,tileCode,color)
{
	this->category = category;
}

item::~item()
{
}


/*
Just gives you the names of the various categories. Useful!
*/
const std::string getItemCategoryName(itemTypes cat)
{
	switch (cat) {
	case(ITEM_WEAPON): return "Weapon";
	case(ITEM_BODY_ARMOUR): return "Body Armour";
	case(ITEM_HELMET): return "Headgear";
	case(ITEM_CONSUMABLE): return "Consumable";
	}
	return "Unknown";
}
