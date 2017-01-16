
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <vector>
#include "libtcod.hpp"
#include "window.h"
#include "map.h"
#include "person.h"

typedef std::vector<coord> pathVector;


//Draw data - useful little chunk of data that defines what to draw at a point on the map
struct drawData {
	drawData() {}
	drawData(int tileCode, TCODColor color, TCODColor bgcolor) :
		tileCode(tileCode), color(color), bgcolor(bgcolor) {}
	int tileCode;
	TCODColor color;
	TCODColor bgcolor;
};



//Base class
class animations
{
public:

	//Constructors
	animations() {}
	~animations() {}

	//Progressing and playing
	virtual drawData getDrawData(drawData* baseData, int x, int y) { return *baseData; }
	virtual void tick() {}
	virtual bool isDone() { return false; }

};

typedef std::vector<animations*> animVector;

/*
Briefly changes a person's colour.
*/
class flashCharacter : public animations
{
public:
	flashCharacter(person* p, TCODColor color);
	virtual drawData getDrawData(drawData* baseData, int x, int y);
	virtual void tick() { timeLeft--; }
	virtual bool isDone() { return timeLeft < 1; }
protected:
	person* p;
	const TCODColor color;
	int timeLeft = 3;
};


/*
Explosion effect, starting at centre and expanding outward.
*/
class explosion : public animations
{
public:
	explosion(coord ctr, int radius, TCODColor col1, TCODColor col2) :
		ctr(ctr), radius(radius), col1(col1), col2(col2) {}
	virtual drawData getDrawData(drawData* baseData, int x, int y);
	virtual void tick() { atPoint++; }
	virtual bool isDone() { return atPoint >= radius; }
protected:
	coord ctr;
	int radius;
	int atPoint = 0;
	TCODColor col1;
	TCODColor col2;
};

#endif
