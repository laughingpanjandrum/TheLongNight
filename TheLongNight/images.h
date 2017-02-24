

#ifndef IMAGES_H
#define IMAGES_H

#include <string>
#include <memory>
#include "libtcod.hpp"


const std::string WEAPONS_FOLDER = "img/weapons/";
const std::string ARMOUR_FOLDER = "img/outfits/";
const std::string IMG_FILE_EXTENSION = ".png";


typedef TCODImage* imagePtr;
typedef std::shared_ptr<TCODImage> imageSharedPtr;


//Main creator.
imagePtr loadImage(const std::string fname);

//Loads specific types of image from the proper folder.
imagePtr loadWeaponImage(const std::string fname);
imagePtr loadArmourImage(const std::string fname);


/*
	Constants
*/


//Weapons

const imagePtr IMAGE_BATTERED_STEEL_SHIELD = loadWeaponImage("S_battered steel shield");
const imagePtr IMAGE_BATTERED_WOODEN_SHIELD = loadWeaponImage("S_battered wooden shield");
const imagePtr IMAGE_DRAGONBONE_SHIELD = loadWeaponImage("S_dragonbone_shield");
const imagePtr IMAGE_ETHEREAL_SHIELD = loadWeaponImage("S_ethereal shield");
const imagePtr IMAGE_GREEN_KNIGHTS_SHIELD = loadWeaponImage("S_green knight shield");
const imagePtr IMAGE_HOLY_SHIELD = loadWeaponImage("S_holy shield");
const imagePtr IMAGE_WYRD_SHIELD = loadWeaponImage("S_wyrd shield");

const imagePtr IMAGE_BLOOD_KNIFE = loadWeaponImage("W_blood knife");
const imagePtr IMAGE_BLOODY_GREATAXE = loadWeaponImage("W_bloody greataxe");
const imagePtr IMAGE_CRIMSON_NAIL = loadWeaponImage("W_crimson nail");
const imagePtr IMAGE_CROW_HALFSWORD = loadWeaponImage("W_crow halfsword");
const imagePtr IMAGE_CROW_KNIFE = loadWeaponImage("W_crow knife");
const imagePtr IMAGE_CROW_KNIGHT_SWORD = loadWeaponImage("W_crow knight sword");
const imagePtr IMAGE_DRAGONBONE_SWORD = loadWeaponImage("W_dragonbone sword");
const imagePtr IMAGE_ENERGY_SWORD = loadWeaponImage("W_energy sword");
const imagePtr IMAGE_ETHEREAL_KNIFE = loadWeaponImage("W_ethereal knife");
const imagePtr IMAGE_ETHEREAL_SWORD = loadWeaponImage("W_ethereal sword");
const imagePtr IMAGE_FISHMANS_HARPOON = loadWeaponImage("W_fishmans harpoon");
const imagePtr IMAGE_FISHMANS_KNIFE = loadWeaponImage("W_fishmans knife");
const imagePtr IMAGE_GREEN_KNIGHT_SWORD = loadWeaponImage("W_green knight sword");
const imagePtr IMAGE_HOLY_SCYTHE = loadWeaponImage("W_holy scythe");
const imagePtr IMAGE_HOLY_WARHAMMER = loadWeaponImage("W_holy warhammer");
const imagePtr IMAGE_LIGHTNING_WARHAMMER = loadWeaponImage("W_lightning warhammer");
const imagePtr IMAGE_NOTCHED_GREATSWORD = loadWeaponImage("W_notched greatsword");
const imagePtr IMAGE_OLD_CROWS_LONG_KNIFE = loadWeaponImage("W_old crow's long knife");
const imagePtr IMAGE_POISONOUS_CLAWS = loadWeaponImage("W_poisonous claws");
const imagePtr IMAGE_PROFANE_GREATSWORD = loadWeaponImage("W_profane greatsword");
const imagePtr IMAGE_PROFANE_SCYTHE = loadWeaponImage("W_iteras scythe");
const imagePtr IMAGE_SACRIFICAL_KNIFE = loadWeaponImage("W_sacrificial knife");
const imagePtr IMAGE_SENTINEL_SPEAR = loadWeaponImage("W_sentinelspear");
const imagePtr IMAGE_SERPENTS_TOOTH = loadWeaponImage("W_serpent tooth");
const imagePtr IMAGE_SLAVEMASTER_WHIP = loadWeaponImage("W_slave master whip");
const imagePtr IMAGE_SPIDERBONE_SHARD = loadWeaponImage("W_spiderbone shard");
const imagePtr IMAGE_SPLINTERED_SWORD = loadWeaponImage("W_splintered sword");
const imagePtr IMAGE_STRAIGHT_SWORD = loadWeaponImage("W_straight sword");
const imagePtr IMAGE_SUPER_HOLY_SCYTHE = loadWeaponImage("W_super holy scythe");
const imagePtr IMAGE_TENTACLE_ARM = loadWeaponImage("W_tentacle arm");
const imagePtr IMAGE_THIN_KNIFE = loadWeaponImage("W_thin knife");
const imagePtr IMAGE_WARHAMMER = loadWeaponImage("W_warhammer");
const imagePtr IMAGE_VOIDFIRE_KNIFE = loadWeaponImage("W_voidfire knife");

const imagePtr IMAGE_CRACKED_CHIME = loadWeaponImage("M_cracked chime");
const imagePtr IMAGE_DRIFTWOOD_WAND = loadWeaponImage("M_driftwood wand");
const imagePtr IMAGE_UNHOLY_CHIME = loadWeaponImage("M_unholy chime");
const imagePtr IMAGE_WYRD_BELLBRANCH = loadWeaponImage("M_wyrd bellbranch");



//Armour

const imagePtr IMAGE_CAPTAINS_TRICORN = loadArmourImage("H_captian's uniform");
const imagePtr IMAGE_RUINED_UNIFORM = loadArmourImage("B_captian's uniform");

const imagePtr IMAGE_CITY_GUARD_HELM = loadArmourImage("H_city guard");
const imagePtr IMAGE_CITY_GUARD_ARMOUR = loadArmourImage("B_city guard");

const imagePtr IMAGE_CROW_KNIGHT_HELM = loadArmourImage("H_crow knight");
const imagePtr IMAGE_CROW_KNIGHT_ARMOUR = loadArmourImage("B_crow knight");

const imagePtr IMAGE_CURSED_KNIGHT_HELM = loadArmourImage("H_cursed knight");
const imagePtr IMAGE_CURSED_KNIGHT_ARMOUR = loadArmourImage("B_cursed knight");

const imagePtr IMAGE_PASH_HOOD = loadArmourImage("H_evil priest");
const imagePtr IMAGE_PASH_ROBES = loadArmourImage("B_evil priest");

const imagePtr IMAGE_RUINED_KNIGHT_HELM = loadArmourImage("H_ruined knight");
const imagePtr IMAGE_RUINED_KNIGHT_ARMOUR = loadArmourImage("B_ruined knight");

const imagePtr IMAGE_CLERICS_HOOD = loadArmourImage("H_vestments");
const imagePtr IMAGE_CLERICS_VESTMENTS = loadArmourImage("B_vestments");


#endif
