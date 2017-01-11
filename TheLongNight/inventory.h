
/*
Keeps track of stored inventory and equipped items.
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "categories.h"
#include "weapon.h"
#include "armour.h"
#include "consumable.h"

//Slot with list of carried items
struct inventoryList {
	inventoryList(itemTypes category): category(category) {}
	void add(item* it) { items.push_back(it); }
	std::vector<item*> items;
	itemTypes category;
};

class inventory
{
	typedef std::vector<inventoryList> itemlistVector;
public:
	inventory();
	~inventory();
	//Constants
	static const int MAX_CONSUMABLE_SLOTS = 5;
	//Setters
	void equipItem(item* which);
	void unequipItem(item* which);
	void addItem(item* which);
	//Getters
	itemlistVector getAllCarried() { return carried; }
	item* getEquipped(itemTypes category);
	std::vector<item*> getItemList(itemTypes category);
	weapon* getWeapon() { return equippedWeapon; }
	armour* getArmour() { return equippedArmour; }
	std::vector<consumable*> getConsumables() { return equippedConsumables; }
private:
	//Slots
	itemlistVector carried;
	//Important items: we save pointers to these when they're equipped
	weapon* equippedWeapon;
	armour* equippedArmour;
	std::vector<consumable*> equippedConsumables;

};

#endif

