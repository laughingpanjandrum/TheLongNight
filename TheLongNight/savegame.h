


#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <iostream>
#include <fstream>

#include "itemMaster.h"
#include "element.h"
#include "map.h"

typedef std::vector<std::string> stringVector;

struct shopData {
	shopData() {}
	shopData(std::string tag, bool eatsKeyWhenBought) :
		tag(tag), eatsKeyWhenBought(eatsKeyWhenBought) {}
	std::string tag;
	stringVector itemTags;
	std::vector<int> itemPrices;
	bool eatsKeyWhenBought;
	void addItem(itemSharedPtr it) { itemTags.push_back(it->getName()); itemPrices.push_back(it->getPrice()); }
};



class savegame
{
	typedef std::vector<int> itemVector;
	typedef std::vector<shopData> shopDataVector;
public:
	
	savegame();
	savegame(stringVector savedMapHandles, mapVector savedMaps, 
		std::string currentMap, coord currentPos, personSharedPtr player,
		stringVector storyFlags, int fragments, 
		shopVector currentShops, shopVector unlockableShops);
	savegame(std::string fileToLoad);
	~savegame();

	//File reading and writing
	void dumpToFile(std::string fname);
	void loadFromFile(std::string fname);

	//Getting data to transform into a real game
	stringVector getAllMapTags() { return knownMapTags; }
	std::vector<coordVector> getItemsToKeep() { return keepItemsAtCoord; }
	std::string getStartMapTag() { return lastMap; }
	coord getStartPosition() { return lastPos; }
	bool isBossDead(int mapIdx) { return bossDead.at(mapIdx); }
	bool shouldSaveItem(int mapIdx, coord pt);
	bool hasItemWithName(std::string itemName);
	bool hasItemEquipped(std::string itemName);
	int getItemQuantity(std::string itemName);
	stringVector getStoryFlags() { return storyFlags; }
	statline* getPlayerStats() { return &playerStats; }
	int getFragmentsHeld() { return fragmentsHeld; }
	shopDataVector getCurrentShops() { return currentShops; }
	shopDataVector getUnlockableShops() { return unlockableShops; }

private:

	//List of all map tags we should remember
	stringVector knownMapTags;

	//We only retain items at the given coordinates; all other items have been picked up.
	std::vector<coordVector> keepItemsAtCoord;

	//Keeps track of which bosses are dead
	std::vector<bool> bossDead;

	//This is where we were when the game was saved.
	std::string lastMap;
	coord lastPos;

	//Items the player is carrying.
	stringVector allCarriedItems;
	stringVector allEquippedItems;
	itemVector itemCount;

	//Story flags triggered
	stringVector storyFlags;

	//Player statline
	statline playerStats;
	int fragmentsHeld;

	//Shop state
	shopDataVector currentShops;
	shopDataVector unlockableShops;

	//Utility functions
	std::string coordToString(coord c);
	coord stringToCoord(std::string c);
	std::string createShopData(shopDataVector shopList);

};

const std::string SAVE_FILE_LOCATION = "saves/";


#endif
