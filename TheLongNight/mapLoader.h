
/*
Creates maps defined by text files.
*/

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "map.h"

class mapLoader
{

	//2d vector for storing map tiles
	typedef std::vector<std::string> stringVector;

public:

	mapLoader();
	~mapLoader();

	//Map generation
	map* loadMapFromFile(std::string filename);

private:

	//Utility functions
	tileVector getTileList(stringVector tileHandles);
	maptile* getTileByChar(char c, std::vector<char> tileChars, tileVector tileList);

	//File locations
	const std::string folder = "maps/";
	const std::string startMap = "stardrift_wreckage_1";

};

#endif