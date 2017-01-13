#include "animations.h"

/*
Given person briefly changes to the given color, then changes back.
*/
flashCharacter::flashCharacter(person* p, TCODColor color) :
	p(p), color(color)
{

}

drawData flashCharacter::getDrawData(drawData * baseData, int x, int y)
{
	if (x == p->getx() && y == p->gety())
		baseData->color = color;
	return *baseData;
}
