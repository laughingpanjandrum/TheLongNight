
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

	//Override
	virtual const int isEquippable() { return false; }

	//For key/lock items
	bool isKey = true;
	void setKeyTag(std::string t) { keyTag = t; }
	std::string getKeyTag() { return keyTag; }

	//For runestones
	bool isRunestone = false;
	weaponRune* getRune() { return runeApplied; }
	void setRune(weaponRune* r) { runeApplied = r; }

private:

	//How this key is identified
	std::string keyTag;

	//Runestone information
	weaponRune* runeApplied;

};


/*
Items!
*/

//Keys
miscItem* key_WretchsKey();
miscItem* key_OldCrowsKey();
miscItem* key_RuinedTownshipKey();

//Spellbooks
miscItem* spellbook_WaterloggedWritings();

//Runestones
miscItem* runestone_CorensRunestone();

//Other
miscItem* misc_VoidSigil();


#endif