
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

	//Getters
	itemTypes getCategory() { return category; }
	virtual int getAmountLeft() { return 1; }

	//Stacking
	virtual const bool isStackable() { return false; }
	virtual const void add(int amount) {}
	virtual const void restoreToMax() {}

protected:

	//Sorting
	itemTypes category;

};

typedef std::vector<item*> itemVector;

const std::string getItemCategoryName(itemTypes cat);

#endif

