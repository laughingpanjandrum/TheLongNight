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



/*
	All tiles
*/



maptile * tile_Floor()
{
	return new maptile("Floor", "floor_stone", BASIC_FLOOR_TILE, TCODColor(47,45,43), TCODColor(23,23,23));
}

maptile * tile_MossyFloor()
{
	return new maptile("Floor", "floor_mossy_stone", MOSS_TILE, LIGHT_GRASS_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Sand()
{
	return new maptile("Sand", "floor_sand", BASIC_FLOOR_TILE, TCODColor::amber, TCODColor::darkerAmber);
}

maptile * tile_Grass()
{
	return new maptile("Grass", "floor_grass", BASIC_FLOOR_TILE, LIGHT_GRASS_COLOR, DARK_GRASS_COLOR);
}

maptile * tile_Road()
{
	return new maptile("Road", "floor_road", BASIC_FLOOR_TILE, TCODColor::sepia, TCODColor::darkerSepia);
}

maptile * tile_Void()
{
	maptile* v = new maptile("Void", "floor_void", BASIC_FLOOR_TILE, TCODColor::darkestPurple, TCODColor::black);
	v->addTouchEffect(APPLY_PROFANE_DAMAGE);
	v->setPotency(50);
	return v;
}

maptile * tile_SandyRock()
{
	return new maptile("Sandy Rock", "wall_sand", BASIC_WALL_TILE, TCODColor::darkAmber, TCODColor::amber, false, false);
}

maptile * tile_Wall()
{
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR, false, false);
}

maptile * tile_WoodenWall()
{
	return new maptile("Wall", "wall_wood", BASIC_WALL_TILE, TCODColor::lightSepia, TCODColor::darkerSepia, false, false);
}

maptile * tile_Door()
{
	maptile* d = new maptile("Door", "door_stone", BASIC_DOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR, true, false);
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
	return new maptile("Bush", "bush", BUSH_TILE, LIGHT_GRASS_COLOR, DARK_GRASS_COLOR, true, false);
}

maptile * tile_Rosebush()
{
	return new maptile("Rosebush", "rosebush", BUSH_TILE, TCODColor::pink, DARK_GRASS_COLOR, true, false);
}

maptile * tile_Tree()
{
	return new maptile("Tree", "tree", TREE_TILE, TCODColor::lightSepia, DARK_GRASS_COLOR, false, false);
}

maptile * tile_VoidTree()
{
	return new maptile("Withered Void Tree", "void_tree", TREE_TILE, TCODColor::purple, DARK_STONE_COLOR, false, false);
}

maptile * tile_Water()
{
	maptile* m = new maptile("Water", "water", BASIC_WATER_TILE, LIGHT_WATER_COLOR, DARK_WATER_COLOR);
	m->isWater = true;
	return m;
}

maptile * tile_MossyWater()
{
	maptile* m = new maptile("Water", "water_mossy", MOSS_TILE, LIGHT_GRASS_COLOR, DARK_WATER_COLOR);
	m->isWater = true;
	return m;
}

maptile * tile_DeepWater()
{
	return new maptile("Deep Water", "water_deep", BASIC_WATER_TILE, LIGHT_WATER_COLOR, DARKEST_WATER_COLOR, false, true);
}

maptile * tile_Acid()
{
	maptile* a = new maptile("Acid", "acid", BASIC_WATER_TILE, TCODColor::lighterGreen, TCODColor::green);
	a->addTouchEffect(APPLY_PHYSICAL_DAMAGE);
	a->setPotency(5);
	return a;
}

maptile * tile_Ooze()
{
	maptile* o = new maptile("Ooze", "ooze", BASIC_WATER_TILE, TCODColor::lightSepia, TCODColor::darkSepia);
	o->addTouchEffect(SLOWDOWN);
	o->setPotency(1);
	return o;
}

maptile * tile_Bloodstain()
{
	return new maptile("Bloodstain", "bloodstain", BASIC_FLOOR_TILE, TCODColor::darkCrimson, DARK_STONE_COLOR);
}

maptile * tile_Corpse()
{
	return new maptile("Corpse", "corpse", CORPSE_TILE, TCODColor::darkCrimson, DARK_STONE_COLOR);
}

maptile * tile_Bed()
{
	return new maptile("Bed", "bed", BED_TILE, TCODColor::darkRed, DARK_STONE_COLOR);
}

maptile * tile_Terminal()
{
	return new maptile("Terminal", "terminal", TERMINAL_TILE, TCODColor::green, DARK_STONE_COLOR);
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

maptile * tile_WyrdDoor()
{
	maptile* door = new maptile("Wyrd Door", "wyrd_door", BASIC_DOOR_TILE, TCODColor::sepia, TCODColor::darkSepia,
		false, false);
	door->unlockCode = "wyrd_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_GreenChapelGardenDoor()
{
	maptile* door = new maptile("Green Chapel Garden Door", "green_chapel_door", BASIC_DOOR_TILE, 
		TCODColor::lightSepia, TCODColor::darkSepia,
		false, false);
	door->unlockCode = "green_chapel_door";
	door->isDoor = true;
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_SordidChapelDoor()
{
	maptile* door = new maptile("Sordid Chapel Door", "sordid_door", BASIC_DOOR_TILE,
		TCODColor::darkGrey, TCODColor::darkCrimson, false, false);
	door->isDoor = true;
	door->unlockCode = "sordid_chapel_door";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}
