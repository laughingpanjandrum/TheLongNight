#include "animations.h"

/*
Given person briefly changes to the given color, then changes back.
*/
flashCharacter::flashCharacter(person* p, TCODColor color) :
	p(p), color(color)
{

}

/*
Changes player's color
*/
drawData flashCharacter::getDrawData(drawData * baseData, int x, int y)
{
	if (x == p->getx() && y == p->gety())
		baseData->color = color;
	return *baseData;
}


/*
Explosion fades from col1 to col2 as we go outward
*/
drawData explosion::getDrawData(drawData * baseData, int x, int y)
{
	int dist = hypot(x - ctr.first, y - ctr.second);
	if (dist <= atPoint) {
		baseData->bgcolor = col1;
	}
	return *baseData;
}
