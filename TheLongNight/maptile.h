
#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>
#include <vector>
#include "libtcod.hpp"
#include "tileConsts.h"
#include "categories.h"

class maptile
{
public:

	//Constructors/destructor
	maptile() : maptile("Floor", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::black, "undefined") {}
	maptile(std::string name, int tileCode, TCODColor color, TCODColor bgcolor, std::string handle, bool walkable = true, bool seeThrough = true): 
		name(name), color(color), bgcolor(bgcolor), tileCode(tileCode), walkable(walkable), seeThrough(seeThrough), handle(handle) {}
	~maptile();

	//Getters
	std::string getName() { return name; }
	std::string getHandle() { return handle; }
	int getTileCode() { return tileCode; }
	TCODColor getColor() { return color; }
	TCODColor getBgColor() { return bgcolor; }
	bool isWalkable() { return walkable; }
	bool isSeeThrough() { return seeThrough; }

	//Touch effects
	void addTouchEffect(effect te) { touchEffects.push_back(te); }
	void setPotency(int p) { potency = p; }
	effectVector getTouchEffects() { return touchEffects; }
	int getPotency() { return potency; }
	bool hasTouchEffect(effect te);

private:

	//Uniquely defines a tile using the map loader
	const std::string handle;

	//Aesthetic data
	std::string name;
	TCODColor color;
	TCODColor bgcolor;
	int tileCode;

	//Movement adjustment
	bool walkable;
	bool seeThrough;

	//What can happen when we touch this guy
	effectVector touchEffects;
	int potency = 1; //Effectiveness of touch effects

};

typedef std::vector<maptile*> tileVector;

//Constants
maptile* floor();
maptile* wall();
maptile* acid();

maptile* tile_StatueOfRest();

const tileVector ALL_MAPTILES = { floor(), wall(), tile_StatueOfRest() };

#endif
