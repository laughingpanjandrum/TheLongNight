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
bool inventory::equipItem(itemSharedPtr which)
{
	//We might also want to make a special pointer to this item
	itemTypes cat = which->getCategory();
	if (cat == ITEM_WEAPON)
		equippedWeapon = std::static_pointer_cast<weapon>(which);
	else if (cat == ITEM_OFFHAND)
		equippedOffhand = std::static_pointer_cast<weapon>(which);
	else if (cat == ITEM_BODY_ARMOUR)
		equippedArmour = std::static_pointer_cast<armour>(which);
	else if (cat == ITEM_HELMET)
		equippedHelmet = std::static_pointer_cast<armour>(which);
	else if (cat == ITEM_CHARM)
		equippedCharm = std::static_pointer_cast<charm>(which);
	else if (cat == ITEM_CONSUMABLE)
		equipConsumable(std::static_pointer_cast<consumable>(which));
	else if (cat == ITEM_SPELL) //Currently only spells can fail to equip
		return equipSpell(std::static_pointer_cast<spell>(which));
	else if (cat == ITEM_MISC)
		equipMiscItem(std::static_pointer_cast<miscItem>(which));
	return true;
}

/*
Unequips item, but doesn't drop it
(NOT IMPLEMENTED)
*/
void inventory::unequipItem(itemSharedPtr which)
{
	itemTypes cat = which->getCategory();
}


/*
Switch secondary and equipped weapons.
Returns the newly-equipped weapon.
*/
weaponSharedPtr inventory::swapWeapon()
{
	weaponSharedPtr temp = equippedWeapon;
	equippedWeapon = secondaryWeapon;
	secondaryWeapon = temp;
	return equippedWeapon;
}

/*
Spells have to be equipped onto an item with spellstore.
Returns whether we were able to equip the spell.
*/
bool inventory::equipSpell(spellSharedPtr sp)
{
	//Can our main weapon hold it?
	weaponSharedPtr wp = getWeapon();
	if (wp != nullptr && wp->canAddSpell()) {
		wp->addSpell(sp);
		return true;
	}
	else {
		//If not, try offhand
		weaponSharedPtr off = getOffhand();
		//If it's already in here, remove it!
		if (off != nullptr) {
			if (off->hasSpellStored(sp)) {
				off->removeSpell(sp);
				return false;
			}
			else if (off->canAddSpell()) {
				off->addSpell(sp);
				return true;
			}
		}
	}
	//We didn't find a place to equip this item.
	return false;
}

/*
Remove spell from whatever we have it equipped on, if anything
(NEEDS TO BE IMPLEMENTED)
*/
void inventory::unequipSpell(spellSharedPtr sp)
{
}

/*
Only true if the spell is contained in something we have equipped.
*/
bool inventory::isSpellEquipped(spellSharedPtr sp)
{
	weaponSharedPtr wp = getWeapon();
	weaponSharedPtr off = getOffhand();
	if (wp != nullptr && wp->hasSpellStored(sp))
		return true;
	if (off != nullptr && off->hasSpellStored(sp))
		return true;
	return false;
}

/*
Equip a consumable to our CONSUMABLES HOTBAR.
*/
void inventory::equipConsumable(consumableSharedPtr c)
{
	
	//Iterator to item
	auto f = std::find(equippedConsumables.begin(), equippedConsumables.end(), c);
	
	//Is it already equipped?
	if (f != equippedConsumables.end()) {
		
		//If it's already equipped, let's UNEQUIP it!
		equippedConsumables.erase(f);
		//If this is our selected consumable, deselect it
		if (selectedConsumable == c)
			selectedConsumable = nullptr;
	
	}
	
	else {
		
		//See if we have room in the hotbar
		if (equippedConsumables.size() < MAX_CONSUMABLE_SLOTS) {
			//Yup, we're not equipping it twice
			equippedConsumables.push_back(c);
			//If we don't have a SELECTED CONSUMABLE, make it this one
			if (selectedConsumable == nullptr) {
				selectedConsumable = c;
			}
		
		}
	}
}

/*
Hard-sets our selected consumable.
*/
void inventory::setSelectedConsumable(consumableSharedPtr c)
{
	selectedConsumable = c;
}

/*
Returns whether the given item is in our HOTBAR.
*/
bool inventory::isConsumableEquipped(consumableSharedPtr c)
{
	return std::find(equippedConsumables.begin(), equippedConsumables.end(), c) != equippedConsumables.end();
}


/*
Misc items can do various things when equipped, but usually it's nothing!
*/
void inventory::equipMiscItem(miscItemSharedPtr m)
{
	if (m->isRunestone) {
		//Equip in current weapon, if possible
		weaponSharedPtr wp = getWeapon();
		if (wp != nullptr) {
			wp->setRune(m->getRune());
		}
	}
}


/*
We lose a misc item, usually a heart (due to heart trading)
*/
void inventory::removeMiscItem(miscItemSharedPtr m)
{
	auto itemList = &carried.at(ITEM_MISC).items;
	auto iter = std::find(itemList->begin(), itemList->end(), m);
	if (iter != itemList->end()) {
		itemList->erase(iter);
	}
}


/*
Returns whether we have the key that will unlock the given door tag.
*/
bool inventory::hasKey(std::string keyTag)
{
	//Search our MISCELLANEOUS ITEMS
	for (auto it : getItemList(ITEM_MISC)) {
		miscItemSharedPtr misc = std::static_pointer_cast<miscItem>(it);
		if (misc->isKey && misc->getKeyTag() == keyTag)
			return true;
	}
	return false;
}

/*
Add an item to our storage, without equipping it.
Returns TRUE if the item stacked with another item.
Returns FALSE if a new entry was created for this item.
*/
bool inventory::addItem(itemSharedPtr which)
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
	//We shouldn't get here, hopefully
	return false;
}

/*
	GETTERS
*/

/*
Returns first equipped item of the given category.
This is generally slower than getting it directly from the pointer!
*/
itemSharedPtr inventory::getEquipped(itemTypes category)
{
	switch (category) {
	case(ITEM_WEAPON): return getWeapon();
	case(ITEM_OFFHAND): return getOffhand();
	case(ITEM_BODY_ARMOUR): return getArmour();
	case(ITEM_HELMET): return getHelmet();
	case(ITEM_CHARM): return getCharm();
	}
	return nullptr;
}

/*
Return all carried items of the given type.
*/
itemVector inventory::getItemList(itemTypes category)
{
	//Find this category
	for (auto it : carried) {
		if (it.category == category)
			return it.items;
	}
	//If we don't find this category for some reason, return an empty vector
	return itemVector();
}

/*
Select the next consumable.
*/
void inventory::cycleConsumable()
{
	/*selectedConsumable++;
	if (selectedConsumable >= equippedConsumables.size())
		selectedConsumable = 0;*/
}

/*
Returns which equipped consumable we currently have selected.
*/
consumableSharedPtr inventory::getSelectedConsumable()
{
	return selectedConsumable;
}
