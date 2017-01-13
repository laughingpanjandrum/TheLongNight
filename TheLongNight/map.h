
#ifndef MAP_H
#define MAP_H

#include <vector>
#include "maptile.h"
#include "person.h"
#include "item.h"

typedef std::vector<person*> personVector;
typedef std::vector<item*> itemVector;

class map
{
	typedef std::vector<maptile*> maptileVector1d;
	typedef std::vector<maptileVector1d*> maptileVector2d;
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
	void removePerson(person* p);
	void addItem(item* it, int x, int y);
	void removeItem(item* it);
	void setStartPoint(coord sp) { startPt = sp; }

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

	//FOV/pathfinding stuff
	void updateFOV(int xpos, int ypos);
	bool isPointInFOV(int x, int y);
	TCODPath* getPath();

private:

	//Boundaries and map creation
	int xsize, ysize;
	coord startPt;

	//Map elements
	maptileVector2d maptiles;
	personVector people;
	itemVector items;

	//TCOD map data - for FOV, pathfinding, etc.
	TCODMap* datamap;

};

#endif
