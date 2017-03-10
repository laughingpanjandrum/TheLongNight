
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>

#include "maptile.h"
#include "person.h"
#include "monster.h"
#include "item.h"


enum connectionPoint {
	CONNECT_NORTH, CONNECT_SOUTH, CONNECT_EAST, CONNECT_WEST,
	CONNECT_VERTICAL, CONNECT_WARP,
};


//Story event, i.e. a specific monster tag is placed on a specific map
struct storyEvent {
	storyEvent(std::string storyFlag, std::string mapFlag, std::string monsterTag, coord spawnPt) :
		storyFlag(storyFlag), mapFlag(mapFlag), monsterTag(monsterTag), spawnPt(spawnPt) {}
	std::string storyFlag;
	std::string mapFlag;
	std::string monsterTag;
	coord spawnPt;
};



typedef std::vector<connectionPoint> connectionVector;
typedef std::vector<storyEvent> storyEventVector;


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
	void setMapTag(std::string t) { mapTag = t; }
	std::string getMapTag() { return mapTag; }

	//Connections and boundary checking
	bool inBounds(int x, int y);
	void addConnection(connectionPoint dr, std::string mapHandle);
	std::string getConnection(connectionPoint dr);

	//Setting
	void setTile(maptile* t, int x, int y);
	void addPerson(personSharedPtr p, int x, int y);
	void removePerson(personSharedPtr p);
	void addItem(itemSharedPtr it, int x, int y);
	void removeItem(itemSharedPtr it);
	void setStartPoint(coord sp) { startPt = sp; }
	void addMonsterSpawner(std::string monsterTag, coord xy);
	void removeMonsterSpawner(std::string monsterTag);

	//Datamap
	void updateDatamapAtPoint(int x, int y);

	//Getting: position and elements
	int getXSize() { return xsize; }
	int getYSize() { return ysize; }
	maptile* getTile(int x, int y);
	personSharedPtr getPerson(int x, int y);
	personVector getAllPeople() { return people; }
	itemSharedPtr getItem(int x, int y);
	itemVector getAllItems() { return items; }
	coord getStartPoint() { return startPt; }

	//Getting: movement data
	bool isWalkable(int x, int y) { return datamap->isWalkable(x, y); }
	bool isSeeThrough(int x, int y) { return datamap->isTransparent(x, y); }

	//Spawning
	void respawnAllMonsters(storyEventVector eventsToWatch);
	void addMovingMonster(storyEventVector eventsToWatch);
	bool checkForMonsterMovement(storyEventVector eventsToWatch, std::string spawnTag);
	bool bossDestroyed = false;

	//FOV/pathfinding stuff
	void updateFOV(int xpos, int ypos);
	bool isPointInFOV(int x, int y);
	TCODPath* getPath();

	//Lighting
	void setDarknessAdjustment(float f) { darknessAdjustment = f; }
	float getDarknessAdjustment() { return darknessAdjustment; }

	//Memory map
	void addToMemoryMap(int x, int y) { memoryMap.at(x).at(y) = true; }
	bool inMemoryMap(int x, int y) { return memoryMap.at(x).at(y); }

	//Fog map
	bool hasFog = false;
	bool isFoggy(int x, int y) { return fogMap.at(x).at(y); }
	void setFog(bool foggy, int x, int y) { fogMap.at(x).at(y) = foggy; }
	void toggleFog(int x, int y) { fogMap.at(x).at(y) = !fogMap.at(x).at(y); }
	void createFogCloud(int ctrx, int ctry, int radius);
	int countAdjacentFogTiles(int x, int y);
	void progressFog();

private:

	//Flava
	std::string name;
	std::string mapTag; //how we're identified

	//Boundaries and map creation
	int xsize, ysize;
	coord startPt;
	connectionVector connectionPoints;
	std::vector<std::string> connectionHandles;

	//Map elements
	maptileVector2d maptiles;
	boolVector2d memoryMap;
	boolVector2d fogMap;
	boolVector2d fogBirthMap;
	boolVector2d fogDeathMap;
	personVector people;
	itemVector items;

	//Fog attributes
	const static int FOG_BIRTH_COUNT = 4;
	const static int FOG_DEATH_COUNT_HIGH = 4;
	const static int FOG_DEATH_COUNT_LOW = 1;

	//Saved list of monsters to respawn
	std::vector<std::string> monsterSpawnTags;
	std::vector<std::string> noRespawnList;
	coordVector monsterSpawnCoords;

	//TCOD map data - for FOV, pathfinding, etc.
	TCODMap* datamap;

	//Lighting
	float darknessAdjustment;

};

typedef std::vector<map*> mapVector;

#endif
