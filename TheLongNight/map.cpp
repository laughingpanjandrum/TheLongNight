#include "map.h"



/*
Basic constructor. Creates an empty map of the given size.
*/
map::map(int xsize, int ysize)
{
	this->xsize = xsize;
	this->ysize = ysize;
	//Fill our array of maptiles with empty tiles.
	for (int x = 0; x < xsize; x++) {
		//Each x-coordinate is a vector
		maptileVector1d* yvector = new maptileVector1d;
		boolVector1d ymemory;
		//Fill this vector with empty tiles
		for (int y = 0; y < ysize; y++) {
			yvector->push_back(tile_Floor());
			ymemory.push_back(false);
		}
		//And then push this vector onto the x-list
		maptiles.push_back(yvector);
		memoryMap.push_back(ymemory);
	}
	//Also build a datamap for FOV/etc
	datamap = new TCODMap(xsize, ysize);
	datamap->clear(true, true);
}


map::~map()
{
}

/*
	BOUNDARY CHECKING
*/

bool map::inBounds(int x, int y)
{
	return x >= 0 && x < xsize && y >= 0 && y < ysize;
}

/*
Add a new connection to a given side of the map.
The string is the handle (filename) of the map we're attaching.
*/
void map::addConnection(connectionPoint dr, std::string mapHandle)
{
	connectionPoints.push_back(dr);
	connectionHandles.push_back(mapHandle);
}

/*
Returns the map handle associated with the given direction, or nullptr if none is found.
*/
std::string map::getConnection(connectionPoint dr)
{
	for (int i = 0; i < connectionPoints.size(); i++) {
		if (connectionPoints.at(i) == dr)
			return connectionHandles.at(i);
	}
	return "";
}

/*
	SETTING
*/

/*
Change the tile at the given position.
*/
void map::setTile(maptile * t, int x, int y)
{
	//Change tile
	maptiles.at(x)->at(y) = t;
	//Update metadata at this point
	updateDatamapAtPoint(x, y);
}

/*
Insert a person into the map at the given position.
*/
void map::addPerson(person * p, int x, int y)
{
	people.push_back(p);
	p->setPosition(x, y);
}

/*
Delete a character from the map.
*/
void map::removePerson(person * p)
{
	people.erase(std::find(people.begin(), people.end(), p));
}

/*
Add an item to the map.
*/
void map::addItem(item * it, int x, int y)
{
	items.push_back(it);
	it->setPosition(x, y);
}

/*
Delete an item from the map.
*/
void map::removeItem(item * it)
{
	auto iter = std::find(items.begin(), items.end(), it);
	items.erase(iter);
}


/*
These respawn each time the map is reloaded
*/
void map::addMonsterSpawner(std::string monsterTag, coord xy)
{
	monsterSpawnTags.push_back(monsterTag);
	monsterSpawnCoords.push_back(xy);
}


/*
Update datamap properties - walkable & seethru - at the given point.
Call whenever things change at a specific point.
*/
void map::updateDatamapAtPoint(int x, int y)
{
	maptile* t = getTile(x, y);
	datamap->setProperties(x, y, t->isSeeThrough(), t->isWalkable());
}


/*
	GETTING
*/

/*
Returns the tile at the given position.
*/
maptile * map::getTile(int x, int y)
{
	return maptiles.at(x)->at(y);
}

/*
Returns the person at the given coordinate, or nullptr if there is none.
*/
person * map::getPerson(int x, int y)
{
	for (auto p : people) {
		if (p->isAtPosition(x, y))
			return p;
	}
	return nullptr;
}

item * map::getItem(int x, int y)
{
	for (auto i : items) {
		if (i->isAtPosition(x, y))
			return i;
	}
	return nullptr;
}



/*
	Clear and respawn npcs
*/


/*
Deletes all monsters and spawns new ones.
Also deletes the PC so make sure they get re-added if you want them to exist!
*/
void map::respawnAllMonsters()
{
	//Remove existing creatures
	people.clear();
	//Spawn anew
	for (int i = 0; i < monsterSpawnTags.size(); i++) {
		//Get monster corresponding to this tag
		monster* m = getMonsterByHandle(monsterSpawnTags.at(i));
		//Make sure we don't respawn the boss!
		if ((!bossDestroyed || !m->isBoss)) {
			coord c = monsterSpawnCoords.at(i);
			addPerson(m, c.first, c.second);
		}
	}
}


/*
	FOV STUFF
*/



/*
Computes FOV for a person at the given position.
*/
void map::updateFOV(int xpos, int ypos)
{
	datamap->computeFov(xpos, ypos);
}

bool map::isPointInFOV(int x, int y)
{
	return datamap->isInFov(x, y);
}

TCODPath* map::getPath()
{
	TCODPath *path = new TCODPath(datamap);
	return path;
}
