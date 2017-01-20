
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
#include "miscItem.h"
#include "charm.h"

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
	bool equipItem(item* which);
	void unequipItem(item* which);
	bool addItem(item* which);

	//Getters
	itemlistVector getAllCarried() { return carried; }
	item* getEquipped(itemTypes category);
	std::vector<item*> getItemList(itemTypes category);
	std::vector<consumable*> getConsumables() { return equippedConsumables; }

	//Getting particular items
	weapon* getWeapon() { return equippedWeapon; }
	weapon* getSecondaryWeapon() { return secondaryWeapon; }
	weapon* getOffhand() { return equippedOffhand; }
	armour* getArmour() { return equippedArmour; }
	armour* getHelmet() { return equippedHelmet; }
	charm* getCharm() { return equippedCharm; }

	//Weapon swapping
	weapon* swapWeapon();

	//Spells, which are special
	bool equipSpell(spell* sp);
	void unequipSpell(spell* sp);
	bool isSpellEquipped(spell* sp);

	//Consumables
	void equipConsumable(consumable* c);
	void setSelectedConsumable(consumable* c);
	bool isConsumableEquipped(consumable* c);

	//Keys
	bool hasKey(std::string keyTag);

	//Selecting particular slots
	void cycleConsumable();
	consumable* getSelectedConsumable();

private:

	//Slots
	itemlistVector carried;
	
	//Pointers to each equipped item
	weapon* equippedWeapon;
	weapon* equippedOffhand;
	armour* equippedArmour;
	armour* equippedHelmet;
	charm* equippedCharm;
	consumable* selectedConsumable;

	//Secondary items for quick-swapping
	weapon* secondaryWeapon;

	//Consumables appear as a list
	std::vector<consumable*> equippedConsumables;
	//int selectedConsumable = 0;
};

#endif

