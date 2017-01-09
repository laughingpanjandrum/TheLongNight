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
		//Fill this vector with empty tiles
		for (int y = 0; y < ysize; y++)
			yvector->push_back(new maptile());
		//And then push this vector onto the x-list
		maptiles.push_back(yvector);
	}
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
	GETTING
*/

maptile * map::getTile(int x, int y)
{
	return maptiles.at(x)->at(y);
}
