
#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>
#include "libtcod.hpp"
#include "tileConsts.h"

class maptile
{
public:
	//Constructors/destructor
	maptile() : maptile("Floor", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::black) {}
	maptile(std::string name, int tileCode, TCODColor color, TCODColor bgcolor): 
		name(name), color(color), bgcolor(bgcolor), tileCode(tileCode) {}
	~maptile();
	//Aesthetic data
	std::string name;
	TCODColor color;
	TCODColor bgcolor;
	int tileCode;
};

#endif
