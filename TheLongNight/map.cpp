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
	SETTING
*/

/*
Change the tile at the given position.
*/
void map::setTile(maptile * t, int x, int y)
{
	maptiles.at(x)->at(y) = t;
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
