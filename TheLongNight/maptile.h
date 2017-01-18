
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
	maptile() : maptile("Floor", "undefined", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::black) {}
	maptile(std::string name, std::string handle, int tileCode, TCODColor color, TCODColor bgcolor, bool walkable = true, bool seeThrough = true): 
		name(name), handle(handle), color(color), bgcolor(bgcolor), tileCode(tileCode), walkable(walkable), seeThrough(seeThrough) {}
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

	//Locking and unlocking
	bool isDoor = false;
	void unlockDoor() { walkable = true; }
	bool isDoorLocked() { return isDoor && !walkable; }
	std::string unlockCode;

private:

	//Uniquely defines a tile using the map loader
	std::string handle;

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

//	Constants

maptile* tile_Floor();	//"floor_stone"
maptile* tile_MossyFloor(); //"floor_mossy_stone"
maptile* tile_Sand(); //"floor_sand"
maptile* tile_Grass(); //"floor_grass"
maptile* tile_Road(); //"floor_road"

maptile* tile_SandyRock(); //"wall_sand"
maptile* tile_Wall(); //"wall_stone"
maptile* tile_WoodenWall(); //"wall_wood";

maptile* tile_Door(); //"door_stone"
maptile* tile_WoodenDoor(); //"door_wood"
maptile* tile_LockedDoor(); //"locked_door"
maptile* tile_DoorUnlocker(); //"door_unlocker"

maptile* tile_Glass(); //"glass"
maptile* tile_Bush(); //"bush"
maptile* tile_Tree(); //"tree"

maptile* tile_Water(); //"water"
maptile* tile_MossyWater(); //"water_mossy"
maptile* tile_DeepWater(); //"water_deep"
maptile* tile_Acid(); //"acid"

maptile* tile_Bed(); //"bed"
maptile* tile_Terminal(); //"terminal"

maptile* tile_StatueOfRest(); //"statue"

//Specific locked doors
maptile* tile_WretchedDoor(); //"wretched_door"

const tileVector ALL_MAPTILES = {	
	tile_Floor(), tile_MossyFloor(), tile_Sand(), tile_Grass(), tile_Road(),
	tile_Wall(), tile_WoodenWall(), tile_SandyRock(),
	tile_Door(), tile_WoodenDoor(), tile_LockedDoor(), tile_DoorUnlocker(),
	tile_Glass(), tile_Bush(), tile_Tree(),
	tile_Water(), tile_MossyWater(), tile_DeepWater(), tile_Acid(),
	tile_Bed(), tile_Terminal(),
	tile_StatueOfRest(),
	//Special locked doors
	tile_WretchedDoor()
};

#endif
