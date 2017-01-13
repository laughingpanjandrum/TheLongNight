#include "itemMaster.h"

item * getItemByHandle(std::string handle)
{
	//WEAPONS
	if (handle == "splintered_sword")
		return weapon_SplinteredSword();
	//SHIELDS
	else if (handle == "battered_wooden_shield")
		return shield_BatteredWoodenShield();
	//WANDS
	else if (handle == "driftwood_wand")
		return wand_DriftwoodWand();
	//CONSUMABLES
	else if (handle == "starwater_draught")
		return consumable_StarwaterDraught();
	else if (handle == "invigorating_tea")
		return consumable_InvigoratingTea();
	//ARMOUR
	else if (handle == "captains_tricorn")
		return headgear_CaptainsTricorn();
	else if (handle == "ruined_uniform")
		return armour_RuinedUniform();
	//Uh oh, we didn't find anything!
	return nullptr;
}
