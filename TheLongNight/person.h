
#ifndef PERSON_H
#define PERSON_H

#include "element.h"
#include "counter.h"

class person: public element
{
public:
	//Constructors/destructors
	person(): person("Player", PLAYER_TILE, TCODColor::white) {}
	person(std::string name, int tileCode, TCODColor color);
	~person();
	//Getters
	counter getHealth() { return health; }
	//Damage and healing
	void takeDamage(int amount);
	void die();
	//Flags
	bool isDead = false;
private:
	//Attributes
	counter health;
};

#endif

