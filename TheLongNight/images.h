

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

const imageSharedPtr IMAGE_BATTERED_WOODEN_SHIELD = loadWeaponImage("battered wooden shield");
const imageSharedPtr IMAGE_CROW_HALFSWORD = loadWeaponImage("crow halfsword");
const imageSharedPtr IMAGE_CROW_KNIFE = loadWeaponImage("crow knife");
const imageSharedPtr IMAGE_CROW_KNIGHT_SWORD = loadWeaponImage("crow knight sword");
const imageSharedPtr IMAGE_NOTCHED_GREATSWORD = loadWeaponImage("notched greatsword");
const imageSharedPtr IMAGE_OLD_CROWS_LONG_KNIFE = loadWeaponImage("old crow's long knife");
const imageSharedPtr IMAGE_SPLINTERED_SWORD = loadWeaponImage("splintered sword");
const imageSharedPtr IMAGE_STRAIGHT_SWORD = loadWeaponImage("straight sword");
const imageSharedPtr IMAGE_THIN_KNIFE = loadWeaponImage("thin knife");
const imageSharedPtr IMAGE_WARHAMMER = loadWeaponImage("warhammer");



//Armour

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
