
#ifndef MAP_H
#define MAP_H

#include <vector>
#include "maptile.h"

class map
{
	typedef std::vector<maptile*> maptileVector1d;
	typedef std::vector<maptileVector1d*> maptileVector2d;
public:
	//Constructors/destructor
	map() : map(10, 10) {}
	map(int xsize, int ysize);
	~map();
private:
	//Boundaries and map creation
	int xsize, ysize;
	//Map elements
	maptileVector2d maptiles;
};

#endif
