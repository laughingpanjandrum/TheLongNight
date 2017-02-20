#include "animations.h"



/*
Given person briefly changes to the given color, then changes back.
*/
flashCharacter::flashCharacter(personSharedPtr p, TCODColor color) :
	xy(p->getPosition()), color(color)
{

}

/*
Changes player's color
*/
drawDataSharedPtr flashCharacter::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
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
drawDataSharedPtr explosion::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
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


bulletPath::bulletPath(coordVectorSharedPtr pts, int tileCode, TCODColor color) :
	tileCode(tileCode), color(color), pts(pts)
{
}

drawDataSharedPtr bulletPath::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
{
	if (atIdx < pts->size()) {
		coord pt = pts->at(atIdx);
		if (pt.first == x && pt.second == y) {
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


glowPath::glowPath(coordVectorSharedPtr pts, TCODColor col1, TCODColor col2) :
	pts(pts)
{
	//Colors fade from col1 to col2
	int idx[] = { 0, pts->size() };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[pts->size()];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawDataSharedPtr glowPath::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
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

drawDataSharedPtr glyphCycle::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
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

drawDataSharedPtr shockwave::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	int dist = hypot(x - this->x, y - this->y);
	if (dist == atIdx + 1) {
		//This is one of our spaces!
		baseData->bgcolor = colors[atIdx];
	}
	return baseData;
}




/*
	Text splash
*/


textSplash::textSplash(int x, int y, std::string txt, TCODColor color) :
	atx(x), aty(y), txt(txt), color(color)
{

}

drawDataSharedPtr textSplash::getDrawData(drawDataSharedPtr baseData, const int x, const int y)
{
	if (y == aty) {
		int idx = x - atx;
		if (idx >= 0 && idx < txt.size()) {
			baseData->tileCode = txt.at(idx);
			baseData->color = color;
		}
	}
	return baseData;
}
