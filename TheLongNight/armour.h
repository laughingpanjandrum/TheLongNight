
#ifndef ARMOUR_H
#define ARMOUR_H

#include "item.h"


class armour : public item
{
public:
	//Constructors
	armour(): item() {}
	armour(std::string name, int tileCode, TCODColor color, itemTypes slot, std::string description);
	~armour();
	//Setting
	void setDefence(int def) { this->defence = def; }
	void setMoveSpeed(int moveSpeed) { this->moveSpeed = moveSpeed; }
	//Getting
	int getDefence() { return defence; }
	int getMoveSpeed() { return moveSpeed; }
private:
	//Attributes
	int defence = 0;
	int moveSpeed = SPEED_NORMAL;
};

/*
	Armour objects
*/

armour* headgear_CaptainsTricorn();
armour* armour_RuinedUniform();

#endif

