
#ifndef ITEM_H
#define ITEM_H

#include "element.h"
#include "categories.h"

class item: public element
{
public:

	//Constructors
	item(): element() {}
	item(std::string name, int tileCode, TCODColor color): element(name,tileCode,color) {}
	item(std::string name, int tileCode, TCODColor color, itemTypes category);
	~item();

	//Getters
	itemTypes getCategory() { return category; }
	virtual int getAmountLeft() { return 1; }

	//Stacking
	virtual const bool isStackable() { return false; }
	virtual const void add(int amount) {}

protected:

	//Sorting
	itemTypes category;

};

const std::string getItemCategoryName(itemTypes cat);

#endif

