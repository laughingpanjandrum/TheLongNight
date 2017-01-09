
#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>
#include "libtcod.hpp"

class maptile
{
public:
	//Constructors/destructor
	maptile() : maptile("Untitled", TCODColor::white, TCODColor::black, '?') {}
	maptile(std::string name, TCODColor color, TCODColor bgcolor, char tile): 
		name(name), color(color), bgcolor(bgcolor), tile(tile) {}
	~maptile();
	//Aesthetic data
	std::string name;
	TCODColor color;
	TCODColor bgcolor;
	char tile;
};

#endif
