#include "maptile.h"



maptile::~maptile()
{
}

TCODColor maptile::getColor()
{
	if (isGlittery) {
		//We create a colour close to, but not exactly like, our colour
		if (ascendingGlitter) {
			glitterCoef += 0.05;
			if (glitterCoef >= 0.8)
				ascendingGlitter = false;
		}
		else {
			glitterCoef -= 0.05;
			if (glitterCoef <= 0.1)
				ascendingGlitter = true;
		}
		return TCODColor::lerp(color, TCODColor::white, glitterCoef);
	}
	return color;
}

/*
Returns whether we have the given touch effect.
*/
bool maptile::hasTouchEffect(effect te)
{
	return std::find(touchEffects.begin(), touchEffects.end(), te) != touchEffects.end();
}

maptile * tile_Floor()
{
	return new maptile("Floor", "floor_stone", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::darkerGrey);
}

maptile * tile_MossyFloor()
{
	return new maptile("Floor", "floor_mossy_stone", MOSS_TILE, TCODColor::darkGreen, TCODColor::darkerGrey);
}

maptile * tile_Sand()
{
	return new maptile("Sand", "floor_sand", BASIC_FLOOR_TILE, TCODColor::amber, TCODColor::darkerAmber);
}

maptile * tile_Grass()
{
	return new maptile("Grass", "floor_grass", BASIC_FLOOR_TILE, TCODColor::green, TCODColor::darkerGreen);
}

maptile * tile_Road()
{
	return new maptile("Road", "floor_road", BASIC_FLOOR_TILE, TCODColor::sepia, TCODColor::darkerSepia);
}

maptile * tile_SandyRock()
{
	return new maptile("Sandy Rock", "wall_sand", BASIC_WALL_TILE, TCODColor::darkAmber, TCODColor::amber, false, false);
}

maptile * tile_Wall()
{
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, TCODColor::darkGrey, TCODColor::darkestGrey, false, false);
}

maptile * tile_WoodenWall()
{
	return new maptile("Wall", "wall_wood", BASIC_WALL_TILE, TCODColor::lightSepia, TCODColor::darkerSepia, false, false);
}

maptile * tile_Door()
{
	maptile* d = new maptile("Door", "door_stone", BASIC_DOOR_TILE, TCODColor::darkerGrey, TCODColor::grey, true, false);
	return d;
}

maptile * tile_WoodenDoor()
{
	return new maptile("Door", "door_wood", BASIC_DOOR_TILE, TCODColor::darkerSepia, TCODColor::lightSepia, true, false);
}

/*
Door is initially locked (but can be unlocked later)
*/
maptile * tile_LockedDoor()
{
	maptile* d = new maptile("Door", "locked_door", BASIC_DOOR_TILE, TCODColor::darkerGrey, TCODColor::grey, false, false);
	d->isDoor = true;
	return d;
}

/*
Unlocks all adjacent doors when stepped on
*/
maptile * tile_DoorUnlocker()
{
	maptile* u = new maptile("Floor", "door_unlocker", BASIC_FLOOR_TILE, TCODColor::grey, TCODColor::darkerGrey);
	u->addTouchEffect(UNLOCK_ADJACENT_DOORS);
	return u;
}

maptile * tile_Glass()
{
	return new maptile("Glass", "glass", BASIC_WALL_TILE, TCODColor::cyan, TCODColor::black, false, true);
}

maptile * tile_Bush()
{
	return new maptile("Bush", "bush", BUSH_TILE, TCODColor::green, TCODColor::darkerGreen, true, false);
}

maptile * tile_Tree()
{
	return new maptile("Tree", "tree", TREE_TILE, TCODColor::lightSepia, TCODColor::darkerGreen, false, false);
}

maptile * tile_Water()
{
	return new maptile("Water", "water", BASIC_WATER_TILE, TCODColor::cyan, TCODColor::blue);
}

maptile * tile_MossyWater()
{
	return new maptile("Water", "water_mossy", MOSS_TILE, TCODColor::green, TCODColor::blue);
}

maptile * tile_DeepWater()
{
	return new maptile("Deep Water", "water_deep", BASIC_WATER_TILE, TCODColor::cyan, TCODColor::darkBlue, false, true);
}

maptile * tile_Acid()
{
	maptile* a = new maptile("Acid", "acid", BASIC_WATER_TILE, TCODColor::lighterGreen, TCODColor::green);
	a->addTouchEffect(APPLY_PHYSICAL_DAMAGE);
	a->setPotency(5);
	return a;
}

maptile * tile_Bed()
{
	return new maptile("Bed", "bed", BED_TILE, TCODColor::darkRed, TCODColor::darkerGrey);
}

maptile * tile_Terminal()
{
	return new maptile("Terminal", "terminal", TERMINAL_TILE, TCODColor::darkGreen, TCODColor::darkerGrey);
}

/*
Resting tile. Serves as a save point and provides a full restore.
*/
maptile * tile_StatueOfRest()
{
	maptile* statue = new maptile("Statue of Rest", "statue", STATUE_TILE, TCODColor::cyan, TCODColor::darkestCyan, 
		true, true, true);
	statue->addTouchEffect(ALLOW_INVENTORY_MANAGEMENT);
	statue->addTouchEffect(FULL_RESTORE);
	statue->addTouchEffect(SET_SAVE_POINT);
	return statue;
}

/*
Lets you teleport between maps.
*/
maptile * tile_VoidWarpstone()
{
	maptile* warpstone = new maptile("Void Warpstone", "warpstone", WARPSTONE_TILE, TCODColor::lightPurple, TCODColor::darkestPurple,
		true, true, true);
	warpstone->addTouchEffect(DO_WARP);
	return warpstone;
}


/*
	Specific locked doors
*/

maptile * tile_WretchedDoor()
{
	maptile* door = new maptile("Wretched Door", "wretched_door", BASIC_DOOR_TILE, TCODColor::lightAmber, TCODColor::darkAmber, 
		false, false);
	door->unlockCode = "wretched_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	door->isGlittery = true;
	return door;
}

maptile * tile_CrowDoor()
{
	maptile* door = new maptile("Old Crow Door", "crow_door", BASIC_DOOR_TILE, TCODColor::grey, TCODColor::darkPurple,
		false, false);
	door->unlockCode = "crow_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	door->isGlittery = true;
	return door;
}

maptile * tile_RuinedTownshipHallDoor()
{
	maptile* door = new maptile("Hall Door", "township_door", BASIC_DOOR_TILE, TCODColor::grey, TCODColor::darkGrey,
		false, false);
	door->unlockCode = "ruined_township_hall_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	door->isGlittery = true;
	return door;
}
