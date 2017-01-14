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

maptile * tile_Glass()
{
	return new maptile("Glass", "glass", BASIC_WALL_TILE, TCODColor::cyan, TCODColor::black, false, true);
}

maptile * tile_Water()
{
	return new maptile("Water", "water", BASIC_WATER_TILE, TCODColor::cyan, TCODColor::blue);
}

maptile * tile_MossyWater()
{
	return new maptile("Water", "water_mossy", MOSS_TILE, TCODColor::green, TCODColor::blue);
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
Resting tile.
*/
maptile * tile_StatueOfRest()
{
	maptile* statue = new maptile("Statue of Rest", "statue", STATUE_TILE, TCODColor::lightCyan, TCODColor::darkestCyan);
	statue->addTouchEffect(ALLOW_INVENTORY_MANAGEMENT);
	statue->addTouchEffect(FULL_RESTORE);
	statue->addTouchEffect(SET_SAVE_POINT);
	return statue;
}
