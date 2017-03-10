


#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <iostream>
#include <fstream>

#include "element.h"
#include "map.h"

class savegame
{
	typedef std::vector<std::string> stringVector;
public:
	
	savegame();
	savegame(stringVector savedMapHandles, mapVector savedMaps, std::string currentMap, coord currentPos);
	savegame(std::string fileToLoad);
	~savegame();

	void dumpToFile(std::string fname);
	void loadFromFile(std::string fname);

	//Getting data to transform into a real game
	stringVector getAllMapTags() { return knownMapTags; }
	std::vector<coordVector> getItemsToKeep() { return keepItemsAtCoord; }
	std::string getStartMapTag() { return lastMap; }
	coord getStartPosition() { return lastPos; }
	bool shouldSaveItem(int mapIdx, coord pt);

private:

	//List of all map tags we should remember
	stringVector knownMapTags;

	//We only retain items at the given coordinates; all other items have been picked up.
	std::vector<coordVector> keepItemsAtCoord;

	//This is where we were when the game was saved.
	std::string lastMap;
	coord lastPos;

	//Utility functions
	std::string coordToString(coord c);
	coord stringToCoord(std::string c);

};

const std::string SAVE_FILE_LOCATION = "saves/";


#endif
