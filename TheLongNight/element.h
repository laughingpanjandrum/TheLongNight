
/*
This is the base class for pretty much anything that appears on the map:
characters, items, and so on.
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include "libtcod.hpp"
#include "tileConsts.h"

typedef std::pair<int, int> coord;

class element
{
public:
	element() : element("Floor", BASIC_FLOOR_TILE, TCODColor::grey) {}
	element(std::string name, int tileCode, TCODColor color): name(name), tileCode(tileCode), color(color) {}
	~element();
	//Setters
	void setPosition(int x, int y);
	void setPosition(coord pos);
	//Getters
	coord getPosition() { return xy; }
	int getx() { return xy.first; }
	int gety() { return xy.second; }
protected:
	//Flavour deets
	std::string name;
	int tileCode;
	TCODColor color;
	//Position
	coord xy;
};

#endif

