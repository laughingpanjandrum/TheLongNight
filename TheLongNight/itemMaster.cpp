#include "itemMaster.h"

item * getItemByHandle(std::string handle)
{
	
	//WEAPONS
	if (handle == "splintered_sword")
		return weapon_SplinteredSword();
	else if (handle == "thin_knife")
		return weapon_ThinKnife();
	else if (handle == "straight_sword")
		return weapon_StraightSword();
	else if (handle == "warhammer")
		return weapon_Warhammer();
	else if (handle == "crow_knight_sword")
		return weapon_CrowKnightSword();
	else if (handle == "crow_knife")
		return weapon_CrowKnife();
	else if (handle == "crow_half_sword")
		return weapon_CrowHalfsword();

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
	else if (handle == "crow_knights_hood")
		return headgear_CrowKnightsHood();
	else if (handle == "crow_knights_armour")
		return armour_CrowKnightsArmour();
	
	//SPELLS
	else if (handle == "magic_missile")
		return spell_MagicMissile();
	else if (handle == "arcane_radiance")
		return spell_ArcaneRadiance();
	
	//MISC ITEMS
	else if (handle == "wretched_key")
		return key_WretchsKey();
	
	//Uh oh, we didn't find anything!
	return nullptr;
}
