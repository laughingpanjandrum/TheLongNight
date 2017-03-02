#include "itemMaster.h"

itemSharedPtr getItemByHandle(std::string handle)
{

	/*
	This is multiple if-statements instead of a single block due to compiler limits.
	*/
	
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
	//Winter Court
	else if (handle == "dragonbone_sword")
		return weapon_DragonboneSword();
	//Murdermire
	else if (handle == "serpents_tooth")
		return weapon_SerpentsTooth();
	else if (handle == "crimson_nail")
		return weapon_CrimsonNail();
	//Tear-Stained Valley
	else if (handle == "sacrificial_knife")
		return weapon_SacrificialKnife();
	//Dead Sparrow's Tower
	else if (handle == "ethereal_sword")
		return weapon_EtherealSword();
	//Silent Mausoleum
	else if (handle == "sir_percivels_sword")
		return weapon_SirPercivelsSword();
	//Bridge of Lords/Moonpale Wastes
	else if (handle == "sentinels_pike")
		return weapon_SentinelsPike();
	else if (handle == "ethereal_greatsword")
		return weapon_EtherealGreatsword();
	//Ancenstral Tombs of Farin
	else if (handle == "gravekeeper_spear")
		return weapon_GravekeeperSpear();
	else if (handle == "farins_torch")
		return weapon_FarinsTorch();

	//SHIELDS
	if (handle == "battered_wooden_shield")
		return shield_BatteredWoodenShield();
	else if (handle == "wooden_wyrd_shield")
		return shield_WoodenWyrdShield();
	else if (handle == "battered_steel_shield")
		return shield_BatteredSteelShield();
	else if (handle == "city_guardian_shield")
		return shield_CityGuardianShield();
	else if (handle == "void_touched_shield")
		return shield_VoidTouchedShield();
	else if (handle == "dragonbone_shield")
		return shield_DragonboneShield();
	else if (handle == "ethereal_shield")
		return shield_EtherealShield();
	else if (handle == "khalles_bell_shield")
		return shield_KhallesBellShield();
	else if (handle == "ghostly_shield")
		return shield_GhostlyShield();
	else if (handle == "sir_percivels_shield")
		return shield_SirPercivelsShield();

	//WANDS/CHIMES
	if (handle == "driftwood_wand")
		return wand_DriftwoodWand();
	else if (handle == "fishmans_toadstaff")
		return wand_FishmansToadstaff();
	else if (handle == "ethereal_wand")
		return wand_EtherealWand();
	else if (handle == "gottrics_wand")
		return wand_GottricsWand();
	else if (handle == "clerics_cracked_chime")
		return chime_ClericsCrackedChime();
	else if (handle == "rotblood_bell")
		return chime_RotbloodBell();

	//CONSUMABLES
	if (handle == "starwater_draught")
		return consumable_StarwaterDraught();
	else if (handle == "invigorating_tea")
		return consumable_InvigoratingTea();
	else if (handle == "tiny_red_flower")
		return consumable_TinyRedFlower();
	else if (handle == "tiny_green_flower")
		return consumable_TinyGreenFlower();
	else if (handle == "putrid_flower")
		return consumable_PutridFlower();
	else if (handle == "blood_drinkers_eyes")
		return consumable_BloodDrinkersEyes();
	else if (handle == "intoxicating_wine")
		return consumable_IntoxicatingWine();
	else if (handle == "witchwater_flask")
		return consumable_WitchwaterFlask();
	else if (handle == "black_honey")
		return consumable_BlackHoney();

	//OILS
	if (handle == "pyromancers_oil")
		return oil_PyromancersOil();
	else if (handle == "corrosive_oil")
		return oil_CorrosiveOil();
	else if (handle == "frozen_oil")
		return oil_FrozenOil();
	else if (handle == "holy_water")
		return oil_HolyWater();
	else if (handle == "cursed_water")
		return oil_CursedWater();
	else if (handle == "hangmans_blood")
		return oil_HangmansBlood();
	else if (handle == "witchs_ooze")
		return oil_WitchsOoze();
	else if (handle == "lightning_oil")
		return oil_LightningOil();
	else if (handle == "deep_red_oil")
		return oil_DeepRedOil();
	else if (handle == "rotblood_oil")
		return oil_RotbloodOil();

	//RANGED WEAPONS
	if (handle == "throwing_knives")
		return ranged_ThrowingKnives();
	else if (handle == "lacerating_knives")
		return ranged_LaceratingKnives();
	else if (handle == "poison_throwing_knives")
		return ranged_PoisonThrowingKnives();
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
	else if (handle == "warping_javelins")
		return ranged_WarpingJavelin();

	//ARMOUR
	if (handle == "captains_tricorn")
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

	else if (handle == "grey_thiefs_hood")
		return headgear_GreyThiefsHood();
	else if (handle == "grey_thiefs_rags")
		return armour_GreyThiefsRags();

	else if (handle == "silver_plated_hood")
		return headgear_SilverPlatedHood();
	else if (handle == "silver_plated_armour")
		return armour_SilverPlatedArmour();

	else if (handle == "ethereal_crown")
		return headgear_EtherealCrown();
	else if (handle == "ethereal_robes")
		return armour_EtherealRobes();

	else if (handle == "sparrow_knights_helm")
		return headgear_SparrowKnightsHelm();
	else if (handle == "sparrow_knights_armour")
		return armour_SparrowKnightsArmour();

	else if (handle == "moshkas_headpiece")
		return headgear_MoshkasHeadpiece();
	else if (handle == "moshkas_robes")
		return armour_MoshkasRobes();

	else if (handle == "sentinels_helm")
		return headgear_SentinelsHelm();
	else if (handle == "sentinels_armour")
		return armour_SentinelsArmour();

	else if (handle == "ghostly_helm")
		return headgear_GhostlyHelm();
	else if (handle == "ghostly_armour")
		return armour_GhostlyArmour();

	else if (handle == "sir_percivels_helm")
		return headgear_SirPercivelsHelm();
	else if (handle == "sir_percivels_armour")
		return armour_SirPercivelsArmour();

	else if (handle == "gravekeepers_hood")
		return headgear_GravekeepersHood();
	else if (handle == "gravekeepers_rags")
		return armour_GravekeepersRags();

	else if (handle == "dragonbone_helm")
		return headgear_DragonboneHelm();
	else if (handle == "dragonbone_armour")
		return armour_DragonboneArmour();

	else if (handle == "starweavers_hood")
		return headgear_StarweaversHood();
	else if (handle == "starweavers_robe")
		return armour_StarweaversRobe();

	//CHARMS
	if (handle == "bloodstained_charm")
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
	else if (handle == "toxicants_charm")
		return charm_ToxicantsCharm();
	else if (handle == "obscuring_charm")
		return charm_ObscuringCharm();
	else if (handle == "fragrant_amulet")
		return charm_FragrantAmulet();
	else if (handle == "ravenous_idol")
		return charm_RavenousIdol();
	else if (handle == "venomruby_ring")
		return charm_VenomrubyRing();
	else if (handle == "sir_percivels_ring")
		return charm_SirPercivelsRing();
	else if (handle == "starweavers_ring")
		return charm_StarweaversRing();
	else if (handle == "voidwalkers_ring")
		return charm_VoidwalkersRing();

	//SPELLS
	else if (handle == "magic_missile")
		return spell_MagicMissile();
	else if (handle == "arcane_radiance")
		return spell_ArcaneRadiance();
	else if (handle == "atalundra_arcane_spear")
		return spell_AtalundraArcaneSpear();
	else if (handle == "atalundra_death_surge")
		return spell_AtalundraDeathSurge();
	else if (handle == "atalundra_soul_blade")
		return spell_AtalundraSoulBlade();

	//PRAYERS
	else if (handle == "restoration")
		return prayer_Restoration();
	else if (handle == "astral_prison")
		return prayer_AstralPrison();

	//MISC ITEMS
	if (handle == "green_chapel_garden_key")
		return key_GreenChapelGardenKey();
	else if (handle == "dead_sparrow_key")
		return key_DeadSparrowKey();
	else if (handle == "void_sigil")
		return misc_VoidSigil();
	else if (handle == "sordid_chapel_key")
		return key_SordidChapelKey();
	else if (handle == "voidwalkers_returning_bell")
		return bell_VoidwalkersReturningBell();
	else if (handle == "khalles_bones")
		return bones_KhallesBones();
	else if (handle == "siltras_bones")
		return bones_SiltrasBones();
	else if (handle == "corens_bones")
		return bones_CorensBones();
	else if (handle == "ietras_bones")
		return bones_IetrasBones();

	//HEARTS
	if (handle == "wretched_heart")
		return heart_WretchedHeart();
	else if (handle == "old_crows_heart")
		return heart_OldCrowsHeart();
	else if (handle == "fishborn_heart")
		return heart_FishBornGoddessesHeart();
	else if (handle == "orsyls_heart")
		return heart_OrsylsShriveledHeart();
	else if (handle == "vortens_heart")
		return heart_VortensShriveledHeart();
	else if (handle == "venomous_spider_heart")
		return heart_VenomousSpiderHeart();
	else if (handle == "lady_tverts_heart")
		return heart_LadyTvertsHeart();
	else if (handle == "dead_sparrows_heart")
		return heart_DeadSparrowsHeart();
	else if (handle == "corens_heart")
		return heart_HeartOfCoren();
	else if (handle == "khalles_heart")
		return heart_KhallesWretchedHeart();
	else if (handle == "ietras_heart")
		return heart_IetrasBurningHeart();
	else if (handle == "pale_heart")
		return heart_PaleHeart();

	//SPELLBOOKS
	if (handle == "waterlogged_writings")
		return spellbook_WaterloggedWritings();
	else if (handle == "minas_profaned_writings")
		return spellbook_MinasProfanedWritings();
	else if (handle == "frostbitten_writings")
		return spellbook_FrostbittenWritings();
	else if (handle == "singed_writings")
		return spellbook_SingedWritings();
	else if (handle == "startouched_writings")
		return spellbook_StartouchedWritings();
	else if (handle == "orsyls_tome_of_prayer")
		return spellbook_OrsylsTomeOfPrayer();
	else if (handle == "divine_moonspark_tome")
		return spellbook_DivineMoonsparkTome();
	else if (handle == "divine_tome_of_the_emissary")
		return spellbook_DivineTomeOfTheEmissary();
	else if (handle == "divine_nightmare_tome")
		return spellbook_DivineNightmareTome();
	else if (handle == "tome_of_the_dead")
		return spellbook_TomeOfTheDead();
	else if (handle == "piece_of_jade")
		return misc_PieceOfJade();
	else if (handle == "piece_of_bone")
		return misc_PieceOfBone();

	//GEMSTONES
	if (handle == "dull_grey_gemstone")
		return gem_DullGreyGemstone();
	else if (handle == "small_shining_gemstone")
		return gem_SmallShiningGemstone();
	else if (handle == "noble_glowing_gemstone")
		return gem_NobleGlowingGemstone();
	else if (handle == "kings_giant_gemstone")
		return gem_KingsGiantGemstone();
	else if (handle == "godless_gemstone")
		return gem_GodlessGemstone();

	//RUNESTONES
	if (handle == "corens_runestone")
		return runestone_CorensRunestone();
	else if (handle == "siltras_runestone")
		return runestone_SiltrasRunestone();
	else if (handle == "khalles_runestone")
		return runestone_KhallesRunestone();
	else if (handle == "ietras_runestone")
		return runestone_IetrasRunestone();
	else if (handle == "charred_runestone")
		return runestone_CharredRunestone();
	else if (handle == "kinslayers_runestone")
		return runestone_KinslayersRunestone();
	else if (handle == "starweavers_runestone")
		return runestone_StarweaversRunestone();
	else if (handle == "thundrous_runestone")
		return runestone_ThundrousRunestone();
	else if (handle == "blood_drenched_runestone")
		return runestone_BloodDrenchedRunestone();
	else if (handle == "corroding_runestone")
		return runestone_CorrodingRunestone();
	else if (handle == "frozen_runestone")
		return runestone_FrozenRunestone();
	else if (handle == "venomous_runestone")
		return runestone_VenomousRunestone();
	
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
	case(REMOVE_PLAGUE): return "cures plague";
	case(ADD_HEALTH_TRICKLE): return "health restore, 1/turn";

	case(INCREASE_SPELL_POWER): return "arcane power";
	case(INCREASE_PRAYER_POWER): return "divine power";

	case(SCALE_NEXT_ATTACK): return "% damage next attack";
	case(SCALE_NEXT_SPELL): return "% spell power for next spell";
	case(SCALE_NEXT_PRAYER): return "% divine power for next prayer";
	case(SPELL_ACID_INFUSION): return "acid infusion for next spell";
	case(SPELL_COLD_INFUSION): return "cold infusion for next spell";
	case(COLD_DAMAGE_ADDS_INFUSION): return "cold infusion upon taking cold damage";
	case(INSTANT_SPELL_CAST): return "instant spell cast";
	case(NEXT_SPELL_COST_ADJUST): return "next spell cost";

	case(GAIN_DEFENCE): return "physical defence";
	case(GAIN_ACID_RESIST): return "acid defence";
	case(GAIN_COLD_RESIST): return "cold defence";
	case(GAIN_ELECTRIC_RESIST): return "electric defence";
	case(GAIN_FIRE_RESIST): return "fire defence";
	case(GAIN_MAGIC_RESIST): return "magic defence";
	case(GAIN_STINK_RESIST): return "stench resistance";

	case(CASTER_MELEE_ATTACK): return "free melee attack";
	case(APPLY_UNTYPED_DAMAGE): return "damage";
	case(APPLY_PHYSICAL_DAMAGE): return "physical damage";
	case(APPLY_ACID_DAMAGE): return "acid damage";
	case(APPLY_BLEED_DAMAGE): return "bleed damage";
	case(APPLY_POISON_DAMAGE): return "poison damage";
	case(APPLY_PLAGUE_DAMAGE): return "plague damage";
	case(APPLY_COLD_DAMAGE): return "cold damage";
	case(APPLY_ELECTRIC_DAMAGE): return "electric damage";
	case(APPLY_FIRE_DAMAGE): return "fire damage";
	case(APPLY_MAGIC_DAMAGE): return "magic damage";
	case(APPLY_BLESSED_DAMAGE): return "blessed damage";
	case(APPLY_PROFANE_DAMAGE): return "profane damage";
	case(HURT_CASTER): return "damage to caster";

	case(APPLY_BLINDING): return "blind target";
	case(APPLY_ENTANGLING): return "entangle target";
	case(APPLY_DAMAGE_PENALTY): return "attack damage penalty";
	case(KNOCKBACK_TARGET): return "knockback target";
	case(PULL_CLOSER): return "pull target";
	case(BLEED_DAMAGE_FACTOR): return "times bleed damage dealt and taken";
	case(GAIN_BLEED_SCALING): return "extra damage dealt when bleeding";
	case(DAMAGE_WHEN_POISONED): return "extra damage dealt when poisoned";
	case(PHYS_RESIST_WHILE_POISONED): return "defence when poisoned";
	case(HURT_BLEEDER): return "profane damage dealt to bleeding target";
	case(HEALING_FACTOR): return "percent healing boost";
	case(BECOME_INVISIBLE): return "invisibility";
	case(GAIN_HEALTH_ON_KILL): return "health restored on kill";
	case(CHANGE_FRAGMENT_PICKUP_MULT): return "times fragments gained";
	case(GAIN_DIVINE_RETRIBUTION): return "divine retribution";
	case(CHANGE_DETECTION_RANGE): return "detection range";
	case(LOW_HEALTH_DAMAGE_BUFF): return "damage buff when health is low";
	case(GAIN_KILL_DAMAGE_BUFF): return "damage buff upon kill";
	case(INSTILL_FEAR): return "instill fear";
	case(BUFF_HOLY_DAMAGE): return "% holy damage dealt";
	case(DEATHFIRE_INFUSION): return "fire explosion on death";
	case(DEATHLINK): return "upon death, return to life";
	case(SILENCE): return "silence";
	case(DROP_WEB): return "create spiderweb";
	case(ASTRAL_IMPRISONMENT): return "astral imprisonment";
	case(ACQUIRE_FRAGMENTS): return "acquire fragments";

	case(TELEPORT_VIA_WATER): return "water warp";
	case(TELEPORT_TO_VOID): return "warp to void";
	case(TELEPORT_BACK_FROM_VOID): return "escape void";
	case(IMMUNE_TO_VOID): return "walk upon the void";

	case(CREATE_FOG): return "radius fog cloud";

	}

	return "Unknown";
}
