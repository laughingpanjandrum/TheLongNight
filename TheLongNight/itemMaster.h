
/*
The sole purpose of this whole shenanigan is to keep track of all the different types of items.
Mostly used by the map loader - so that we have a consistent place to track item generation.
*/

#ifndef ITEM_MASTER_H
#define ITEM_MASTER_H

#include "weapon.h"
#include "armour.h"
#include "charm.h"
#include "spell.h"
#include "consumable.h"
#include "miscItem.h"




//Shop inventory storage
struct shop {
	//Basic constructors
	shop(std::string tag, bool eatsKeyWhenBought = false) :
		tag(tag), eatsKeyWhenBought(eatsKeyWhenBought) {}
	//Methods
	void addItem(itemSharedPtr it, int price) {
		it->isGlittery = false;
		it->setPrice(price);
		stock.push_back(it);
		//Additional attributes if this is a key-eating shop
		if (eatsKeyWhenBought) {
			it->setKeyEaten(tag);
		}
	}
	void removeItem(itemSharedPtr it) {
		auto iter = std::find(stock.begin(), stock.end(), it);
		if (iter != stock.end())
			stock.erase(iter);
	}
	//Data
	std::string tag; //How we're identified
	itemVector stock; //List of items sold
					  //If true, items in this inventory consume a key when bought
	bool eatsKeyWhenBought = false;
};


typedef std::shared_ptr<shop> shopSharedPtr;
typedef std::vector<shopSharedPtr> shopVector;


/*
Giant nightmare function that turns names into items.
Used by the map loader.
*/
itemSharedPtr getItemByHandle(std::string handle);

/*
This is for getting names and other ui data for damage types.
*/

std::string getDamageTypeName(damageType dtype);
TCODColor getDamageTypeColor(damageType dtype);

std::string getStatusEffectName(statusEffects stype);
TCODColor getStatusEffectColor(statusEffects stype);

std::string getEffectName(effect e);



/*
Monolithic list of all items in the game.
This data is required for save games, which refer to each item by its name.
*/
const itemVector getListOfAllItems();
itemSharedPtr getItemByName(std::string name);

#endif