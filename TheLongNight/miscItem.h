
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
	void setKeyTag(std::string t) { keyTag = t; isKey = true; category = ITEM_KEY; }
	std::string getKeyTag() { return keyTag; }

	//For runestones
	bool isRunestone = false;
	weaponRuneSharedPtr getRune() { return runeApplied; }
	void setRune(weaponRune* r) { runeApplied = weaponRuneSharedPtr(r); }

	//Information
	void setWhatWeDo(std::string wwd) { whatWeDo = wwd; }
	std::string getWhatWeDo() { return whatWeDo; }

private:

	//How this key is identified
	std::string keyTag;

	//Little chunk of text that tells you what the item is for
	std::string whatWeDo;

	//Runestone information
	weaponRuneSharedPtr runeApplied;

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
miscItemSharedPtr key_DeadSparrowKey();
miscItemSharedPtr key_MoshkasKey();
miscItemSharedPtr key_MoonpaleKey();
miscItemSharedPtr key_FarinsKey();
miscItemSharedPtr key_AtalundraRunekey();
miscItemSharedPtr key_PercivelsSign();

miscItemSharedPtr key_WatchfulEyestalk();
miscItemSharedPtr key_MawtoothFragment();
miscItemSharedPtr key_WitheredFinger();
miscItemSharedPtr key_BeatingHeart();
miscItemSharedPtr key_EffigyOfTheSlumberingLord();

miscItemSharedPtr key_AbattoirKey();
miscItemSharedPtr key_GreenKnightsKey();

miscItemSharedPtr key_MoltenPriestsKey();
miscItemSharedPtr key_AzarelsMark();

miscItemSharedPtr key_UnderpoolKey();
miscItemSharedPtr key_KlugsKey();

miscItemSharedPtr key_IetrasAmberPendant();

//Monster hearts
miscItemSharedPtr heart_WretchedHeart();
miscItemSharedPtr heart_OldCrowsHeart();
miscItemSharedPtr heart_FishBornGoddessesHeart();
miscItemSharedPtr heart_OrsylsShriveledHeart();
miscItemSharedPtr heart_VortensShriveledHeart();
miscItemSharedPtr heart_VenomousSpiderHeart();
miscItemSharedPtr heart_LadyTvertsHeart();
miscItemSharedPtr heart_DeadSparrowsHeart();
miscItemSharedPtr heart_HeartOfCoren();
miscItemSharedPtr heart_KhallesWretchedHeart();
miscItemSharedPtr heart_IetrasBurningHeart();
miscItemSharedPtr heart_PaleHeart();
miscItemSharedPtr heart_PlagueRiddenHeart();
miscItemSharedPtr heart_HeartOfFarin();
miscItemSharedPtr heart_RatKingsHeart();

miscItemSharedPtr heart_HeartOfTheOldGods();

//Spellbooks
miscItemSharedPtr spellbook_WaterloggedWritings();
miscItemSharedPtr spellbook_MinasProfanedWritings();
miscItemSharedPtr spellbook_FrostbittenWritings();
miscItemSharedPtr spellbook_SingedWritings();
miscItemSharedPtr spellbook_StartouchedWritings();
//Prayer books
miscItemSharedPtr spellbook_OrsylsTomeOfPrayer();
miscItemSharedPtr spellbook_DivineMoonsparkTome();
miscItemSharedPtr spellbook_DivineTomeOfTheEmissary();
miscItemSharedPtr spellbook_DivineNightmareTome();
miscItemSharedPtr spellbook_TomeOfTheDead();

//Runestones
miscItemSharedPtr runestone_CorensRunestone();
miscItemSharedPtr runestone_SiltrasRunestone();
miscItemSharedPtr runestone_KhallesRunestone();
miscItemSharedPtr runestone_IetrasRunestone();
miscItemSharedPtr runestone_CharredRunestone();
miscItemSharedPtr runestone_KinslayersRunestone();
miscItemSharedPtr runestone_StarweaversRunestone();
miscItemSharedPtr runestone_ThundrousRunestone();
miscItemSharedPtr runestone_BloodDrenchedRunestone();
miscItemSharedPtr runestone_CorrodingRunestone();
miscItemSharedPtr runestone_FrozenRunestone();
miscItemSharedPtr runestone_VenomousRunestone();

//Bone keys
miscItemSharedPtr bones_KhallesBones();
miscItemSharedPtr bones_SiltrasBones();
miscItemSharedPtr bones_CorensBones();
miscItemSharedPtr bones_IetrasBones();

//Other
miscItemSharedPtr misc_VoidSigil();
miscItemSharedPtr misc_PieceOfJade();
miscItemSharedPtr misc_PieceOfBone();


#endif