
#ifndef MISC_ITEM_H
#define MISC_ITEM_H

#include "item.h"

class miscItem : public item
{
public:

	//Constructors
	miscItem() : item() {}
	miscItem(std::string name, int tileCode, TCODColor color, std::string description) :
		item(name, tileCode, color, ITEM_MISC, description) {}
	~miscItem();

	//For key/lock items
	bool isKey;
	void setKeyTag(std::string t) { keyTag = t; }
	std::string getKeyTag() { return keyTag; }

private:

	//How this key is identified
	std::string keyTag;

};


/*
Items!
*/

miscItem* key_WretchsKey();


#endif