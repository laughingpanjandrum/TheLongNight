#include "inventory.h"



inventory::inventory()
{
	//We get an equipment slot for each item type
	for (unsigned int i = 0; i < ALL_ITEM_TYPES.size(); i++) {
		equipped.push_back(inventorySlot(ALL_ITEM_TYPES[i]));
	}
}


inventory::~inventory()
{
}

/*
	SETTERS
*/

void inventory::equipItem(item * which)
{
	//Find slot for this category
	for (auto it : equipped) {
		if (it.category == which->getCategory()) {
			//Equip
			it.equipped = which;
			//We might also want to make a special pointer to this item
			if (it.category == ITEM_WEAPON)
				equippedWeapon = static_cast<weapon*>(which);
		}
	}
}

void inventory::unequipItem(item * which)
{
}

/*
	GETTERS
*/

/*
Returns first equipped item of the given category.
This is generally slower than getting it directly from the pointer!
*/
item * inventory::getEquipped(itemTypes category)
{
	for (auto it : equipped) {
		if (it.category == category)
			return it.equipped;
	}
	return nullptr;
}
