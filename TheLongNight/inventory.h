
/*
Keeps track of stored inventory and equipped items.
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "categories.h"
#include "weapon.h"
#include "armour.h"

//Slot with equipped item
struct inventorySlot {
	inventorySlot(itemTypes category): category(category) {}
	item* equipped = nullptr; //Item equipped here
	itemTypes category; //What type of item can be equipped here
};

//Slot with list of carried items
struct inventoryList {
	inventoryList(itemTypes category): category(category) {}
	void add(item* it) { items.push_back(it); }
	std::vector<item*> items;
	itemTypes category;
};

class inventory
{
	typedef std::vector<inventorySlot> slotVector;
	typedef std::vector<inventoryList> itemlistVector;
public:
	inventory();
	~inventory();
	//Setters
	void equipItem(item* which);
	void unequipItem(item* which);
	void addItem(item* which);
	//Getters
	item* getEquipped(itemTypes category);
	weapon* getWeapon() { return equippedWeapon; }
	armour* getArmour() { return equippedArmour; }
private:
	//Slots
	slotVector equipped;
	itemlistVector carried;
	//Important items: we save pointers to these when they're equipped
	weapon* equippedWeapon;
	armour* equippedArmour;

};

#endif

