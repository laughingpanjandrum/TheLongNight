
/*
The sole purpose of this whole shenanigan is to keep track of all the different types of items.
Mostly used by the map loader - so that we have a consistent place to track item generation.
*/

#ifndef ITEM_MASTER_H
#define ITEM_MASTER_H

#include "weapon.h"
#include "armour.h"
#include "spell.h"
#include "consumable.h"
#include "miscItem.h"

/*
Giant nightmare function that turns names into items.
Used by the map loader.
*/
item* getItemByHandle(std::string handle);

#endif