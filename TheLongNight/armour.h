
#ifndef ARMOUR_H
#define ARMOUR_H

#include "item.h"


class armour : public item
{
public:

	//Constructors
	armour() : armour("", 0, TCODColor::white, ITEM_BODY_ARMOUR, "") {}
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


typedef std::shared_ptr<armour> armourSharedPtr;


/*
	Armour objects
*/

armourSharedPtr headgear_CaptainsTricorn();
armourSharedPtr armour_RuinedUniform();

armourSharedPtr headgear_RuinedKnightsHelm();
armourSharedPtr armour_RuinedKnightsArmour();

armourSharedPtr headgear_ClericsHood();
armourSharedPtr armour_ClericsVestments();

armourSharedPtr headgear_CrowKnightsHood();
armourSharedPtr armour_CrowKnightsArmour();

armourSharedPtr headgear_FishpriestHat();
armourSharedPtr armour_FishscaleCoat();

armourSharedPtr headgear_CityGuardHelm();
armourSharedPtr armour_CityGuardArmour();

armourSharedPtr headgear_PashHood();
armourSharedPtr armour_PashRobes();

armourSharedPtr headgear_CursedKnightsHelm();
armourSharedPtr armour_CursedKnightsArmour();

armourSharedPtr headgear_GreyThiefsHood();
armourSharedPtr armour_GreyThiefsRags();

armourSharedPtr headgear_SilverPlatedHood();
armourSharedPtr armour_SilverPlatedArmour();

armourSharedPtr headgear_EtherealCrown();
armourSharedPtr armour_EtherealRobes();

armourSharedPtr headgear_SparrowKnightsHelm();
armourSharedPtr armour_SparrowKnightsArmour();

armourSharedPtr headgear_MoshkasHeadpiece();
armourSharedPtr armour_MoshkasRobes();

armourSharedPtr headgear_SentinelsHelm();
armourSharedPtr armour_SentinelsArmour();

armourSharedPtr headgear_GhostlyHelm();
armourSharedPtr armour_GhostlyArmour();

armourSharedPtr headgear_SirPercivelsHelm();
armourSharedPtr armour_SirPercivelsArmour();

#endif

