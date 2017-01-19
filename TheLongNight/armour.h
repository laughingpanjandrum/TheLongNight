
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
	void setDamageResist(damageType dtype, int def) { damageResist.at(dtype) = def; }
	void setMoveSpeed(int moveSpeed) { this->moveSpeed = moveSpeed; }
	void setBleedResist(int r) { bleedResist = r; }

	//Getting
	int getDefence() { return defence; }
	int getDamageResist(damageType dtype) { return damageResist.at(dtype); }
	int getMoveSpeed() { return moveSpeed; }
	int getBleedResist() { return bleedResist; }

private:

	//Attributes
	int defence = 0;
	int bleedResist = 0;
	int moveSpeed = SPEED_NORMAL;
	std::vector<int> damageResist;

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


#endif

