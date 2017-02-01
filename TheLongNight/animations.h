
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <vector>
#include "libtcod.hpp"
#include "window.h"
#include "map.h"
#include "person.h"

typedef std::vector<coord> pathVector;
typedef std::vector<TCODColor> colorVector;


//Draw data - useful little chunk of data that defines what to draw at a point on the map
struct drawData {
	drawData() {}
	drawData(int tileCode, TCODColor color, TCODColor bgcolor) :
		tileCode(tileCode), color(color), bgcolor(bgcolor) {}
	//Copy constructor
	drawData(const drawData &rhs) :
		drawData(rhs.tileCode, rhs.color, rhs.bgcolor) {}
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
	virtual drawData getDrawData(const drawData* baseData, const int x, const int y) { return *baseData; }
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
	virtual drawData getDrawData(const drawData* baseData, const int x, const int y);
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
	explosion(coord ctr, int radius, TCODColor col1, TCODColor col2);
	virtual drawData getDrawData(const drawData* baseData, const int x, const int y);
	virtual void tick();
	virtual bool isDone() { return atPoint > radius; }
protected:
	coord ctr;
	const int radius;
	counter tickDelay;
	int atPoint = 0; 
	TCODColor* colArray;
};


/*
Bullet path effect - traces path with some kind of tile.
*/
class bulletPath : public animations
{
public:
	bulletPath(coordVector pts, int tileCode, TCODColor color);
	virtual drawData getDrawData(const drawData* baseData, const int x, const int y);
	virtual void tick();
	virtual bool isDone() { return atIdx >= pts.size(); }
private:
	int atIdx = 0;
	coordVector pts;
	int tileCode;
	TCODColor color;
};

/*
Bullet path effect, but tiles glow all along the trail.
*/
class glowPath : public animations
{
public:
	glowPath(coordVector pts, TCODColor col1, TCODColor col2);
	virtual drawData getDrawData(const drawData* baseData, const int x, const int y);
	virtual void tick();
	virtual bool isDone() { return atIdx >= pts.size(); }
private:
	int atIdx = 0;
	coordVector pts;
	TCODColor* colors;
};

#endif
