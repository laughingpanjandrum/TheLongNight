
#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>
#include <vector>
#include "libtcod.hpp"
#include "tileConsts.h"

//This tracks the sort of stuff that tiles can do when you touch them!
enum touchEffect {
	ACID_TOUCH
};

typedef std::vector<touchEffect> touchEffectVector;

class maptile
{
public:
	//Constructors/destructor
	maptile() : maptile("Floor", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::black) {}
	maptile(std::string name, int tileCode, TCODColor color, TCODColor bgcolor, bool walkable = true, bool seeThrough = true): 
		name(name), color(color), bgcolor(bgcolor), tileCode(tileCode), walkable(walkable), seeThrough(seeThrough) {}
	~maptile();
	//Getters
	std::string getName() { return name; }
	int getTileCode() { return tileCode; }
	TCODColor getColor() { return color; }
	TCODColor getBgColor() { return bgcolor; }
	bool isWalkable() { return walkable; }
	bool isSeeThrough() { return seeThrough; }
	//Touch effects
	void addTouchEffect(touchEffect te) { touchEffects.push_back(te); }
	touchEffectVector getTouchEffects() { return touchEffects; }
	bool hasTouchEffect(touchEffect te);
private:
	//Aesthetic data
	std::string name;
	TCODColor color;
	TCODColor bgcolor;
	int tileCode;
	//Movement adjustment
	bool walkable;
	bool seeThrough;
	//What can happen when we touch this guy
	touchEffectVector touchEffects;
};

//Constants
maptile* floor();
maptile* wall();
maptile* acid();

#endif
