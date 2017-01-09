#include "maptile.h"



maptile::~maptile()
{
}

maptile * floor()
{
	return new maptile("Floor", BASIC_FLOOR_TILE, TCODColor::darkGreen, TCODColor::darkestGreen);
}

maptile * wall()
{
	return new maptile("Wall", BASIC_WALL_TILE, TCODColor::darkGrey, TCODColor::black, false, false);
}
