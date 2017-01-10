
#ifndef PERSON_H
#define PERSON_H

#include "element.h"
#include "counter.h"

#include "inventory.h"
#include "categories.h"
#include "weapon.h"

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
	int getMoveDelay();
	int getAttackDelay();
	//Setters
	void setTarget(person* target) { this->target = target; }
	void clearTarget() { target = nullptr; }
	//Damage and healing
	void takeDamage(int amount);
	void die();
	//Equipment
	void equipItem(item* which);
	weapon* getWeapon();
	//Flags
	bool isDead = false;
	bool isPlayer = true;
private:
	//Attributes
	counter health;
	//Equipment
	inventory items;
	//Current thing we're trying to kill
	person* target;
};

#endif

