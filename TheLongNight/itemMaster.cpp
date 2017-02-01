#include "itemMaster.h"

item * getItemByHandle(std::string handle)
{
	
	//	WEAPONS
	//Starters
	if (handle == "splintered_sword")
		return weapon_SplinteredSword();
	else if (handle == "thin_knife")
		return weapon_ThinKnife();
	else if (handle == "straight_sword")
		return weapon_StraightSword();
	else if (handle == "warhammer")
		return weapon_Warhammer();
	//Pilgrim's Road
	else if (handle == "crow_knight_sword")
		return weapon_CrowKnightSword();
	else if (handle == "crow_knife")
		return weapon_CrowKnife();
	else if (handle == "crow_half_sword")
		return weapon_CrowHalfsword();
	//Flooded Lowlands
	else if (handle == "notched_greatsword")
		return weapon_NotchedGreatsword();
	else if (handle == "fishmans_harpoon")
		return weapon_FishmansHarpoon();
	else if (handle == "fishmans_knife")
		return weapon_FishmansKnife();
	//Crumbling City
	else if (handle == "city_guardian_warhammer")
		return weapon_CityGuardianWarhammer();
	else if (handle == "blood_drinkers_knife")
		return weapon_BloodDrinkersKnife();
	else if (handle == "kythiels_scythe")
		return weapon_KythielsScythe();
	else if (handle == "void_touched_knife")
		return weapon_VoidTouchedKnife();

	//SHIELDS
	else if (handle == "battered_wooden_shield")
		return shield_BatteredWoodenShield();
	else if (handle == "wooden_wyrd_shield")
		return shield_WoodenWyrdShield();
	else if (handle == "battered_steel_shield")
		return shield_BatteredSteelShield();
	else if (handle == "city_guardian_shield")
		return shield_CityGuardianShield();
	else if (handle == "void_touched_shield")
		return shield_VoidTouchedShield();

	//WANDS/CHIMES
	else if (handle == "driftwood_wand")
		return wand_DriftwoodWand();
	else if (handle == "fishmans_toadstaff")
		return wand_FishmansToadstaff();
	else if (handle == "clerics_cracked_chime")
		return chime_ClericsCrackedChime();

	//CONSUMABLES
	else if (handle == "starwater_draught")
		return consumable_StarwaterDraught();
	else if (handle == "invigorating_tea")
		return consumable_InvigoratingTea();
	else if (handle == "tiny_red_flower")
		return consumable_TinyRedFlower();
	else if (handle == "tiny_green_flower")
		return consumable_TinyGreenFlower();
	else if (handle == "blood_drinkers_eyes")
		return consumable_BloodDrinkersEyes();

	//OILS
	else if (handle == "pyromancers_oil")
		return oil_PyromancersOil();
	else if (handle == "corrosive_oil")
		return oil_CorrosiveOil();

	//RANGED WEAPONS
	else if (handle == "throwing_knives")
		return ranged_ThrowingKnives();
	else if (handle == "lacerating_knives")
		return ranged_LaceratingKnives();
	else if (handle == "heavy_javelin")
		return ranged_HeavyJavelin();
	else if (handle == "corroding_jar")
		return ranged_CorrodingJar();
	else if (handle == "void_essence_jar")
		return ranged_VoidEssenceJar();
	else if (handle == "pyromancers_flask")
		return ranged_PyromancersFlask();
	else if (handle == "witchs_jar")
		return ranged_WitchsJar();
	else if (handle == "lightning_javelins")
		return ranged_LightingJavelin();
	else if (handle == "frost_knives")
		return ranged_FrostKnives();

	//ARMOUR
	else if (handle == "captains_tricorn")
		return headgear_CaptainsTricorn();
	else if (handle == "ruined_uniform")
		return armour_RuinedUniform();

	else if (handle == "clerics_hood")
		return headgear_ClericsHood();
	else if (handle == "clerics_vestments")
		return armour_ClericsVestments();

	else if (handle == "ruined_knights_helm")
		return headgear_RuinedKnightsHelm();
	else if (handle == "ruined_knights_armour")
		return armour_RuinedKnightsArmour();

	else if (handle == "crow_knights_hood")
		return headgear_CrowKnightsHood();
	else if (handle == "crow_knights_armour")
		return armour_CrowKnightsArmour();

	else if (handle == "fishpriest_hat")
		return headgear_FishpriestHat();
	else if (handle == "fishscale_coat")
		return armour_FishscaleCoat();

	else if (handle == "city_guard_helm")
		return headgear_CityGuardHelm();
	else if (handle == "city_guard_armour")
		return armour_CityGuardArmour();

	else if (handle == "pash_hood")
		return headgear_PashHood();
	else if (handle == "pash_robes")
		return armour_PashRobes();

	else if (handle == "cursed_knights_helm")
		return headgear_CursedKnightsHelm();
	else if (handle == "cursed_knights_armour")
		return armour_CursedKnightsArmour();

	//CHARMS
	else if (handle == "bloodstained_charm")
		return charm_BloodstainedCharm();
	else if (handle == "khalles_headband")
		return charm_KhallesHeadband();
	else if (handle == "idol_of_pash")
		return charm_IdolOfPash();
	else if (handle == "eviscerating_ring")
		return charm_EvisceratingRing();
	else if (handle == "blood_drinkers_band")
		return charm_BloodDrinkersBand();
	else if (handle == "frozen_flower_charm")
		return charm_FrozenFlowerCharm();

	//SPELLS
	else if (handle == "magic_missile")
		return spell_MagicMissile();
	else if (handle == "arcane_radiance")
		return spell_ArcaneRadiance();

	//PRAYERS
	else if (handle == "restoration")
		return prayer_Restoration();

	//MISC ITEMS
	else if (handle == "green_chapel_garden_key")
		return key_GreenChapelGardenKey();
	else if (handle == "void_sigil")
		return misc_VoidSigil();
	else if (handle == "sordid_chapel_key")
		return key_SordidChapelKey();
	else if (handle == "voidwalkers_returning_bell")
		return bell_VoidwalkersReturningBell();

	//SPELLBOOKS
	else if (handle == "waterlogged_writings")
		return spellbook_WaterloggedWritings();
	else if (handle == "minas_profaned_writings")
		return spellbook_MinasProfanedWritings();
	else if (handle == "orsyls_tome_of_prayer")
		return spellbook_OrsylsTomeOfPrayer();
	else if (handle == "piece_of_jade")
		return misc_PieceOfJade();

	//RUNESTONES
	else if (handle == "corens_runestone")
		return runestone_CorensRunestone();
	else if (handle == "siltras_runestone")
		return runestone_SiltrasRunestone();
	else if (handle == "khalles_runestone")
		return runestone_KhallesRunestone();
	
	//Uh oh, we didn't find anything!
	return nullptr;
}



/*
Damage type categories
*/


std::string getDamageTypeName(damageType dtype)
{
	switch (dtype) {
	case(DAMAGE_MAGIC): return "Magical";
	case(DAMAGE_ACID): return "Acid";
	case(DAMAGE_COLD): return "Cold";
	case(DAMAGE_ELECTRIC): return "Electric";
	case(DAMAGE_FIRE): return "Fire";
	case(DAMAGE_PROFANE): return "Profane";
	case(DAMAGE_BLESSED): return "Blessed";
	}
	return "Physical";
}

TCODColor getDamageTypeColor(damageType dtype)
{
	switch (dtype) {
	case(DAMAGE_MAGIC): return TCODColor::magenta;
	case(DAMAGE_ACID): return TCODColor::green;
	case(DAMAGE_COLD): return TCODColor::cyan;
	case(DAMAGE_ELECTRIC): return TCODColor::lightPurple;
	case(DAMAGE_FIRE): return TCODColor::flame;
	case(DAMAGE_PROFANE): return TCODColor::darkPurple;
	case(DAMAGE_BLESSED): return TCODColor::darkYellow;
	}
	return TCODColor::white;
}

std::string getStatusEffectName(statusEffects stype)
{
	switch (stype) {
	case(EFFECT_BLEED): return "Bleed";
	case(EFFECT_POISON): return "Poison";
	case(EFFECT_PLAGUE): return "Plague";
	}
	return "Unknown";
}

TCODColor getStatusEffectColor(statusEffects stype)
{
	switch (stype) {
	case(EFFECT_BLEED): return TCODColor::crimson;
	case(EFFECT_POISON): return TCODColor::lime;
	case(EFFECT_PLAGUE): return TCODColor::amber;
	}
	return TCODColor::white;
}

/*
Names of effects.
*/
std::string getEffectName(effect e)
{
	switch (e) {

	case(FULL_RESTORE): return "full restore";
	case(RESTORE_HEALTH): return "health regain";
	case(RESTORE_VIGOUR): return "vigour regain";
	case(GAIN_FREE_MOVES): return "free moves";
	case(GAIN_MAX_HEALTH): return "max health";

	case(REMOVE_BLEED): return "cures bleed buildup";
	case(REMOVE_POISON): return "cures poisoning";
	case(ADD_HEALTH_TRICKLE): return "health restore, 1/turn";

	case(SCALE_NEXT_ATTACK): return "% damage next attack";
	case(SCALE_NEXT_SPELL): return "% spell power for next spell";
	case(SCALE_NEXT_PRAYER): return "% divine power for next prayer";
	case(SPELL_ACID_INFUSION): return "acid infusion for next spell";
	case(SPELL_COLD_INFUSION): return "cold infusion for next spell";
	case(COLD_DAMAGE_ADDS_INFUSION): return "cold infusion upon taking cold damage";

	case(GAIN_DEFENCE): return "physical defence";
	case(GAIN_ACID_RESIST): return "acid defence";
	case(GAIN_COLD_RESIST): return "cold defence";
	case(GAIN_ELECTRIC_RESIST): return "electric defence";
	case(GAIN_FIRE_RESIST): return "fire defence";
	case(GAIN_MAGIC_RESIST): return "magic defence";

	case(CASTER_MELEE_ATTACK): return "free melee attack";
	case(APPLY_PHYSICAL_DAMAGE): return "physical damage";
	case(APPLY_ACID_DAMAGE): return "acid damage";
	case(APPLY_BLEED_DAMAGE): return "bleed damage";
	case(APPLY_COLD_DAMAGE): return "cold damage";
	case(APPLY_ELECTRIC_DAMAGE): return "electric damage";
	case(APPLY_FIRE_DAMAGE): return "fire damage";
	case(APPLY_MAGIC_DAMAGE): return "magic damage";
	case(APPLY_BLESSED_DAMAGE): return "blessed damage";
	case(APPLY_PROFANE_DAMAGE): return "profane damage";

	case(APPLY_BLINDING): return "blind target";
	case(APPLY_ENTANGLING): return "entangle target";
	case(KNOCKBACK_TARGET): return "knockback target";
	case(PULL_CLOSER): return "pull target";
	case(BLEED_DAMAGE_FACTOR): return "times bleed damage dealt and taken";
	case(GAIN_BLEED_SCALING): return "extra damage dealt when bleeding";
	case(HURT_BLEEDER): return "profane damage dealt to bleeding target";
	case(HEALING_FACTOR): return "percent healing boost";
	case(BECOME_INVISIBLE): return "invisibility";
	case(GAIN_HEALTH_ON_KILL): return "health restored on kill";
	case(CHANGE_FRAGMENT_PICKUP_MULT): return "times fragments gained";

	case(TELEPORT_VIA_WATER): return "water warp";
	case(TELEPORT_TO_VOID): return "warp to void";
	case(TELEPORT_BACK_FROM_VOID): return "escape void";

	}

	return "Unknown";
}
