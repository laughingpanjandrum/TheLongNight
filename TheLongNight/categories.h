
/*
This file defines types of things.
i.e. all item types/monster types should be kept here.
*/

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <vector>

//Game states

enum gameState {
	STATE_VIEW_MAP,
	STATE_VIEW_INVENTORY
};

//Item types

enum itemTypes {
	ITEM_WEAPON, 
	ITEM_BODY_ARMOUR, ITEM_HELMET,
	ITEM_CONSUMABLE
};

const std::vector<itemTypes> ALL_ITEM_TYPES = { ITEM_WEAPON, ITEM_BODY_ARMOUR, ITEM_HELMET, ITEM_CONSUMABLE };

//Speeds

const int SPEED_SLOW = 4;
const int SPEED_NORMAL = 2;
const int SPEED_FAST = 1;

//Effects

enum effect {
	RESTORE_HEALTH, RESTORE_VIGOUR
};

//Damage types

enum statusEffects {
	EFFECT_BLEED, EFFECT_POISON, EFFECT_PLAGUE
};

#endif
