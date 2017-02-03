
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
miscItem* key_WyrdKey();
miscItem* key_GreenChapelGardenKey();
miscItem* key_SordidChapelKey();
miscItem* key_SpinalColumnShard();
miscItem* key_WatchfulEyestalk();

//Monster hearts
miscItem* heart_WretchedHeart();
miscItem* heart_OldCrowsHeart();
miscItem* heart_FishBornGoddessesHeart();
miscItem* heart_OrsylsShriveledHeart();
miscItem* heart_VortensShriveledHeart();

//Spellbooks
miscItem* spellbook_WaterloggedWritings();
miscItem* spellbook_MinasProfanedWritings();
miscItem* spellbook_FrostbittenWritings();
miscItem* spellbook_OrsylsTomeOfPrayer();
miscItem* spellbook_DivineMoonsparkTome();

//Runestones
miscItem* runestone_CorensRunestone();
miscItem* runestone_SiltrasRunestone();
miscItem* runestone_KhallesRunestone();
miscItem* runestone_IetrasRunestone();

//Other
miscItem* misc_VoidSigil();
miscItem* misc_PieceOfJade();


#endif