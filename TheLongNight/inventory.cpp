#include "inventory.h"



inventory::inventory()
{
	//We get an equipment slot for each item type
	for (unsigned int i = 0; i < ALL_ITEM_TYPES.size(); i++) {
		equipped.push_back(inventorySlot(ALL_ITEM_TYPES[i]));
	}
	//And a list of items for each item type
	for (unsigned int i = 0; i < ALL_ITEM_TYPES.size(); i++) {
		carried.push_back(inventoryList(ALL_ITEM_TYPES[i]));
	}
}


inventory::~inventory()
{
}

/*
	SETTERS
*/

/*
Equips the item in the relevant slot
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
			else if (it.category == ITEM_BODY_ARMOUR)
				equippedArmour = static_cast<armour*>(which);
		}
	}
}

/*
Unequips item, but doesn't drop it
*/
void inventory::unequipItem(item * which)
{
	for (auto it : equipped) {
		if (it.equipped == which)
			it.equipped = nullptr;
	}
}

/*
Add an item to our storage, without equipping it.
*/
void inventory::addItem(item * which)
{
	//Find the proper list
	for (auto ilist : carried) {
		if (ilist.category == which->getCategory()) {
			ilist.add(which);
		}
	}
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
