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
