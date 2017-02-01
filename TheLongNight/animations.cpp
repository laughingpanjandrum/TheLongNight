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
drawData* flashCharacter::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	if (x == p->getx() && y == p->gety())
		data->color = color;
	return data;
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
drawData* explosion::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	int dist = hypot(x - ctr.first, y - ctr.second);
	if (dist <= atPoint && atPoint <= radius) {
		data->bgcolor = colArray[atPoint - 1];
	}
	return data;
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

drawData* bulletPath::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	if (atIdx < pts.size()) {
		coord pt = pts.at(atIdx);
		if (pt.first == x && pt.second == y) {
			data->tileCode = tileCode;
			data->color = color;
		}
	}
	return data;
}

void bulletPath::tick()
{
	atIdx++;
}



/*
	GLOW PATH
*/


glowPath::glowPath(coordVector pts, TCODColor col1, TCODColor col2) :
	pts(pts)
{
	//Colors fade from col1 to col2
	int idx[] = { 0, pts.size() };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[pts.size()];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawData* glowPath::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	//Each point up to idx has a different color.
	for (int i = 0; i < atIdx; i++) {
		coord pt = pts.at(i);
		if (pt.first == x && pt.second == y) {
			data->bgcolor = colors[i];
			break;
		}
	}
	return data;
}

void glowPath::tick()
{
	atIdx++;
}


/*
	GLYPH CYCLE
*/


glyphCycle::glyphCycle(coordVector pts, TCODColor col1, TCODColor col2):
	pts(pts)
{
	//Make a colour map for us to cycle through
	int idx[] = { 0, 10 };
	TCODColor cols[] = { col1, col2 };
	colors = new TCODColor[10];
	TCODColor::genMap(colors, 2, cols, idx);
}

drawData* glyphCycle::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	for (auto pt : pts) 
	{
		if (pt.first == x && pt.second == y)
		{
			//Pick a random glyph
			int glyph = randint(128, 175);
			data->tileCode = glyph;
			//Pick a random color
			int idx = randrange(10);
			data->color = colors[idx];
			break;
		}
	}
	//None of our points correspond
	return data;
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

drawData* shockwave::getDrawData(const drawData * baseData, const int x, const int y)
{
	drawData* data = new drawData(*baseData);
	int dist = hypot(x - this->x, y - this->y);
	if (dist == atIdx + 1) {
		//This is one of our spaces!
		data->bgcolor = colors[atIdx];
	}
	return data;
}
