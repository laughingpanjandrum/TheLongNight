#include "element.h"



element::~element()
{
}

/*
	SETTERS
*/

/*
Sets our current position.
Input: int x, int y.
*/
void element::setPosition(int x, int y)
{
	xy.first = x;
	xy.second = y;
}

/*
Sets our current position.
Input: a coord object (which is just a pair of ints)
*/
void element::setPosition(coord pos)
{
	xy = pos;
}

/*
Returns whether our position matches the given coordinates.
*/
bool element::isAtPosition(int x, int y)
{
	return x == xy.first && y == xy.second;
}

TCODColor element::getColor()
{
	if (isGlittery) {
		//We create a colour close to, but not exactly like, our colour
		if (ascendingGlitter) {
			glitterCoef += 0.05;
			if (glitterCoef >= 0.8)
				ascendingGlitter = false;
		}
		else {
			glitterCoef -= 0.05;
			if (glitterCoef <= 0.1)
				ascendingGlitter = true;
		}
		return TCODColor::lerp(color, TCODColor::white, glitterCoef);
	}
	return color;
}
