
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <vector>
#include <memory>

#include "libtcod.hpp"
#include "window.h"
#include "map.h"
#include "person.h"



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




//For differentiating which type of animation to use
enum animationType {
	ANIMATION_BULLET_PATH, ANIMATION_GLOW_PATH,
	ANIMATION_EXPLOSION, ANIMATION_SHOCKWAVE,
	ANIMATION_FLASH_CHARACTER, ANIMATION_GLYPH_SCRAMBLE,
	ANIMATION_NONE
};



typedef std::shared_ptr<coord> coordSharedPtr;
typedef std::vector<coordSharedPtr> pathVectorP; //Path vector with pointers to coords instead of values
typedef std::vector<coord> pathVector;
typedef std::vector<TCODColor> colorVector;

typedef std::shared_ptr<drawData> drawDataSharedPtr;
typedef std::shared_ptr<coordVector> coordVectorSharedPtr;


//Base class
class animations
{
public:

	//Constructors
	animations() {}
	~animations() {}

	//Progressing and playing
	virtual drawDataSharedPtr getDrawData (drawDataSharedPtr baseData, const int x, const int y) { return baseData; }
	virtual void tick() {}
	virtual bool isDone() { return false; }

};


//Types

typedef std::vector<animations*> animVector;




/*
Briefly changes a person's colour.
*/
class flashCharacter : public animations
{
public:
	flashCharacter(personSharedPtr p, TCODColor color);
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseData, const int x, const int y);
	virtual void tick() { timeLeft--; }
	virtual bool isDone() { return timeLeft < 1; }
protected:
	coord xy;
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
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseData, const int x, const int y);
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
	bulletPath(coordVectorSharedPtr pts, int tileCode, TCODColor color);
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseData, const int x, const int y);
	virtual void tick();
	virtual bool isDone() { return atIdx >= pts->size(); }
private:
	int atIdx = 0;
	coordVectorSharedPtr pts;
	int tileCode;
	TCODColor color;
};

/*
Bullet path effect, but tiles glow all along the trail.
*/
class glowPath : public animations
{
public:
	glowPath(coordVectorSharedPtr pts, TCODColor col1, TCODColor col2);
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseData, const int x, const int y);
	virtual void tick();
	virtual bool isDone() { return atIdx >= pts->size(); }
private:
	int atIdx = 0;
	coordVectorSharedPtr pts;
	TCODColor* colors;
};


/*
Rapidly cycles through random glyphs.
*/
class glyphCycle : public animations
{
public:
	glyphCycle(coordVector* pts, TCODColor col1, TCODColor col2);
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseData, const int x, const int y);
	virtual void tick() { timeLeft--; }
	virtual bool isDone() { return timeLeft < 1; }
private:
	coordVector* pts;
	TCODColor* colors;
	int timeLeft = 20;
};


/*
Like an explosion, but creates a ring instead of a circle.
*/
class shockwave : public animations
{
public:
	shockwave (int x, int y, TCODColor col1, TCODColor col2);
	virtual drawDataSharedPtr getDrawData (drawDataSharedPtr baseData, const int x, const int y);
	virtual void tick() { atIdx++; }
	virtual bool isDone() { return atIdx >= RADIUS; }
private:
	int x, y;
	int atIdx = 0;
	TCODColor* colors;
	const static int RADIUS = 4;
};


/*
Throws up some text on the screen.
*/
class textSplash : public animations
{
public:
	textSplash(int x, int y, std::string txt, TCODColor color);
	virtual drawDataSharedPtr getDrawData(drawDataSharedPtr baseDAta, const int x, const int y);
	virtual void tick() { timePassed++; }
	virtual bool isDone() { return timePassed >= TIME_ON_SCREEN; }
private:
	int atx, aty;
	int timePassed = 0;
	const std::string txt;
	const TCODColor color;
	const static int TIME_ON_SCREEN = 10;
};



#endif
