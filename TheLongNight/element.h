
/*
This is the base class for pretty much anything that appears on the map:
characters, items, and so on.
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include "libtcod.hpp"
#include "tileConsts.h"

typedef std::pair<int, int> coord;
typedef std::vector<coord> coordVector;

class element
{
public:
	
	element() : element("Floor", BASIC_FLOOR_TILE, TCODColor::grey) {}
	element(std::string name, int tileCode, TCODColor color, std::string description = "") : 
		name(name), tileCode(tileCode), color(color), description(description) {}
	~element();
	
	//Setters
	void setPosition(int x, int y);
	void setPosition(coord pos);
	
	//Getters: position
	coord getPosition() { return xy; }
	int getx() { return xy.first; }
	int gety() { return xy.second; }
	bool isAtPosition(int x, int y);
	
	//Getters: aesthetics
	std::string getName() { return name; }
	int getTileCode() { return tileCode; }
	TCODColor getColor() { return color; }

	//Text description of element; purely for ui
	const std::string description;
	
	//For overriding
	virtual std::string getMenuName() { return name; }

protected:

	//Flavour deets
	std::string name;
	int tileCode;
	TCODColor color;

	//Position
	coord xy;

};

typedef std::vector<element*> elementVector;

#endif

