
/*
Keeps track of stored inventory and equipped items.
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "categories.h"
#include "weapon.h"
#include "armour.h"


struct inventorySlot {
	inventorySlot(itemTypes category): category(category) {}
	item* equipped = nullptr; //Item equipped here
	itemTypes category; //What type of item can be equipped here
};

class inventory
{
	typedef std::vector<inventorySlot> slotVector;
public:
	inventory();
	~inventory();
	//Setters
	void equipItem(item* which);
	void unequipItem(item* which);
	//Getters
	item* getEquipped(itemTypes category);
	weapon* getWeapon() { return equippedWeapon; }
	armour* getArmour() { return equippedArmour; }
private:
	//Slots
	slotVector equipped;
	//Important items: we save pointers to these when they're equipped
	weapon* equippedWeapon;
	armour* equippedArmour;

};

#endif

