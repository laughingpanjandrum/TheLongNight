

#ifndef IMAGES_H
#define IMAGES_H

#include <string>
#include <memory>
#include "libtcod.hpp"


const std::string WEAPONS_FOLDER = "img/weapons/";
const std::string ARMOUR_FOLDER = "img/outfits/";
const std::string IMG_FILE_EXTENSION = ".png";


typedef std::shared_ptr<TCODImage> imageSharedPtr;


//Main creator.
imageSharedPtr loadImage(const std::string fname);

//Loads specific types of image from the proper folder.
imageSharedPtr loadWeaponImage(const std::string fname);
imageSharedPtr loadArmourImage(const std::string fname);


/*
	Constants
*/


//Weapons

const imageSharedPtr IMAGE_BATTERED_STEEL_SHIELD = loadWeaponImage("S_battered steel shield");
const imageSharedPtr IMAGE_BATTERED_WOODEN_SHIELD = loadWeaponImage("S_battered wooden shield");
const imageSharedPtr IMAGE_ETHEREAL_SHIELD = loadWeaponImage("S_ethereal shield");
const imageSharedPtr IMAGE_HOLY_SHIELD = loadWeaponImage("S_holy shield");
const imageSharedPtr IMAGE_WYRD_SHIELD = loadWeaponImage("S_wyrd shield");

const imageSharedPtr IMAGE_BLOOD_KNIFE = loadWeaponImage("W_blood knife");
const imageSharedPtr IMAGE_CROW_HALFSWORD = loadWeaponImage("W_crow halfsword");
const imageSharedPtr IMAGE_CROW_KNIFE = loadWeaponImage("W_crow knife");
const imageSharedPtr IMAGE_CROW_KNIGHT_SWORD = loadWeaponImage("W_crow knight sword");
const imageSharedPtr IMAGE_ETHEREAL_KNIFE = loadWeaponImage("W_ethereal knife");
const imageSharedPtr IMAGE_ETHEREAL_SWORD = loadWeaponImage("W_ethereal sword");
const imageSharedPtr IMAGE_FISHMANS_HARPOON = loadWeaponImage("W_fishmans harpoon");
const imageSharedPtr IMAGE_FISHMANS_KNIFE = loadWeaponImage("W_fishmans knife");
const imageSharedPtr IMAGE_HOLY_SCYTHE = loadWeaponImage("W_holy scythe");
const imageSharedPtr IMAGE_HOLY_WARHAMMER = loadWeaponImage("W_holy warhammer");
const imageSharedPtr IMAGE_NOTCHED_GREATSWORD = loadWeaponImage("W_notched greatsword");
const imageSharedPtr IMAGE_OLD_CROWS_LONG_KNIFE = loadWeaponImage("W_old crow's long knife");
const imageSharedPtr IMAGE_SPLINTERED_SWORD = loadWeaponImage("W_splintered sword");
const imageSharedPtr IMAGE_STRAIGHT_SWORD = loadWeaponImage("W_straight sword");
const imageSharedPtr IMAGE_TENTACLE_ARM = loadWeaponImage("W_tentacle arm");
const imageSharedPtr IMAGE_THIN_KNIFE = loadWeaponImage("W_thin knife");
const imageSharedPtr IMAGE_WARHAMMER = loadWeaponImage("W_warhammer");

const imageSharedPtr IMAGE_CRACKED_CHIME = loadWeaponImage("M_cracked chime");
const imageSharedPtr IMAGE_DRIFTWOOD_WAND = loadWeaponImage("M_driftwood wand");
const imageSharedPtr IMAGE_UNHOLY_CHIME = loadWeaponImage("M_unholy chime");
const imageSharedPtr IMAGE_WYRD_BELLBRANCH = loadWeaponImage("M_wyrd bellbranch");



//Armour

const imageSharedPtr IMAGE_CAPTAINS_TRICORN = loadArmourImage("H_captian's uniform");
const imageSharedPtr IMAGE_RUINED_UNIFORM = loadArmourImage("B_captian's uniform");

const imageSharedPtr IMAGE_CITY_GUARD_HELM = loadArmourImage("H_city guard");
const imageSharedPtr IMAGE_CITY_GUARD_ARMOUR = loadArmourImage("B_city guard");

const imageSharedPtr IMAGE_CROW_KNIGHT_HELM = loadArmourImage("H_crow knight");
const imageSharedPtr IMAGE_CROW_KNIGHT_ARMOUR = loadArmourImage("B_crow knight");

const imageSharedPtr IMAGE_CURSED_KNIGHT_HELM = loadArmourImage("H_cursed knight");
const imageSharedPtr IMAGE_CURSED_KNIGHT_ARMOUR = loadArmourImage("B_cursed knight");

const imageSharedPtr IMAGE_PASH_HOOD = loadArmourImage("H_evil priest");
const imageSharedPtr IMAGE_PASH_ROBES = loadArmourImage("B_evil priest");

const imageSharedPtr IMAGE_RUINED_KNIGHT_HELM = loadArmourImage("H_ruined knight");
const imageSharedPtr IMAGE_RUINED_KNIGHT_ARMOUR = loadArmourImage("B_ruined knight");

const imageSharedPtr IMAGE_CLERICS_HOOD = loadArmourImage("H_vestments");
const imageSharedPtr IMAGE_CLERICS_VESTMENTS = loadArmourImage("B_vestments");


#endif
