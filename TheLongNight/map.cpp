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
			yvector->push_back(floor());
		//And then push this vector onto the x-list
		maptiles.push_back(yvector);
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
	datamap->setProperties(x, y, t->isSeeThrough(), t->isWalkable());
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
	people.erase(std::find(people.begin(), people.end(), p), people.end());
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
