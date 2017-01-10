
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
	int getBaseMeleeDamage() { return baseMeleeDamage; }
	int getDefence();
	//Setters
	void setTarget(person* target) { this->target = target; }
	void clearTarget() { target = nullptr; }
	//Damage and healing
	void takeDamage(int amount);
	void die();
	//Equipment
	void equipItem(item* which);
	void addItem(item* which);
	weapon* getWeapon();
	armour* getArmour();
	//Flags
	bool isDead = false;
	bool isPlayer = true;
protected:
	//Attributes
	counter health;
	int baseMeleeDamage = 1; //how much damage we do bare-handed
	int baseAttackSpeed = SPEED_NORMAL; //how quickly we attack bare-handed
	int baseMoveSpeed = SPEED_FAST; //how quickly we move if unarmoured
	//Equipment
	inventory items;
	//Current thing we're trying to kill
	person* target;
};

#endif

