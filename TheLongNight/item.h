
#ifndef ITEM_H
#define ITEM_H

#include "element.h"
#include "categories.h"



class item: public element
{
public:

	//Constructors
	item(): element() {}
	item(std::string name, int tileCode, TCODColor color, std::string description = "") : 
		element(name, tileCode, color, description) {}
	item(std::string name, int tileCode, TCODColor color, itemTypes category, std::string description = "");
	~item();

	//Setters
	void setDefence(int def) { this->defence = def; }
	void setBleedResist(int r) { bleedResist = r; }
	void setDamageResist(damageType dtype, int def) { damageResist.at(dtype) = def; }

	//Getters
	itemTypes getCategory() { return category; }
	virtual int getAmountLeft() { return 1; }
	virtual const int isEquippable() { return true; }

	//Universal attributes: getting
	int getDefence() { return defence; }
	int getDamageResist(damageType dtype) { return damageResist.at(dtype); }
	int getBleedResist() { return bleedResist; }

	//Stacking
	virtual const bool isStackable() { return false; }
	virtual const void add(int amount) {}
	virtual const void restoreToMax() {}

	//Shopkeeping
	int getPrice() { return price; }
	void setPrice(int p) { price = p; }

protected:

	//Sorting
	itemTypes category;

	//Shopkeeping
	int price = 0;

	//Any item can have these attributes
	int defence = 0;
	int bleedResist = 0;
	std::vector<int> damageResist;

};

typedef std::vector<item*> itemVector;


/*
For shopkeepers.
Keeps track of a set of sellable items that are unlocked with a code.
*/
struct unlockableStock {
	unlockableStock(item* it, std::string unlockCode) :
		it(it), unlockCode(unlockCode) {}
	item* it;
	std::string unlockCode;
};

typedef std::vector<unlockableStock*> stockUnlockVector;

const std::string getItemCategoryName(itemTypes cat);

#endif

