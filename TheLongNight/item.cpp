#include "item.h"


/*
Basic constructor you should always use!
*/
item::item(std::string name, int tileCode, TCODColor color, itemTypes category, std::string description)
	: item(name, tileCode, color, description)
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
	case(ITEM_OFFHAND): return "Offhand";
	case(ITEM_BODY_ARMOUR): return "Body Armour";
	case(ITEM_HELMET): return "Headgear";
	case(ITEM_CONSUMABLE): return "Consumable";
	case(ITEM_SPELL): return "Spell";
	}
	return "Unknown";
}
