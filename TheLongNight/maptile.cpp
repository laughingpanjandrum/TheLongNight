#include "maptile.h"



maptile::~maptile()
{
}

maptile * floor()
{
	return new maptile("Floor", BASIC_FLOOR_TILE, TCODColor::darkerGreen, TCODColor::black);
}

maptile * wall()
{
	return new maptile("Wall", BASIC_WALL_TILE, TCODColor::darkerGreen, TCODColor::black, false, false);
}
