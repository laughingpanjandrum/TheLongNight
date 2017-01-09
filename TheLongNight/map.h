
#ifndef MAP_H
#define MAP_H

#include <vector>
#include "maptile.h"
#include "person.h"

class map
{
	typedef std::vector<maptile*> maptileVector1d;
	typedef std::vector<maptileVector1d*> maptileVector2d;
	typedef std::vector<person*> personVector;
public:
	//Constructors/destructor
	map() : map(30, 30) {}
	map(int xsize, int ysize);
	~map();
	//Boundary checking
	bool inBounds(int x, int y);
	//Setting
	void setTile(maptile* t, int x, int y);
	void addPerson(person* p, int x, int y);
	//Getting: position and elements
	int getXSize() { return xsize; }
	int getYSize() { return ysize; }
	maptile* getTile(int x, int y);
	person* getPerson(int x, int y);
	personVector getAllPeople() { return people; }
	//Getting: movement data
	bool isWalkable(int x, int y) { return datamap->isWalkable(x, y); }
	bool isSeeThrough(int x, int y) { return datamap->isTransparent(x, y); }
	//FOV stuff
	void updateFOV(int xpos, int ypos);
	bool isPointInFOV(int x, int y);
private:
	//Boundaries and map creation
	int xsize, ysize;
	//Map elements
	maptileVector2d maptiles;
	personVector people;
	//TCOD map data - for FOV, pathfinding, etc.
	TCODMap* datamap;
};

#endif
