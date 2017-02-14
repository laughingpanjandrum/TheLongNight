#include "images.h"



/*
Creates and returns a pointer to an image.
*/
imageSharedPtr loadImage(const std::string fname)
{
	//Get actual file name by appending the extension
	const std::string realname = fname + IMG_FILE_EXTENSION;
	//Create pointer to image
	imageSharedPtr pix = imageSharedPtr(new TCODImage(realname.c_str()));
	//Set the transparency colour to the top-left pixel
	pix->setKeyColor(pix->getPixel(0, 0));
	//Done
	return pix;
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



