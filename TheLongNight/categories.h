
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
	STATE_VIEW_INVENTORY, STATE_VIEW_INVENTORY_CATEGORY
};

//Item types

enum itemTypes {
	ITEM_WEAPON, ITEM_OFFHAND,
	ITEM_BODY_ARMOUR, ITEM_HELMET,
	ITEM_CONSUMABLE,
	ITEM_SPELL
};

const std::vector<itemTypes> ALL_ITEM_TYPES = 
{ ITEM_WEAPON, ITEM_OFFHAND, ITEM_BODY_ARMOUR, ITEM_HELMET, ITEM_CONSUMABLE, ITEM_SPELL };

//Speeds

const int SPEED_SLOW = 4;
const int SPEED_NORMAL = 2;
const int SPEED_FAST = 1;


//Effects

enum effect {
	//Interface effects (for PC only)
	ALLOW_INVENTORY_MANAGEMENT, SET_SAVE_POINT,
	//Nice effects
	FULL_RESTORE, RESTORE_HEALTH, RESTORE_VIGOUR,
	//Damage effects
	APPLY_PHYSICAL_DAMAGE,
	APPLY_BLEED_DAMAGE,
	KILL_CASTER
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
