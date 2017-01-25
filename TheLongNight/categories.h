
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
	STATE_LEVEL_UP_MENU, STATE_SHOP_MENU,
	STATE_SELECT_SPELL, STATE_SELECT_CONSUMABLE,
	STATE_WARP
};

//Equipment scaling types

enum statScaling {
	SCALE_STR, SCALE_DEX, SCALE_ARC, SCALE_DEV,
	SCALE_NONE
};

typedef std::vector<statScaling> statScaleVector;

//Item types

enum itemTypes {
	ITEM_WEAPON, ITEM_OFFHAND,
	ITEM_BODY_ARMOUR, ITEM_HELMET,
	ITEM_CHARM,
	ITEM_CONSUMABLE,
	ITEM_SPELL,
	ITEM_MISC
};

const std::vector<itemTypes> ALL_ITEM_TYPES = 
{ ITEM_WEAPON, ITEM_OFFHAND, ITEM_BODY_ARMOUR, ITEM_HELMET, ITEM_CHARM, ITEM_CONSUMABLE, ITEM_SPELL, ITEM_MISC };

//Speeds

const int SPEED_SLOW = 3;
const int SPEED_NORMAL = 2;
const int SPEED_FAST = 1;


//Effects

enum effect {
	NULL_EFFECT,
	//Interface effects (for PC only)
	ALLOW_INVENTORY_MANAGEMENT, SET_SAVE_POINT, UNLOCK_ADJACENT_DOORS, CHECK_FOR_UNLOCK, DO_WARP,
	//Nice effects
	FULL_RESTORE, RESTORE_HEALTH, RESTORE_VIGOUR,
	GAIN_FREE_MOVES, GAIN_MULTIPLE_ATTACKS, ADD_HEALTH_TRICKLE,
	//Attack buffs
	SCALE_NEXT_ATTACK, SCALE_NEXT_SPELL, SCALE_NEXT_PRAYER,
	SPELL_ACID_INFUSION,
	//Defensive buffs
	GAIN_DEFENCE, GAIN_MAGIC_RESIST,
	GAIN_ACID_RESIST, GAIN_FIRE_RESIST, GAIN_COLD_RESIST, GAIN_ELECTRIC_RESIST,
	REMOVE_BLEED,
	//Damage effects
	CASTER_MELEE_ATTACK,
	APPLY_PHYSICAL_DAMAGE, APPLY_MAGIC_DAMAGE,
	APPLY_ACID_DAMAGE, APPLY_FIRE_DAMAGE, APPLY_COLD_DAMAGE, APPLY_ELECTRIC_DAMAGE,
	APPLY_BLEED_DAMAGE,
	//Other special effects
	KNOCKBACK_TARGET, PULL_CLOSER,
	TELEPORT_VIA_WATER
};

//Damage types

enum statusEffects {
	EFFECT_BLEED, EFFECT_POISON, EFFECT_PLAGUE
};

enum damageType {
	DAMAGE_MAGIC = 0,
	DAMAGE_ACID = 1, DAMAGE_ELECTRIC = 2, DAMAGE_FIRE = 3, DAMAGE_COLD = 4,
	DAMAGE_PROFANE = 5, DAMAGE_BLESSED = 6,
	ALL_DAMAGE_TYPES = 7,
	//Secret additional damage types!
	DAMAGE_PHYSICAL = 8, DAMAGE_UNTYPED = 9
};

//Spell attack types

enum attackType {
	ATTACK_MELEE, ATTACK_RANGE, ATTACK_AOE, ATTACK_BUFF_SELF, ATTACK_BUFF_WEAPON
};


//Useful types

typedef std::vector<effect> effectVector;


struct weaponBuff {
	weaponBuff() {}
	weaponBuff(damageType dtype, int bonusDamage) : dtype(dtype), bonusDamage(bonusDamage) {}
	damageType dtype;
	int bonusDamage = 0;
};

#endif
