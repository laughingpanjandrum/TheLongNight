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



void maptile::makeDoor(std::string unlockCode)
{
	isDoor = true;
	this->unlockCode = unlockCode;
	addTouchEffect(CHECK_FOR_UNLOCK);
}

/*
Used by ritual altars!
*/
void maptile::activateAltar()
{
	color = TCODColor::lightRed;
	isAltarActive = true;
}



/*
	All tiles
*/



maptile * tile_Floor()
{
	return new maptile("Floor", "floor_stone", BASIC_FLOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR);
}

maptile * tile_MagicFloor()
{
	return new maptile("Floor", "floor_magic", BASIC_FLOOR_TILE, TCODColor::lightPurple, DARK_STONE_COLOR);
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
	v->addTouchEffect(APPLY_VOID_DAMAGE);
	v->setPotency(50);
	return v;
}

maptile * tile_Carpet()
{
	return new maptile("Carpet", "floor_carpet", CARPET_TILE, TCODColor::darkerRed, TCODColor::darkestRed);
}

maptile * tile_SnowFloor()
{
	return new maptile("Snow", "floor_snow", BASIC_FLOOR_TILE, TCODColor::cyan, TCODColor::lighterGrey);
}

maptile * tile_SingedFloor()
{
	return new maptile("Floor", "floor_singed", BASIC_FLOOR_TILE, TCODColor::lightFlame, DARK_STONE_COLOR);
}

maptile * tile_SandyRock()
{
	return new maptile("Sandy Rock", "wall_sand", STONE_WALL_TILE, DARKEST_SAND_COLOR, LIGHT_SAND_COLOR, false, false);
}

maptile * tile_SecretWall()
{
	return new maptile("Sandy Rock", "wall_secret", STONE_WALL_TILE, DARKEST_SAND_COLOR, LIGHT_SAND_COLOR, true, false);
}

maptile * tile_Wall()
{
	return new maptile("Wall", "wall_stone", BASIC_WALL_TILE, DARK_STONE_COLOR, LIGHT_STONE_COLOR, false, false);
}

maptile * tile_MagicWall()
{
	return new maptile("Wall", "wall_magic", BASIC_WALL_TILE, DARK_STONE_COLOR, TCODColor::lightPurple, false, false);
}

maptile * tile_WoodenWall()
{
	return new maptile("Wall", "wall_wood", WOODEN_WALL, LIGHT_WOOD_COLOR, DARK_WOOD_COLOR, false, false);
}

maptile * tile_MarbleWall()
{
	return new maptile("Marble Wall", "wall_marble", BASIC_WALL_TILE, TCODColor::darkCyan, TCODColor::white, false, false);
}

maptile * tile_GoldenWall()
{
	return new maptile("Golden Wall", "wall_golden", BASIC_WALL_TILE, TCODColor::gold, TCODColor::black, false, false);
}

maptile * tile_Bars()
{
	return new maptile("Bars", "bars", BARS_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR, false, true);
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

maptile * tile_CellDoor()
{
	return new maptile("Cell Door", "door_cell", CELL_DOOR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR);
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
	return new maptile("Rose-laden Branch", "rosebush", TREE_TILE, TCODColor::pink, DARK_GRASS_COLOR, true, false);
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

maptile * tile_SnowboundTree()
{
	return new maptile("Snowbound Tree", "snowbound_tree", TREE_TILE, DARK_WOOD_COLOR, TCODColor::lighterGrey, false, false);
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

maptile * tile_DrainingPool()
{
	maptile* a = new maptile("Draining Pool", "draining_pool", BASIC_WATER_TILE, 
		TCODColor::lighterMagenta, TCODColor::blue);
	a->addTouchEffect(DRAIN_VIGOUR);
	a->setPotency(1);
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

maptile * tile_PoisonOoze()
{
	maptile* ooze = new maptile("Poison Ooze", "poison_ooze", BASIC_WATER_TILE, TCODColor::darkLime, TCODColor::lime);
	ooze->addTouchEffect(APPLY_POISON_DAMAGE);
	ooze->setPotency(10);
	return ooze;

}

maptile * tile_CorrodingBlood()
{
	maptile* blood = new maptile("Corroding Blood", "corroding_blood", BASIC_WATER_TILE, TCODColor::darkRed, TCODColor::red);
	blood->addTouchEffect(APPLY_ACID_DAMAGE);
	blood->setPotency(20);
	return blood;
}

maptile * tile_Bloodstain()
{
	return new maptile("Bloodstain", "bloodstain", BASIC_FLOOR_TILE, TCODColor::darkCrimson, DARK_STONE_COLOR);
}

maptile * tile_Corpse()
{
	return new maptile("Corpse", "corpse", CORPSE_TILE, TCODColor::darkCrimson, DARK_STONE_COLOR);
}

maptile * tile_Bones()
{
	return new maptile("Bones", "bones", CORPSE_TILE, TCODColor::lightestYellow, DARK_STONE_COLOR);
}

maptile * tile_SandBones()
{
	return new maptile("Bones", "bones_sand", CORPSE_TILE, TCODColor::lightestYellow, DARKEST_SAND_COLOR);
}

maptile * tile_Bed()
{
	return new maptile("Ruined Bed", "bed", BED_TILE, TCODColor::darkestRed, DARK_STONE_COLOR);
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

maptile * tile_StairsDown()
{
	maptile* s = new maptile("Stairs Down", "downstairs", STAIRS_DOWN_TILE, TCODColor::white, DARK_STONE_COLOR,
		true, true, true);
	s->addTouchEffect(NAVIGATE_STAIRS);
	return s;
}

maptile * tile_StairsUp()
{
	maptile* s = new maptile("Stairs Up", "upstairs", STAIRS_UP_TILE, TCODColor::white, DARK_STONE_COLOR,
		true, true, true);
	s->addTouchEffect(NAVIGATE_STAIRS);
	return s;
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

maptile * tile_BasementDoor()
{
	maptile* door = new maptile("Basement Door", "basement_door", BASIC_DOOR_TILE,
		TCODColor::lightGrey, TCODColor::darkGrey, false, false);
	door->isDoor = true;
	door->unlockCode = "lady_tverts_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_SparrowTowerDoor()
{
	maptile* door = new maptile("Sparrow Tower Door", "sparrow_tower_door", BASIC_DOOR_TILE,
		TCODColor::cyan, TCODColor::darkGrey, false, false);
	door->isDoor = true;
	door->unlockCode = "dead_sparrows_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_ObservatoryDoor()
{
	maptile* door = new maptile("Observatory Door", "observatory_door", BASIC_DOOR_TILE,
		TCODColor::fuchsia, TCODColor::darkGrey, false, false);
	door->isDoor = true;
	door->unlockCode = "moshkas_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_MoonPalaceDoor()
{
	maptile* door = new maptile("Moon Palace Door", "moon_palace_door", BASIC_DOOR_TILE,
		TCODColor::lightBlue, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "moon_pale_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_GraveDoor()
{
	maptile* door = new maptile("Grave Door", "grave_door", BASIC_DOOR_TILE,
		TCODColor::amber, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "farins_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_AtalundraGate()
{
	maptile* door = new maptile("Atalundra Gate", "atalundra_gate", BASIC_DOOR_TILE,
		TCODColor::brass, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "atalundra_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_RatDoor()
{
	maptile* door = new maptile("Rat Door", "rat_door", BASIC_DOOR_TILE, TCODColor::lightSepia, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "percivels_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_BloodyMausoleumDoor()
{
	maptile* door = new maptile("Bloody Mausoleum Door", "bloody_mausoleum_door", BASIC_DOOR_TILE,
		TCODColor::crimson, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "corens_bones";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_LostDoor()
{
	maptile* door = new maptile("Lost Door", "lost_door", BASIC_DOOR_TILE,
		TCODColor::cyan, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "khalles_bones";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_SilentDoor()
{
	maptile* door = new maptile("Silent Door", "silent_door", BASIC_DOOR_TILE,
		TCODColor::green, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "siltras_bones";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_VoidMausoleumDoor()
{
	maptile* door = new maptile("Void Mausoleum Door", "void_mausoleum_door", BASIC_DOOR_TILE,
		TCODColor::purple, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "ietras_bones";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_AncientPrisonDoor()
{
	maptile* door = new maptile("Ancient Prison Door", "ancient_prison_door", BASIC_DOOR_TILE,
		TCODColor::amber, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "ietras_amber_pendant";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_SlumberingGate()
{
	maptile* door = new maptile("Slumbering Gate", "slumbering_gate", BASIC_DOOR_TILE,
		TCODColor::lightRed, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "effigy";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_UnderpoolDoor()
{
	maptile* door = new maptile("Underpool Door", "underpool_door", BASIC_DOOR_TILE,
		TCODColor::lighterCrimson, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "underpool_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_AbattoirDoor()
{
	maptile* door = new maptile("Abattoir Door", "abattoir_door", BASIC_DOOR_TILE,
		TCODColor::lighterCrimson, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "abattoir_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_RoseCourtGate()
{
	maptile* door = new maptile("Rose Court Gate", "rose_court_gate", BASIC_DOOR_TILE,
		TCODColor::lightPink, TCODColor::black, false, false);
	door->isDoor = true;
	door->unlockCode = "green_knights_key";
	door->addTouchEffect(CHECK_FOR_UNLOCK);
	return door;
}

maptile * tile_MoonGate()
{
	maptile* mg = new maptile("Moon Gate", "moon_gate", BASIC_FLOOR_TILE,
		TCODColor::black, TCODColor::pink);
	mg->isGlittery = true;
	mg->addTouchEffect(MOON_GATE_WARP);
	return mg;
}

maptile * tile_FlameDoor()
{
	maptile* door = new maptile("Flame Door", "flame_door", BASIC_DOOR_TILE,
		TCODColor::black, TCODColor::darkFlame, false, false);
	door->makeDoor("molten_priests_key");
	return door;
}

maptile * tile_GateOfUrGnash()
{
	maptile* door = new maptile("Gate of Ur-Gnash", "gate_of_ur_gnash", BASIC_DOOR_TILE,
		TCODColor::black, TCODColor::lime, false, false);
	door->makeDoor("klugs_key");
	return door;
}


/*
	Ritual altars
*/

maptile * tile_AltarOfTheEye()
{
	maptile* a = new maptile("Altar of the Eye", "eye_altar", ALTAR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR,
		true, true, true);
	a->unlockCode = "watchful_eyestalk";
	a->addTouchEffect(MAKE_RITUAL_OFFERING);
	return a;
}

maptile * tile_AltarOfTheMouth()
{
	maptile* a = new maptile("Altar of the Mouth", "mouth_altar", ALTAR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR,
		true, true, true);
	a->unlockCode = "mawtooth_fragment";
	a->addTouchEffect(MAKE_RITUAL_OFFERING);
	return a;
}

maptile * tile_AltarOfTheHand()
{
	maptile* a = new maptile("Altar of the Hand", "hand_altar", ALTAR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR,
		true, true, true);
	a->unlockCode = "withered_finger";
	a->addTouchEffect(MAKE_RITUAL_OFFERING);
	return a;
}

maptile * tile_AltarOfTheHeart()
{
	maptile* a = new maptile("Altar of the Heart", "heart_altar", ALTAR_TILE, LIGHT_STONE_COLOR, DARK_STONE_COLOR,
		true, true, true);
	a->unlockCode = "beating_heart";
	a->addTouchEffect(MAKE_RITUAL_OFFERING);
	return a;
}


/*
	Furniture
*/

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
