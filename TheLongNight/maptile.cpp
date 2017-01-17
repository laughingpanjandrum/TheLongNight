#include "maptile.h"



maptile::~maptile()
{
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
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, TCODColor::darkGrey, TCODColor::black, false, false);
}

maptile * tile_Door()
{
	maptile* d = new maptile("Door", "door_stone", BASIC_DOOR_TILE, TCODColor::darkerGrey, TCODColor::grey, true, false);
	return d;
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
	return new maptile("Tree", "tree", TREE_TILE, TCODColor::sepia, TCODColor::darkerGreen, false, true);
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
	return new maptile("Terminal", "terminal", TERMINAL_TILE, TCODColor::green, TCODColor::darkerGrey);
}

/*
Resting tile. Serves as a save point and provides a full restore.
*/
maptile * tile_StatueOfRest()
{
	maptile* statue = new maptile("Statue of Rest", "statue", STATUE_TILE, TCODColor::lightCyan, TCODColor::darkestCyan);
	statue->addTouchEffect(ALLOW_INVENTORY_MANAGEMENT);
	statue->addTouchEffect(FULL_RESTORE);
	statue->addTouchEffect(SET_SAVE_POINT);
	return statue;
}


/*
	Specific locked doors
*/

maptile * tile_WretchedDoor()
{
	maptile* door = new maptile("Wretched Door", "wretched_door", BASIC_DOOR_TILE, TCODColor::lightAmber, TCODColor::darkAmber, false, false);
	door->unlockCode = "wretched_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}
