
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
	ITEM_CONSUMABLE,
	ITEM_SPELL
};

const std::vector<itemTypes> ALL_ITEM_TYPES = { ITEM_WEAPON, ITEM_BODY_ARMOUR, ITEM_HELMET, ITEM_CONSUMABLE };

//Speeds

const int SPEED_SLOW = 4;
const int SPEED_NORMAL = 2;
const int SPEED_FAST = 1;

//Effects

enum effect {
	//Nice effects
	RESTORE_HEALTH, RESTORE_VIGOUR,
	//Weapon effects
	APPLY_BLEED_DAMAGE
};

//Damage types

enum statusEffects {
	EFFECT_BLEED, EFFECT_POISON, EFFECT_PLAGUE
};

//Spell attack types

enum attackType {
	ATTACK_MELEE, ATTACK_RANGE, ATTACK_AOE
};


//Useful types

typedef std::vector<effect> effectVector;

#endif
