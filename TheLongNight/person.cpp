#include "person.h"

/*
Constructor you should probably use.
Also sets a default health value, which you'll probably want to change!
*/
person::person(std::string name, int tileCode, TCODColor color) : element(name, tileCode, color)
{
	//Default values for stats
	health.setTo(100);
	vigour.setTo(10);
	//Status effects
	bleedBuildup.setTo(20, 0);
	//Resistances
	for (int r = 0; r < ALL_DAMAGE_TYPES; r++)
		damageResist.push_back(0);
}

person::~person()
{
}




/*
Returns how much extra damage the given weapon does, based on our stats.
The total bonus is a percentage of the weapon's base damage.
*/
int person::getScalingDamage(weapon * wp)
{
	int total = 0;
	//Bonus from each stat
	if (stats != nullptr) {
		//Value of each scaling type
		int str = wp->getScalingDamage(SCALE_STR);
		int dex = wp->getScalingDamage(SCALE_DEX);
		int arc = wp->getScalingDamage(SCALE_ARC);
		int dev = wp->getScalingDamage(SCALE_DEV);
		//Sum up bonus damage
		total += str * stats->strength;
		total += dex * stats->dexterity;
		total += arc * stats->arcana;
		total += dev * stats->devotion;
	}
	return total;
}

/*
	GETTING ATTRIBUTES
*/

/*
How long our movement takes.
Based on our equipped body armour.
*/
int person::getMoveDelay()
{
	armour* a = getArmour();
	if (a != nullptr)
		return a->getMoveSpeed();
	return baseMoveSpeed;
}

/*
How long a melee attack takes.
Based on our equipped weapon.
*/
int person::getAttackDelay()
{
	weapon* wp = getWeapon();
	if (wp != nullptr)
		return wp->getAttackDelay();
	return baseAttackSpeed;
}

/*
Returns reduction to physical damage as an int out of 100%.
*/
int person::getDefence()
{
	//Base reduction
	int base = baseDefence;
	//Bonus from armour
	armour* ar = getArmour();
	if (ar != nullptr)
		base += ar->getDefence();
	//Bonus from headgear
	armour* helm = getHelmet();
	if (helm != nullptr)
		base += helm->getDefence();
	//Bonus from offhand item
	weapon* offhand = getOffhand();
	if (offhand != nullptr)
		base += offhand->getDefence();
	//Done!
	return base;
}

/*
	DAMAGE AND HEALING
*/

void person::addHealth(int amount)
{
	health.increase(amount);
}

/*
Standard damage. Kills us if we run out of health.
*/
void person::takeDamage(int amount, damageType dtype)
{
	//Armour reduces damage
	int resist = 0;
	if (dtype == DAMAGE_PHYSICAL)
		resist = getDefence();
	else if (dtype != DAMAGE_UNTYPED)
		resist = getDamageResist(dtype);
	int def = ((float)resist / 100) * (float)amount;
	amount -= def;
	//Minimum 1 damage
	if (amount < 1)
		amount = 1;
	//Take the rest
	health.decrease(amount);
	if (health.isEmpty())
		die();
}

/*
Applies special effects, e.g. bleed, poison.
*/
void person::takeStatusEffectDamage(statusEffects eType, int damage)
{
	if (eType == EFFECT_BLEED) {
		bleedBuildup.increase(damage);
		if (bleedBuildup.isFull()) {
			//We BLEED!
			bleedBuildup.clear();
			isBleeding += 2;
		}
	}
}

/*
Fully restores all of our attributes and items.
*/
void person::fullRestore()
{
	//Remove buffs
	clearBuffs();
	//Replenish health
	health.restore();
	vigour.restore();
	//Remove status effects
	bleedBuildup.clear();
	isBleeding = 0;
	//Replenish items
	restoreItemsToMax();
}

/*
We DIE.
The game is in charge of making sure we get removed from the map.
*/
void person::die()
{
	isDead = true;
}

/*
Returns counter for the designation damage type.
Returns an empty counter if, for some reason, no result is found.
*/
counter* person::getSpecialEffectBuildup(statusEffects eff)
{
	switch (eff) {
	case(EFFECT_BLEED): return &bleedBuildup;
	}
	return new counter();
}



/*
	PERMANENT BUFFS
*/


/*
We can't add a new buff if we already have one with the same name.
*/
bool person::canAddBuff(buff* b)
{
	for (auto b2 : buffs) {
		if (b2->name == b->name)
			return false;
	}
	return true;
}

/*
Creates and adds a buff.
This one first makes sure that we CAN have the buff, then adds it.
Returns whether the buff was properly added.
*/
bool person::addBuff(std::string name, TCODColor color, effect eType, int potency)
{
	buff* b = new buff(name, color, eType, potency);
	if (canAddBuff(b)) {
		addBuff(b);
		return true;
	}
	return false;
}

/*
Deletes a specific buff ELEMENT
This does not actually clear the buff!
*/
void person::removeBuff(buff* b)
{
	auto iter = std::find(buffs.begin(), buffs.end(), b);
	if (iter != buffs.end())
		buffs.erase(iter);
}



/*
	MAGIC
*/


/*
Forget a spell we know (e.g. because we un-equipped a wand)
*/
void person::removeSpellKnown(spell * sp)
{
	auto iter = std::find(spellsKnown.begin(), spellsKnown.end(), sp);
	if (iter != spellsKnown.end())
		spellsKnown.erase(iter);
}

/*
Returns the spell we currently have active.
*/
spell * person::getCurrentSpell()
{
	if (spellsKnown.size() > 0)
		return spellsKnown.at(selectedSpell);
	return nullptr;
}

/*
We're given a spell, figure out which index we should be at
*/
void person::setCurrentSpell(spell * sp)
{
	while (spellsKnown.at(selectedSpell) != sp)
		cycleSelectedSpell();
}

/*
Cycle to the next spell
*/
void person::cycleSelectedSpell()
{
	selectedSpell++;
	if (selectedSpell >= spellsKnown.size())
		selectedSpell = 0;
}

/*
Effective potency of most spells.
If we have two equipped items that have separate spellpower ratings, we use the higher of the two.
*/
int person::getSpellPower()
{
	weapon* wp = getWeapon();
	weapon* offhand = getOffhand();
	int spellPower = baseSpellPower;

	//From weapon
	if (wp != nullptr)
		spellPower = wp->getSpellPower();

	//See if offhand is higher
	if (offhand != nullptr)
		if (offhand->getSpellPower() > spellPower)
			spellPower = offhand->getSpellPower();

	//Bonus from arcane scaling (if we use stats)
	if (stats != nullptr)
		spellPower += (stats->arcana - 1) * 10;

	//Done
	return spellPower;
}


/*
Potency of prayers.
Scales with Devotion.
*/
int person::getDivinePower()
{
	weapon* wp = getWeapon();
	weapon* offhand = getOffhand();
	int divinePower = 0;// baseDivinePower;

	//From weapon
	if (wp != nullptr)
		divinePower = wp->getDivinePower();

	//See if offhand is higher
	if (offhand != nullptr)
		if (offhand->getDivinePower() > divinePower)
			divinePower = offhand->getDivinePower();

	//Bonus from arcane scaling (if we use stats)
	if (stats != nullptr)
		divinePower += (stats->devotion - 1) * 10;

	//Done
	return divinePower;
}



/*
	INVENTORY, EQUIPMENT
*/

/*
Equip the given item, if possible.
*/
void person::equipItem(item * which)
{
	
	//If there's already an equipped item in this slot, UNEQUIP IT
	item* here = items.getEquipped(which->getCategory());
	if (here != nullptr)
		unequipItem(here);

	//Equip the new item
	bool equipped = items.equipItem(which);
	
	//Other consequences of equipping something
	if (equipped) {
		itemTypes cat = which->getCategory();

		//Specific item type stuff
		if (cat == ITEM_WEAPON || cat == ITEM_OFFHAND) {
			weapon* wp = static_cast<weapon*>(which);
			doWeaponEquip(wp);
		}
		else if (cat == ITEM_SPELL) {
			//Update our spellstore if the item was successfully equipped
			addSpellKnown(static_cast<spell*>(which));
		}

		/*
		Numerous item types can apply these effects
		*/

		if (cat == ITEM_WEAPON || cat == ITEM_BODY_ARMOUR || cat == ITEM_HELMET || cat == ITEM_CHARM) {

			//Damage resistances
			for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
				damageType dr = static_cast<damageType>(r);
				addDamageResist(dr, which->getDamageResist(dr));
			}

			//Status effect resistances
			int bleedResist = which->getBleedResist();
			if (bleedResist)
				bleedBuildup.increaseMaxValue(bleedResist, false);
		
		}

	}
}


/*
Switch between primary and secondary weapons.
*/
void person::swapWeapon()
{
	//Unequip current weapon
	weapon* wp = items.getWeapon();
	if (wp != nullptr)
		unequipItem(wp);
	//Switch to secondary weapon
	wp = items.swapWeapon();
	if (wp != nullptr)
		doWeaponEquip(wp);
}

/*
Unequip the given item (if it's equipped).
*/
void person::unequipItem(item * which)
{
	if (hasItemEquipped(which)) {
		itemTypes cat = which->getCategory();

		if (cat == ITEM_WEAPON || cat == ITEM_OFFHAND) {
			weapon* wp = static_cast<weapon*>(which);
			//We forget any spells/special attacks this item conferred
			if (wp->getSpecialAttack() != nullptr)
				removeSpellKnown(wp->getSpecialAttack());
			for (auto sp : wp->getSpells())
				removeSpellKnown(sp);
			//Remove resistances
			bleedBuildup.increaseMaxValue(-wp->getBleedResist(), false);
		}

		else if (cat == ITEM_BODY_ARMOUR || cat == ITEM_HELMET) {
			armour* ar = static_cast<armour*>(which); 
			//Damage resistances
			for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
				damageType dr = static_cast<damageType>(r);
				addDamageResist(dr, -ar->getDamageResist(dr));
			}
			//Lose status effect resistances
			int bleedResist = ar->getBleedResist();
			if (bleedResist)
				bleedBuildup.increaseMaxValue(-bleedResist, false);
		}

	}
}

/*
Add the given item to our items carried.
*/
bool person::addItem(item * which)
{
	return items.addItem(which);
}


/*
Sets up a newly-equipped weapon.
*/
void person::doWeaponEquip(weapon * wp)
{
	//Special weapon attack is a spell
	if (wp->getSpecialAttack() != nullptr)
		addSpellKnown(wp->getSpecialAttack());
	//See if it contains any spells we can memorize
	for (auto sp : wp->getSpells())
		addSpellKnown(sp);
	//Reset selected spell, just in case the number of spells changed
	selectedSpell = 0;
	//Resistances conferred
	if (wp->getBleedResist() > 0)
		bleedBuildup.increaseMaxValue(wp->getBleedResist(), false);
}

/*
Returns the currently-highlighted consumable.
*/
consumable * person::getSelectedConsumable()
{
	return items.getSelectedConsumable();
}

/*
Returns a vector of all of our consumable slots.
*/
consumableVector person::getConsumableList()
{
	auto clist = items.getConsumables();
	//Pad out to full size with empty slots
	while (clist.size() < items.MAX_CONSUMABLE_SLOTS)
		clist.push_back(nullptr);
	return clist;
}

/*
Hard-set our selected consumable
*/
void person::setCurrentConsumable(consumable * c)
{
	items.setSelectedConsumable(c);
}

/*
Refreshes all consumables (and any other items that require restoration)
*/
void person::restoreItemsToMax()
{
	for (auto itemCat : items.getAllCarried()) {
		for (auto item : itemCat.items) {
			item->restoreToMax();
		}
	}
}

/*
Returns whether the given item is equipped.
*/
bool person::hasItemEquipped(item * it)
{
	switch (it->getCategory()) {
	case(ITEM_WEAPON): return it == getWeapon();
	case(ITEM_OFFHAND): return it == getOffhand();
	case(ITEM_BODY_ARMOUR): return it == getArmour();
	case(ITEM_HELMET): return it == getHelmet();
	case(ITEM_SPELL): return items.isSpellEquipped(static_cast<spell*>(it));
	case(ITEM_CONSUMABLE): return items.isConsumableEquipped(static_cast<consumable*>(it));
	}
	return false;
}

/*
Returns whether we have the key that will unlock a door.
*/
bool person::hasKey(std::string keyTag)
{
	return items.hasKey(keyTag);
}

/*
	TARGETING
*/

/*
If our target is dead or otherwise invalid, forget about them.
*/
void person::checkTargetValidity()
{
	if (target != nullptr) {
		if (target->isDead)
			target = nullptr;
	}
}


/*
Timer-based stuff
*/

/*
Status effects tick down or proc
*/
void person::applyStatusEffects()
{
	//Bleed: 25 damage/turn once it procs
	bleedBuildup.decrease();
	if (isBleeding) {
		isBleeding--;
		takeDamage(25);
	}
}

/*
Everything timer-based happens
*/
void person::tick()
{
	//Status effects
	applyStatusEffects();
	//Health trickle
	if (healthTrickle) {
		healthTrickle--;
		addHealth(1);
	}
}
