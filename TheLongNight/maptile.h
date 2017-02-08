
#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>
#include <vector>
#include "libtcod.hpp"
#include "tileConsts.h"
#include "categories.h"
#include "utility.h"

//	Colour constants


#define LIGHT_GRASS_COLOR TCODColor(76, 76, 35)
#define DARK_GRASS_COLOR TCODColor(68, 77, 21)
#define DARKEST_GRASS_COLOR TCODColor(60, 71, 11)

#define LIGHT_WATER_COLOR TCODColor(25, 65, 148)
#define DARK_WATER_COLOR TCODColor(22, 58, 135)
#define DARKEST_WATER_COLOR TCODColor(4, 12, 28)

#define LIGHT_STONE_COLOR TCODColor(134, 129, 186)
#define DARK_STONE_COLOR TCODColor(23, 23, 23)

#define TREE_COLOR TCODColor(122, 92, 44)

#define LIGHT_SAND_COLOR TCODColor(242, 234, 189)
#define DARK_SAND_COLOR TCODColor(222, 210, 161)
#define DARKEST_SAND_COLOR TCODColor(202, 192, 149)

#define LIGHT_WOOD_COLOR TCODColor(87, 73, 34)
#define DARK_WOOD_COLOR TCODColor(38, 29, 10)

#define LIGHT_GLASS_COLOR TCODColor(209, 227, 226)
#define DARK_GLASS_COLOR TCODColor(134, 165, 189)



class maptile
{
public:

	//Constructors/destructor
	maptile() : maptile("Floor", "undefined", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::black) {}
	maptile(std::string name, std::string handle, int tileCode, TCODColor color, TCODColor bgcolor, 
		bool walkable = true, bool seeThrough = true, bool glitters = false): 
		name(name), handle(handle), color(color), bgcolor(bgcolor), tileCode(tileCode), 
		walkable(walkable), seeThrough(seeThrough), isGlittery(glitters) {}
	~maptile();

	//Getters
	std::string getName() { return name; }
	std::string getHandle() { return handle; }
	int getTileCode() { return tileCode; }
	TCODColor getColor();
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

	//Colour effects
	bool isGlittery = false;
	float glitterCoef = 0.0;
	bool ascendingGlitter = true;

	//Special flags
	bool isWater = false;

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




//	Tiles



maptile* tile_Floor();	//"floor_stone"
maptile* tile_MossyFloor(); //"floor_mossy_stone"
maptile* tile_Sand(); //"floor_sand"
maptile* tile_Grass(); //"floor_grass"
maptile* tile_Road(); //"floor_road"
maptile* tile_Void(); //"floor_void"
maptile* tile_Carpet(); //"floor_carpet"
maptile* tile_SnowFloor(); //"floor_snow"

maptile* tile_SandyRock(); //"wall_sand"
maptile* tile_Wall(); //"wall_stone"
maptile* tile_WoodenWall(); //"wall_wood";
maptile* tile_MarbleWall(); //"wall_marble"

maptile* tile_Door(); //"door_stone"
maptile* tile_WoodenDoor(); //"door_wood"
maptile* tile_LockedDoor(); //"locked_door"
maptile* tile_DoorUnlocker(); //"door_unlocker"

maptile* tile_Glass(); //"glass"
maptile* tile_Bush(); //"bush"
maptile* tile_Rosebush(); //"rosebush"
maptile* tile_Tree(); //"tree"
maptile* tile_VoidTree(); //"void_tree"
maptile* tile_DeadTree(); //"dead_tree"

maptile* tile_Water(); //"water"
maptile* tile_MossyWater(); //"water_mossy"
maptile* tile_DeepWater(); //"water_deep"
maptile* tile_Acid(); //"acid"

maptile* tile_Ooze(); //"ooze"
maptile* tile_Web(); //"web"
maptile* tile_WebOnFloor(); //"web_floor"
maptile* tile_Bloodstain(); //"bloodstain"
maptile* tile_Corpse(); //"corpse"

maptile* tile_Bed(); //"bed"
maptile* tile_Terminal(); //"terminal"

maptile* tile_StatueOfRest(); //"statue"
maptile* tile_VoidWarpstone(); //"warpstone"

//Specific locked doors
maptile* tile_WretchedDoor(); //"wretched_door"
maptile* tile_CrowDoor(); //"crow_door"
maptile* tile_RuinedTownshipHallDoor(); //"township_door"
maptile* tile_WyrdDoor(); //"wyrd_door"
maptile* tile_GreenChapelGardenDoor(); //"green_chapel_door"
maptile* tile_SordidChapelDoor(); //"sordid door"
maptile* tile_VoidDrenchedDoor(); //"void_drenched_door"
maptile* tile_DoorOfWinter(); //"door_of_winter"
maptile* tile_OldValleyDoor(); //"old_valley_door"
maptile* tile_DesolateChurchDoor(); //"bartons_door"
maptile* tile_HightowerDoor(); //"hightower_door"

//Furniture
maptile* tile_Chair1(); //"chair1"
maptile* tile_Chair2(); //"chair2"
maptile* tile_Table1(); //"table1"
maptile* tile_Table2(); //"table2"
maptile* tile_Table3(); //"table3"
maptile* tile_Table4(); //"table4"
maptile* tile_Tombstone(); //"tombstone"


const tileVector ALL_MAPTILES = {
	tile_Floor(), tile_MossyFloor(), tile_Sand(), tile_Grass(), tile_Road(), tile_Void(), tile_Carpet(), tile_SnowFloor(),
	tile_Wall(), tile_WoodenWall(), tile_SandyRock(), tile_MarbleWall(),
	tile_Door(), tile_WoodenDoor(), tile_LockedDoor(), tile_DoorUnlocker(),
	tile_Glass(), tile_Bush(), tile_Rosebush(), tile_Tree(), tile_VoidTree(), tile_DeadTree(),
	tile_Water(), tile_MossyWater(), tile_DeepWater(), tile_Acid(),
	tile_Ooze(), tile_Web(), tile_WebOnFloor(),
	tile_Bloodstain(), tile_Corpse(),
	tile_Bed(), tile_Terminal(),
	tile_StatueOfRest(), tile_VoidWarpstone(),
	//Furniture
	tile_Chair1(), tile_Chair2(), tile_Tombstone(),
	tile_Table1(), tile_Table2(), tile_Table3(), tile_Table4(),
	//Special locked doors
	tile_WretchedDoor(), tile_CrowDoor(), tile_RuinedTownshipHallDoor(),
	tile_WyrdDoor(), tile_GreenChapelGardenDoor(), tile_SordidChapelDoor(),
	tile_VoidDrenchedDoor(), tile_DoorOfWinter(), tile_OldValleyDoor(),
	tile_DesolateChurchDoor(), tile_HightowerDoor()
};

#endif
