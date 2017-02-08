
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
	void setKeyTag(std::string t) { keyTag = t; isKey = true; }
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


typedef std::shared_ptr<miscItem> miscItemSharedPtr;

/*
Items!
*/

//Keys
miscItemSharedPtr key_WretchsKey();
miscItemSharedPtr key_OldCrowsKey();
miscItemSharedPtr key_RuinedTownshipKey();
miscItemSharedPtr key_WyrdKey();
miscItemSharedPtr key_GreenChapelGardenKey();
miscItemSharedPtr key_SordidChapelKey();
miscItemSharedPtr key_SpinalColumnShard();
miscItemSharedPtr key_OldValleyKey();
miscItemSharedPtr key_BartonsKey();
miscItemSharedPtr key_HightowerKey();
miscItemSharedPtr key_LadyTvertsKey();

miscItemSharedPtr key_WatchfulEyestalk();

//Monster hearts
miscItemSharedPtr heart_WretchedHeart();
miscItemSharedPtr heart_OldCrowsHeart();
miscItemSharedPtr heart_FishBornGoddessesHeart();
miscItemSharedPtr heart_OrsylsShriveledHeart();
miscItemSharedPtr heart_VortensShriveledHeart();
miscItemSharedPtr heart_VenomousSpiderHeart();
miscItemSharedPtr heart_LadyTvertsHeart();

//Spellbooks
miscItemSharedPtr spellbook_WaterloggedWritings();
miscItemSharedPtr spellbook_MinasProfanedWritings();
miscItemSharedPtr spellbook_FrostbittenWritings();
miscItemSharedPtr spellbook_OrsylsTomeOfPrayer();
miscItemSharedPtr spellbook_DivineMoonsparkTome();
miscItemSharedPtr spellbook_DivineTomeOfTheEmissary();

//Runestones
miscItemSharedPtr runestone_CorensRunestone();
miscItemSharedPtr runestone_SiltrasRunestone();
miscItemSharedPtr runestone_KhallesRunestone();
miscItemSharedPtr runestone_IetrasRunestone();
miscItemSharedPtr runestone_CharredRunestone();

//Bone keys
miscItemSharedPtr bones_KhallesBones();
miscItemSharedPtr bones_SiltrasBones();

//Other
miscItemSharedPtr misc_VoidSigil();
miscItemSharedPtr misc_PieceOfJade();


#endif