
/*
Creates maps defined by text files.
*/

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include "map.h"

class mapLoader
{
public:

	mapLoader();
	~mapLoader();

private:

	//File locations
	const std::string folder = "maps/";
	const std::string startMap = "stardrift_wreckage_1";

};

#endif