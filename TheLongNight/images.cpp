#include "images.h"



/*
Creates and returns a pointer to an image.
*/
imageSharedPtr loadImage(const std::string fname)
{
	const std::string realname = fname + IMG_FILE_EXTENSION;
	return imageSharedPtr(new TCODImage(realname.c_str()));
}


/*
Image from the weapons folder.
*/
imageSharedPtr loadWeaponImage(const std::string fname)
{
	return loadImage(WEAPONS_FOLDER + fname);
}


/*
Image from the armour folder.
*/
imageSharedPtr loadArmourImage(const std::string fname)
{
	return loadImage(ARMOUR_FOLDER + fname);
}



