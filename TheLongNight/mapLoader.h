
/*
Creates maps defined by text files.
*/

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "map.h"
#include "monster.h"
#include "itemMaster.h"

class mapLoader
{

	//2d vector for storing map tiles
	typedef std::vector<std::string> stringVector;
	typedef std::vector<char> charVector;

public:

	mapLoader();
	~mapLoader() {}

	//Map generation
	std::string getStartMapHandle() { return FOLDER + START_MAP_NAME + FILE_EXTENSION; }
	map* loadMapFromFile(std::string filename);

private:

	//Utility functions
	tileVector* getTileList(stringVector tileHandles);
	maptile* getTileByChar(char c, charVector* tileChars, tileVector* tileList);

	//File locations
	const std::string FOLDER = "maps/"; //This is prepended to every map handle we load
	const std::string FILE_EXTENSION = ".txt"; //This is appended to every map handle we load
	const std::string START_MAP_NAME = "stardrift_wreckage_1";

};

#endif