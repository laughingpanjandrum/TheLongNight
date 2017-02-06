#include "animations.h"



//Converts the given vector of coordinates into a vector of pointers to coordinates.
pathVectorP * animations::getPointerPath(coordVector * pts)
{
	pathVectorP* vec = new pathVectorP();
	for (auto iter = pts->begin(); iter != pts->end(); iter++) {
		vec->push_back(new coord(iter->first, iter->second));
	}
	return vec;
}



/*
Given person briefly changes to the given color, then changes back.
*/
flashCharacter::flashCharacter(person* p, TCODColor color) :
	xy(p->getPosition()), color(color)
{

}

/*
Changes player's color
*/
drawData* flashCharacter::getDrawData(drawData * baseData, const int x, const int y)
{
	if (x == xy.first && y == xy.second)
		baseData->color = color;
	return baseData;
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
drawData* explosion::getDrawData(drawData * baseData, const int x, const int y)
{
	int dist = hypot(x - ctr.first, y - ctr.second);
	if (dist <= atPoint && atPoint <= radius) {
		baseData->bgcolor = colArray[atPoint - 1];
	}
	return baseData;
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


bulletPath::bulletPath(coordVector* pts, int tileCode, TCODColor color) :
	tileCode(tileCode), color(color)
{
	//Create a brand-new path
	this->pts = new coordVector();
	for (auto iter = pts->begin(); iter != pts->end(); iter++) {
		this->pts->push_back(coord(iter->first, iter->second));
	}
}

drawData* bulletPath::getDrawData(drawData * baseData, const int x, const int y)
{
	if (atIdx < pts->size()) {
		coord* pt = new coord(pts->at(atIdx));
		if (pt->first == x && pt->second == y) {
			baseData->tileCode = tileCode;
			baseData->color = color;
		}
	}
	return baseData;
}

void bulletPath::tick()
{
	atIdx++;
}



/*
	GLOW PATH
*/


glowPath::glowPath(coordVector* pts, TCODColor col1, TCODColor col2)
{
	//Create a brand-new path
	this->pts = new coordVector();
	for (auto iter = pts->begin(); iter != pts->end(); iter++) {
		this->pts->push_back(coord(iter->first, iter->second));
	}
	//Colors fade from col1 to col2
	int idx[] = { 0, pts->size() };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[pts->size()];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawData* glowPath::getDrawData(drawData * baseData, const int x, const int y)
{
	//Each point up to idx has a different color.
	for (int i = 0; i < atIdx; i++) {
		coord* pt = new coord(pts->at(i));
		if (pt->first == x && pt->second == y) {
			baseData->bgcolor = colors[i];
			break;
		}
	}
	return baseData;
}

void glowPath::tick()
{
	atIdx++;
}


/*
	GLYPH CYCLE
*/


glyphCycle::glyphCycle(coordVector* pts, TCODColor col1, TCODColor col2):
	pts(pts)
{
	//Make a colour map for us to cycle through
	int idx[] = { 0, 10 };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[10];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawData* glyphCycle::getDrawData(drawData * baseData, const int x, const int y)
{
	for (auto pt = pts->begin(); pt != pts->end(); pt++) 
	{
		if (pt->first == x && pt->second == y)
		{
			//Pick a random glyph
			int glyph = randint(128, 175);
			baseData->tileCode = glyph;
			//Pick a random color
			int idx = randrange(10);
			baseData->color = colors[idx];
			break;
		}
	}
	//None of our points correspond
	return baseData;
}


/*
	SHOCKWAVE
*/

shockwave::shockwave(int x, int y, TCODColor col1, TCODColor col2) :
	x(x), y(y)
{
	//Color map traveling outward
	int idx[] = { 0, RADIUS };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[RADIUS];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawData* shockwave::getDrawData(drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	int dist = hypot(x - this->x, y - this->y);
	if (dist == atIdx + 1) {
		//This is one of our spaces!
		baseData->bgcolor = colors[atIdx];
	}
	return baseData;
}
