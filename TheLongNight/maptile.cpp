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

maptile * floor()
{
	return new maptile("Floor", "floor_stone", BASIC_FLOOR_TILE, TCODColor::darkGreen, TCODColor::darkestGreen);
}

maptile * wall()
{
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, TCODColor::darkGrey, TCODColor::black, false, false);
}

maptile * acid()
{
	maptile* a = new maptile("Acid", "acid", BASIC_WATER_TILE, TCODColor::lighterGreen, TCODColor::green);
	a->addTouchEffect(APPLY_PHYSICAL_DAMAGE);
	a->setPotency(5);
	return a;
}

/*
Resting tile.
*/
maptile * tile_StatueOfRest()
{
	maptile* statue = new maptile("Statue of Rest", "statue", STATUE_TILE, TCODColor::lightCyan, TCODColor::darkestCyan);
	statue->addTouchEffect(ALLOW_INVENTORY_MANAGEMENT);
	return statue;
}
