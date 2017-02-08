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
	return new maptile("Floor", "floor_stone", BASIC_FLOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_MossyFloor()
{
	return new maptile("Floor", "floor_mossy_stone", MOSS_TILE, LIGHT_GRASS_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Sand()
{
	return new maptile("Sand", "floor_sand", SAND_TILE, DARK_SAND_COLOR, DARKEST_SAND_COLOR);
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

maptile * tile_Carpet()
{
	return new maptile("Carpet", "floor_carpet", CARPET_TILE, TCODColor::darkerRed, TCODColor::black);
}

maptile * tile_SnowFloor()
{
	return new maptile("Snow", "floor_snow", BASIC_FLOOR_TILE, TCODColor::cyan, TCODColor::lighterGrey);
}

maptile * tile_SandyRock()
{
	return new maptile("Sandy Rock", "wall_sand", STONE_WALL_TILE, DARKEST_SAND_COLOR, LIGHT_SAND_COLOR, false, false);
}

maptile * tile_Wall()
{
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, DARK_STONE_COLOR, LIGHT_STONE_COLOR, false, false);
}

maptile * tile_WoodenWall()
{
	return new maptile("Wall", "wall_wood", WOODEN_WALL, LIGHT_WOOD_COLOR, DARK_WOOD_COLOR, false, false);
}

maptile * tile_MarbleWall()
{
	return new maptile("Marble Wall", "wall_marble", BASIC_WALL_TILE, TCODColor::darkCyan, TCODColor::white, false, false);
}

maptile * tile_Door()
{
	maptile* d = new maptile("Door", "door_stone", BASIC_DOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR, true, false);
	return d;
}

maptile * tile_WoodenDoor()
{
	return new maptile("Door", "door_wood", BASIC_DOOR_TILE, DARK_WOOD_COLOR, LIGHT_WOOD_COLOR, true, false);
}

/*
Door is initially locked (but can be unlocked later)
*/
maptile * tile_LockedDoor()
{
	maptile* d = new maptile("Door", "locked_door", BASIC_DOOR_TILE, DARK_STONE_COLOR, LIGHT_STONE_COLOR, false, false);
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
	return new maptile("Glass", "glass", GLASS_TILE, LIGHT_GLASS_COLOR, DARK_GLASS_COLOR, false, true);
}

maptile * tile_Bush()
{
	return new maptile("Bush", "bush", BUSH_TILE, TCODColor::lightestGreen, DARK_GRASS_COLOR, true, false);
}

maptile * tile_Rosebush()
{
	return new maptile("Rosebush", "rosebush", BUSH_TILE, TCODColor::pink, DARK_GRASS_COLOR, true, false);
}

maptile * tile_Tree()
{
	return new maptile("Tree", "tree", TREE_TILE, TREE_COLOR, DARK_GRASS_COLOR, false, false);
}

maptile * tile_VoidTree()
{
	return new maptile("Withered Void Tree", "void_tree", TREE_TILE, TCODColor::purple, DARK_STONE_COLOR, false, false);
}

maptile * tile_DeadTree()
{
	return new maptile("Dead Tree", "dead_tree", TREE_TILE, DARK_WOOD_COLOR, DARK_SAND_COLOR, false, false);
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

maptile * tile_Web()
{
	maptile* web = new maptile("Web", "web", WEB_TILE, TCODColor::white, DARK_GRASS_COLOR);
	web->addTouchEffect(APPLY_ENTANGLING);
	web->setPotency(1);
	return web;
}

maptile * tile_WebOnFloor()
{
	maptile* web = new maptile("Web", "web_floor", WEB_TILE, TCODColor::white, DARK_STONE_COLOR);
	web->addTouchEffect(APPLY_ENTANGLING);
	web->setPotency(1);
	return web;
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
	maptile* door = new maptile("Wretched Door", "wretched_door", BASIC_DOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR, 
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

maptile * tile_VoidDrenchedDoor()
{
	maptile* door = new maptile("Void Drenched Door", "void_drenched_door", BASIC_DOOR_TILE,
		TCODColor::lightPurple, TCODColor::darkPurple, false, false);
	door->isDoor = true;
	door->unlockCode = "spinal_column_shard";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_DoorOfWinter()
{
	maptile* door = new maptile("Door of Winter", "door_of_winter", BASIC_DOOR_TILE,
		TCODColor::lightGrey, TCODColor::white, false, false);
	door->isDoor = true;
	door->unlockCode = "watchful_eyestalk";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_OldValleyDoor()
{
	maptile* door = new maptile("Old Valley Door", "old_valley_door", BASIC_DOOR_TILE,
		TCODColor::lightBlue, TCODColor::darkerBlue, false, false);
	door->isDoor = true;
	door->unlockCode = "old_valley_door";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_DesolateChurchDoor()
{
	maptile* door = new maptile("Desolate Church Door", "desolate_church_door", BASIC_DOOR_TILE,
		TCODColor::darkFlame, TCODColor::grey, false, false);
	door->isDoor = true;
	door->unlockCode = "bartons_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_HightowerDoor()
{
	maptile* door = new maptile("Hightower Door", "hightower_door", BASIC_DOOR_TILE,
		TCODColor::pink, TCODColor::grey, false, false);
	door->isDoor = true;
	door->unlockCode = "hightower_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_Chair1()
{
	return new maptile("Chair", "chair1", CHAIR_TILE_1,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Chair2()
{
	return new maptile("Chair", "chair2", CHAIR_TILE_2,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Table1()
{
	return new maptile("Table", "table1", TABLE_TILE_1,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Table2()
{
	return new maptile("Table", "table2", TABLE_TILE_2,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Table3()
{
	return new maptile("Table", "table3", TABLE_TILE_3,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Table4()
{
	return new maptile("Table", "table4", TABLE_TILE_4,
		TREE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_Tombstone()
{
	return new maptile("Tombstone", "tombstone", TOMBSTONE_TILE,
		LIGHT_STONE_COLOR, DARKEST_GRASS_COLOR);
}
