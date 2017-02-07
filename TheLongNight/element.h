
/*
This is the base class for pretty much anything that appears on the map:
characters, items, and so on.
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <memory>

#include "libtcod.hpp"
#include "tileConsts.h"
#include "utility.h"

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
	TCODColor getColor();

	//Text description of element; purely for ui
	const std::string description;
	
	//For overriding
	virtual std::string getMenuName() { return name; }

	//If true, this changes how our colour is displayed
	bool isGlittery = false;
	float glitterCoef = 0.0;
	bool ascendingGlitter = true;

protected:

	//Flavour deets
	std::string name;
	int tileCode;
	TCODColor color;

	//Position
	coord xy;

};


typedef std::shared_ptr<element> elementSharedPtr;
typedef std::vector<elementSharedPtr> elementVector;

#endif

