#include "map.h"



/*
Basic constructor. Creates an empty map of the given size.
*/
map::map(int xsize, int ysize)
{
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
