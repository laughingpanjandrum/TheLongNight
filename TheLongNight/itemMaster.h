
/*
The sole purpose of this whole shenanigan is to keep track of all the different types of items.
Mostly used by the map loader - so that we have a consistent place to track item generation.
*/

#ifndef ITEM_MASTER_H
#define ITEM_MASTER_H

#include "weapon.h"
#include "armour.h"
#include "charm.h"
#include "spell.h"
#include "consumable.h"
#include "miscItem.h"

/*
Giant nightmare function that turns names into items.
Used by the map loader.
*/
itemSharedPtr getItemByHandle(std::string handle);

/*
This is for getting names and other ui data for damage types.
*/

std::string getDamageTypeName(damageType dtype);
TCODColor getDamageTypeColor(damageType dtype);

std::string getStatusEffectName(statusEffects stype);
TCODColor getStatusEffectColor(statusEffects stype);

std::string getEffectName(effect e);



/*
Monolithic list of all items in the game.
This data is required for save games, which refer to each item by its name.
*/
const itemVector getListOfAllItems();

#endif