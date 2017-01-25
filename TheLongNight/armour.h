
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
	void setMoveSpeed(int moveSpeed) { this->moveSpeed = moveSpeed; }

	//Getting
	int getMoveSpeed() { return moveSpeed; }

private:

	//Attributes
	int moveSpeed = SPEED_NORMAL;

};

/*
	Armour objects
*/

armour* headgear_CaptainsTricorn();
armour* armour_RuinedUniform();

armour* headgear_RuinedKnightsHelm();
armour* armour_RuinedKnightsArmour();

armour* headgear_ClericsHood();
armour* armour_ClericsVestments();

armour* headgear_CrowKnightsHood();
armour* armour_CrowKnightsArmour();

armour* headgear_FishpriestHat();
armour* armour_FishscaleCoat();


#endif

