
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
	person* getTarget() { return target; }
	int getMoveDelay() { return 1; }
	int getAttackDelay() { return 1; }
	//Setters
	void setTarget(person* target) { this->target = target; }
	void clearTarget() { target = nullptr; }
	//Damage and healing
	void takeDamage(int amount);
	void die();
	//Flags
	bool isDead = false;
	bool isPlayer = true;
private:
	//Attributes
	counter health;
	//Current thing we're trying to kill
	person* target;
};

#endif

