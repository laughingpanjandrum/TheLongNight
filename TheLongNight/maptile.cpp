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
	return new maptile("Floor", BASIC_FLOOR_TILE, TCODColor::darkGreen, TCODColor::darkestGreen);
}

maptile * wall()
{
	return new maptile("Wall", BASIC_WALL_TILE, TCODColor::darkGrey, TCODColor::black, false, false);
}

maptile * acid()
{
	maptile* a = new maptile("Acid", BASIC_WATER_TILE, TCODColor::lighterGreen, TCODColor::green);
	a->addTouchEffect(APPLY_PHYSICAL_DAMAGE);
	a->setPotency(5);
	return a;
}
