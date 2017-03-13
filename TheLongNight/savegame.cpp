#include "savegame.h"



savegame::savegame()
{
}



/*
Constructor to generate a new savegame, hurrah.
*/
savegame::savegame(stringVector savedMapHandles, mapVector savedMaps, std::string currentMap, 
	coord currentPos, personSharedPtr player)
{

	//Copy over all saved map handles
	for (auto mh : savedMapHandles)
		knownMapTags.push_back(mh);

	//Save coords at each map point that still has an item on it
	for (auto m : savedMaps) {

		coordVector* itemCoords = new coordVector();

		//Now sort through all items
		for (auto it : m->getAllItems()) {

			//Remember this point
			itemCoords->push_back(it->getPosition());

		}

		//Retain this list
		keepItemsAtCoord.push_back(*itemCoords);

		//Also keep track of whether or not the boss of this map is dead
		if (m->bossDestroyed)
			bossDead.push_back(true);
		else
			bossDead.push_back(false);

	}

	//Save our current position
	lastMap = currentMap;
	lastPos = currentPos;

	//Complete list of names of items carried by the player
	for (auto cat : ALL_ITEM_TYPES) {
		auto itemList = player->getItemsOfType(cat);
		for (auto it : itemList) {
			//Item
			allCarriedItems.push_back(it->getName());
			//Quantity
			itemCount.push_back(it->getMaxAmount());
		}
	}

	//Complete list of names of items the player has equipped
	for (auto it : player->getAllEquippedItems()) {
		allEquippedItems.push_back(it->getName());
	}

}


/*
This one loads a save game from a given file.
*/
savegame::savegame(std::string fileToLoad)
{
	loadFromFile(fileToLoad);
}


savegame::~savegame()
{
}



/*
Saves all relevant datum to a file.
*/
void savegame::dumpToFile(std::string fname)
{

	//We put all the save info into a single line.
	std::string saveData;

	//First piece of information is two lines: current map tag and current coordinate.
	saveData += lastMap + ';';
	saveData += coordToString(lastPos) + ';';

	//Then the rest of the line is each of the saved maps and the coordinates that still have items.
	for (int i = 0; i < knownMapTags.size(); i++) {

		//First dump the name tag of this map
		saveData += knownMapTags.at(i) + ';';

		//Then list all the saved item coordinates
		for (auto crd : keepItemsAtCoord.at(i)) {
			saveData += coordToString(crd) + ';';
		}

		//Dump boss living/dead information
		if (bossDead.at(i))
			saveData += "BOSS_DEAD;";
		else
			saveData += "BOSS_ALIVE;";

		//Indication that we're done saving this map information
		saveData += "END;";

	}

	//Label indicating that ITEMS are up next
	saveData += "&ITEMS;";
	for (int i = 0; i < allCarriedItems.size(); i++) {
		std::string itName = allCarriedItems.at(i);
		//Item name
		saveData += itName + ';';
		//Quantity
		saveData += std::to_string(itemCount.at(i)) + ';';
	}

	//Label indicating that EQUIPPED ITEMS are up next
	saveData += "&EQUIPPED;";
	for (auto itName : allEquippedItems)
		saveData += itName + ';';

	//We have it all! Make a file and dump it in there
	std::ofstream saveFile(SAVE_FILE_LOCATION + fname);
	saveFile << saveData;
	saveFile.close();

}



/*
Generate all of our save data using a given file.
*/
void savegame::loadFromFile(std::string fname)
{

	//The whole file is one big ole' line!
	std::string saveData;
	std::ifstream saveFile(SAVE_FILE_LOCATION + fname);
	if (!saveFile.is_open())
		return;

	//And here is the line
	getline(saveFile, saveData);

	//Keep track of the stuff we've memorized so far
	bool gotCurrentLocation = false;
	bool gotCurrentPosition = false;
	bool readingInItems = false;
	bool nextChunkIsItemAmount = false;
	bool readingInEquipped = false;
	bool endOfMap = true;

	coordVector* savedItemVectors = new coordVector();

	//Each ; represents the end of a line
	std::string chunk = "";
	for (int i = 0; i < saveData.size(); i++) {

		if (saveData.at(i) != ';') {
			chunk += saveData.at(i);
		}
		else {

			//End of line! Now interpret this information

			if (chunk == "&ITEMS")
				readingInItems = true;
			else if (chunk == "&EQUIPPED") {
				readingInItems = false;
				readingInEquipped = true;
			}
			
			//Current map
			else if (!gotCurrentLocation) {
				lastMap = chunk;
				gotCurrentLocation = true;
			}

			//Current position
			else if (!gotCurrentPosition) {
				lastPos = stringToCoord(chunk);
				gotCurrentPosition = true;
			}

			//Loading equipped item names
			else if (readingInEquipped) {
				allEquippedItems.push_back(chunk);
			}

			//Loading quantity of the previous item we loaded
			else if (nextChunkIsItemAmount) {
				itemCount.push_back(std::stoi(chunk));
				nextChunkIsItemAmount = false;
			}

			//Loading item names
			else if (readingInItems) {
				allCarriedItems.push_back(chunk);
				nextChunkIsItemAmount = true;
			}

			//New map handle (cuz we're at the end of the last map)
			else if (endOfMap) {
				//This is a new map handle!
				knownMapTags.push_back(chunk);
				endOfMap = false;
				savedItemVectors = new coordVector();
			}

			//We're NOT at the end of the last map
			else {
				if (chunk == "END") {
					//We're at the end of a map tag
					keepItemsAtCoord.push_back(*savedItemVectors);
					endOfMap = true;
				}
				else if (chunk == "BOSS_DEAD")
					//The boss of this map has been killed, which is important to remember
					bossDead.push_back(true);
				else if (chunk == "BOSS_ALIVE")
					bossDead.push_back(false);
				else {
					//This is a coordinate
					savedItemVectors->push_back(stringToCoord(chunk));
				}
			}

			//Clear chunk
			chunk = "";

		}
	}

}

/*
Returns whether the item at the given coordinate should be kept.
*/
bool savegame::shouldSaveItem(int mapIdx, coord pt)
{
	for (auto keepPt : keepItemsAtCoord.at(mapIdx)) {
		if (keepPt == pt)
			return true;
	}
	return false;
}



/*
Returns true if the player should have the item with the given name.
*/
bool savegame::hasItemWithName(std::string itemName)
{
	for (auto it : allCarriedItems) {
		if (it == itemName)
			return true;
	}
	return false;
}


/*
Returns true if the player has this item equipped.
*/
bool savegame::hasItemEquipped(std::string itemName)
{
	for (auto it : allEquippedItems) {
		if (it == itemName)
			return true;
	}
	return false;
}


/*
Returns zero if we don't have the item at all.
*/
int savegame::getItemQuantity(std::string itemName)
{
	for (int i = 0; i < allCarriedItems.size(); i++) {
		if (allCarriedItems.at(i) == itemName) {
			return itemCount.at(i);
		}
	}
	return 0;
}



/*
	UTILITY FUNCTIONS
*/


/*
This converts a coordinate to a string representation.
*/
std::string savegame::coordToString(coord c)
{
	return std::to_string(c.first) + ',' + std::to_string(c.second);
}

/*
This converts a string representation to a coordinate.
*/
coord savegame::stringToCoord(std::string c)
{
	coord pos;
	std::string chunk = "";
	int i = 0;
	//First coordinate
	while (c.at(i) != ',')
		chunk += c.at(i++);
	pos.first = std::stoi(chunk);
	chunk = "";
	//Second coordinate
	i++;
	while (i < c.size())
		chunk += c.at(i++);
	pos.second = std::stoi(chunk);
	//Done
	return pos;
}
