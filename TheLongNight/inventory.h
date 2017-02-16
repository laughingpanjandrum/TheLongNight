
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
	void add(itemSharedPtr it) { items.push_back(it); }
	itemVector items;
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
	bool equipItem(itemSharedPtr which);
	void unequipItem(itemSharedPtr which);
	bool addItem(itemSharedPtr which);

	//Getters
	itemlistVector getAllCarried() { return carried; }
	itemSharedPtr getEquipped(itemTypes category);
	itemVector getItemList(itemTypes category);
	consumableVector getConsumables() { return equippedConsumables; }

	//Getting particular items
	weaponSharedPtr getWeapon() { return equippedWeapon; }
	weaponSharedPtr getSecondaryWeapon() { return secondaryWeapon; }
	weaponSharedPtr getOffhand() { return equippedOffhand; }
	armourSharedPtr getArmour() { return equippedArmour; }
	armourSharedPtr getHelmet() { return equippedHelmet; }
	charmSharedPtr getCharm() { return equippedCharm; }

	//Weapon swapping
	weaponSharedPtr swapWeapon();

	//Spells, which are special
	bool equipSpell(spellSharedPtr sp);
	void unequipSpell(spellSharedPtr sp);
	bool isSpellEquipped(spellSharedPtr sp);

	//Consumables
	void equipConsumable(consumableSharedPtr c);
	void setSelectedConsumable(consumableSharedPtr c);
	bool isConsumableEquipped(consumableSharedPtr c);

	//Misc items
	void equipMiscItem(miscItemSharedPtr m);
	void removeMiscItem(miscItemSharedPtr m);

	//Keys
	bool hasKey(std::string keyTag);

	//Selecting particular slots
	void cycleConsumable();
	consumableSharedPtr getSelectedConsumable();

private:

	//Slots
	itemlistVector carried;
	
	//Pointers to each equipped item
	weaponSharedPtr equippedWeapon;
	weaponSharedPtr equippedOffhand;
	armourSharedPtr equippedArmour;
	armourSharedPtr equippedHelmet;
	charmSharedPtr equippedCharm;
	consumableSharedPtr selectedConsumable;

	//Secondary items for quick-swapping
	weaponSharedPtr secondaryWeapon;

	//Consumables appear as a list
	consumableVector equippedConsumables;
	//int selectedConsumable = 0;
};

#endif

