
#ifndef MAP_H
#define MAP_H

#include <vector>
#include "maptile.h"
#include "person.h"
#include "monster.h"
#include "item.h"

enum connectionPoint {
	CONNECT_NORTH, CONNECT_SOUTH, CONNECT_EAST, CONNECT_WEST,
	CONNECT_VERTICAL, CONNECT_WARP
};

typedef std::vector<person*> personVector;
typedef std::vector<item*> itemVector;
typedef std::vector<connectionPoint> connectionVector;

class map
{
	typedef std::vector<maptile*> maptileVector1d;
	typedef std::vector<maptileVector1d*> maptileVector2d;
	typedef std::vector<bool> boolVector1d;
	typedef std::vector<boolVector1d> boolVector2d;
public:

	//Constructors/destructor
	map() : map(30, 30) {}
	map(int xsize, int ysize);
	~map();

	//Flava getting & setting
	void setName(std::string n) { name = n; }
	std::string getName() { return name; }

	//Connections and boundary checking
	bool inBounds(int x, int y);
	void addConnection(connectionPoint dr, std::string mapHandle);
	std::string getConnection(connectionPoint dr);

	//Setting
	void setTile(maptile* t, int x, int y);
	void addPerson(person* p, int x, int y);
	void removePerson(person* p);
	void addItem(item* it, int x, int y);
	void removeItem(item* it);
	void setStartPoint(coord sp) { startPt = sp; }
	void addMonsterSpawner(std::string monsterTag, coord xy);

	//Datamap
	void updateDatamapAtPoint(int x, int y);

	//Getting: position and elements
	int getXSize() { return xsize; }
	int getYSize() { return ysize; }
	maptile* getTile(int x, int y);
	person* getPerson(int x, int y);
	personVector getAllPeople() { return people; }
	item* getItem(int x, int y);
	coord getStartPoint() { return startPt; }

	//Getting: movement data
	bool isWalkable(int x, int y) { return datamap->isWalkable(x, y); }
	bool isSeeThrough(int x, int y) { return datamap->isTransparent(x, y); }

	//Spawning
	void respawnAllMonsters();
	bool checkForMonsterMovement(std::string spawnTag);
	bool bossDestroyed = false;

	//FOV/pathfinding stuff
	void updateFOV(int xpos, int ypos);
	bool isPointInFOV(int x, int y);
	TCODPath* getPath();

	//Memory map
	void addToMemoryMap(int x, int y) { memoryMap.at(x).at(y) = true; }
	bool inMemoryMap(int x, int y) { return memoryMap.at(x).at(y); }

private:

	//Flava
	std::string name;

	//Boundaries and map creation
	int xsize, ysize;
	coord startPt;
	connectionVector connectionPoints;
	std::vector<std::string> connectionHandles;

	//Map elements
	maptileVector2d maptiles;
	boolVector2d memoryMap;
	personVector people;
	itemVector items;

	//Saved list of monsters to respawn
	std::vector<std::string> monsterSpawnTags;
	coordVector monsterSpawnCoords;

	//TCOD map data - for FOV, pathfinding, etc.
	TCODMap* datamap;

};

typedef std::vector<map*> mapVector;

#endif
