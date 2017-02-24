#include "images.h"



/*
Creates and returns a pointer to an image.
*/
imagePtr loadImage(const std::string fname)
{
	//Get actual file name by appending the extension
	const std::string realname = fname + IMG_FILE_EXTENSION;
	//Create pointer to image
	imagePtr pix = new TCODImage(realname.c_str());
	//Set the transparency colour to the top-left pixel
	pix->setKeyColor(pix->getPixel(0, 0));
	//Done
	return pix;
}


/*
Image from the weapons folder.
*/
imagePtr loadWeaponImage(const std::string fname)
{
	return loadImage(WEAPONS_FOLDER + fname);
}


/*
Image from the armour folder.
*/
imagePtr loadArmourImage(const std::string fname)
{
	return loadImage(ARMOUR_FOLDER + fname);
}



