#include "inventory.h"



inventory::inventory()
{
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
	//We might also want to make a special pointer to this item
	itemTypes cat = which->getCategory();
	if (cat == ITEM_WEAPON)
		equippedWeapon = static_cast<weapon*>(which);
	else if (cat == ITEM_OFFHAND)
		equippedOffhand = static_cast<weapon*>(which);
	else if (cat == ITEM_BODY_ARMOUR)
		equippedArmour = static_cast<armour*>(which);
	else if (cat == ITEM_HELMET)
		equippedHelmet = static_cast<armour*>(which);
	else if (cat == ITEM_CONSUMABLE && equippedConsumables.size() < MAX_CONSUMABLE_SLOTS) {
		//We get a new consumable
		equippedConsumables.push_back(static_cast<consumable*>(which));
	}
}

/*
Unequips item, but doesn't drop it
(NOT IMPLEMENTED)
*/
void inventory::unequipItem(item * which)
{
	itemTypes cat = which->getCategory();
}

/*
Add an item to our storage, without equipping it.
Returns TRUE if the item stacked with another item.
Returns FALSE if a new entry was created for this item.
*/
bool inventory::addItem(item * which)
{
	//Find the proper list
	for (auto ilist = carried.begin(); ilist != carried.end(); ilist++) {
		if ((*ilist).category == which->getCategory()) {
			//Found the correct list, now check for stacking
			if (which->isStackable()) {
				//Is there anything we can stack with?
				for (auto item : (*ilist).items) {
					if (item->getName() == which->getName()) {
						item->add(which->getAmountLeft());
						return true;
					}
				}
			}
			//If we failed to stack, we just append the item
			(*ilist).add(which);
			return false;
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
	switch (category) {
	case(ITEM_WEAPON): return getWeapon();
	case(ITEM_OFFHAND): return getOffhand();
	case(ITEM_BODY_ARMOUR): return getArmour();
	case(ITEM_HELMET): return getHelmet();
	}
	return nullptr;
}

/*
Return all carried items of the given type.
*/
std::vector<item*> inventory::getItemList(itemTypes category)
{
	//Find this category
	for (auto it : carried) {
		if (it.category == category)
			return it.items;
	}
	//If we don't find this category for some reason, return an empty vector
	return std::vector<item*>();
}

/*
Select the next consumable.
*/
void inventory::cycleConsumable()
{
	selectedConsumable++;
	if (selectedConsumable >= equippedConsumables.size())
		selectedConsumable = 0;
}

/*
Returns which equipped consumable we currently have selected.
*/
consumable * inventory::getSelectedConsumable()
{
	if (selectedConsumable < equippedConsumables.size())
		return equippedConsumables.at(selectedConsumable);
	return nullptr;
}
