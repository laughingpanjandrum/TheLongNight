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
		EXPLOSION
*/


explosion::explosion(coord ctr, int radius, TCODColor col1, TCODColor col2) :
	ctr(ctr), radius(radius) 
{
	tickDelay.setTo(3);
	//Generate map of colours, spreading out from zero to the radius
	int idx[] = { 0, radius };
	TCODColor cols[] = { col1, col2 };
	colArray = new TCODColor[radius];
	TCODColor::genMap(colArray, 2, cols, idx);
}

/*
Explosion fades from col1 to col2 as we go outward
*/
drawData explosion::getDrawData(drawData * baseData, int x, int y)
{
	int dist = hypot(x - ctr.first, y - ctr.second);
	if (dist <= atPoint && atPoint <= radius) {
		baseData->bgcolor = colArray[atPoint - 1];
	}
	return *baseData;
}

/*
We have a bit of a delay between each expansion of the explosion radius
*/
void explosion::tick()
{
	tickDelay.decrease();
	if (tickDelay.isEmpty()) {
		tickDelay.restore();
		atPoint++;
	}
}



/*
		BULLET PATH
*/


bulletPath::bulletPath(coordVector pts, int tileCode, TCODColor color) :
	pts(pts), tileCode(tileCode), color(color)
{
}

drawData bulletPath::getDrawData(drawData * baseData, int x, int y)
{
	if (atIdx < pts.size()) {
		coord pt = pts.at(atIdx);
		if (pt.first == x && pt.second == y) {
			baseData->tileCode = tileCode;
			baseData->color = color;
		}
	}
	return *baseData;
}

void bulletPath::tick()
{
	atIdx++;
}
