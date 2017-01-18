
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
	STATE_VIEW_INVENTORY, STATE_VIEW_INVENTORY_CATEGORY,
	STATE_LEVEL_UP_MENU
};

//Equipment scaling types
enum statScaling {
	SCALE_STR, SCALE_DEX, SCALE_ARC, SCALE_DEV
};

//Item types

enum itemTypes {
	ITEM_WEAPON, ITEM_OFFHAND,
	ITEM_BODY_ARMOUR, ITEM_HELMET,
	ITEM_CONSUMABLE,
	ITEM_SPELL,
	ITEM_MISC
};

const std::vector<itemTypes> ALL_ITEM_TYPES = 
{ ITEM_WEAPON, ITEM_OFFHAND, ITEM_BODY_ARMOUR, ITEM_HELMET, ITEM_CONSUMABLE, ITEM_SPELL, ITEM_MISC };

//Speeds

const int SPEED_SLOW = 3;
const int SPEED_NORMAL = 2;
const int SPEED_FAST = 1;


//Effects

enum effect {
	//Interface effects (for PC only)
	ALLOW_INVENTORY_MANAGEMENT, SET_SAVE_POINT, UNLOCK_ADJACENT_DOORS, CHECK_FOR_UNLOCK,
	//Nice effects
	FULL_RESTORE, RESTORE_HEALTH, RESTORE_VIGOUR,
	GAIN_FREE_MOVES,
	//Attack buffs
	SCALE_NEXT_ATTACK,
	//Damage effects
	APPLY_PHYSICAL_DAMAGE,
	APPLY_BLEED_DAMAGE,
	//Other special effects
	KNOCKBACK_TARGET
};

//Damage types

enum statusEffects {
	EFFECT_BLEED, EFFECT_POISON, EFFECT_PLAGUE
};

//Spell attack types

enum attackType {
	ATTACK_MELEE, ATTACK_RANGE, ATTACK_AOE, ATTACK_BUFF_SELF
};


//Useful types

typedef std::vector<effect> effectVector;

#endif
