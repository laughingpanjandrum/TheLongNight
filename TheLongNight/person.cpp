#include "person.h"

/*
Constructor you should probably use.
Also sets a default health value, which you'll probably want to change!
*/
person::person(std::string name, int tileCode, TCODColor color, std::string description) : 
	element(name, tileCode, color, description)
{
	//Default values for stats
	health.setTo(100);
	vigour.setTo(10);
	//Status effects
	bleedBuildup.setTo(20, 0);
	poisonBuildup.setTo(20, 0);
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
	//Slowdown effect?
	if (slowdown > 0) {
		slowdown--;
		return SPEED_SLOW;
	}
	//Otherwise, from armour
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
Returns how much damage we do in melee.
*/
int person::getMeleeDamage()
{
	weapon* wp = getWeapon();
	int damage = baseMeleeDamage;
	if (wp != nullptr) {
		//Base damage
		damage = wp->getDamage();
		//Scaling damage
		int scalingPercent = getScalingDamage(wp);
		damage += (float)scalingPercent / 100.0 * (float)damage;
		//Bonus from bleed, if any
		if (isBleeding || !bleedBuildup.isEmpty()) {
			damage += bleedScaling;
		}
	}
	return damage;
}

/*
Returns how much SPECIAL DAMAGE we do in melee.
*/
int person::getDamageOfType(damageType dtype)
{
	weapon* wp = getWeapon();
	if (wp != nullptr) {
		return wp->getDamageOfType(dtype);
	}
	return 0;
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

	//Susceptibility
	if (dtype == DAMAGE_BLESSED) {
		if (isProfane())
			amount *= 2;
		else
			amount /= 2;
	}
	else if (dtype == DAMAGE_PROFANE) {
		if (isBlessed())
			amount *= 2;
		else
			amount /= 2;
	}
	
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
		damage *= bleedDamageFactor;
		bleedBuildup.increase(damage);
		if (bleedBuildup.isFull()) {
			//We BLEED!
			bleedBuildup.clear();
			isBleeding += 2;
		}
	}
	else if (eType == EFFECT_POISON) {
		poisonBuildup.increase(damage);
		if (poisonBuildup.isFull()) {
			//We are POISONED
			poisonBuildup.clear();
			isPoisoned += 1;
		}
	}
}

/*
Fully restores all of our attributes and items.
*/
void person::fullRestore()
{
	//Replenish health
	health.restore();
	vigour.restore();
	//Remove status effects
	bleedBuildup.clear();
	poisonBuildup.clear();
	isBleeding = 0;
	isPoisoned = 0;
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
	case(EFFECT_POISON): return &poisonBuildup;
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
	Effects
*/
void person::applyEffect(effect eff, int potency)
{
	//Restoratives

	if (eff == FULL_RESTORE)
		fullRestore();
	else if (eff == RESTORE_HEALTH)
		addHealth(potency);
	else if (eff == RESTORE_VIGOUR)
		addVigour(potency);

	//Buffs

	else if (eff == GAIN_FREE_MOVES)
		gainFreeMoves(potency);
	else if (eff == GAIN_MULTIPLE_ATTACKS)
		attacksPerHit = potency;
	else if (eff == SCALE_NEXT_ATTACK)
		scaleNextAttack = potency;
	else if (eff == ADD_HEALTH_TRICKLE)
		healthTrickle += potency;
	else if (eff == GAIN_MAX_HEALTH)
		health.increaseMaxValue(potency, true);
	else if (eff == GAIN_HEALTH_ON_KILL)
		healthOnKill += potency;
	else if (eff == GAIN_BLEED_SCALING)
		bleedScaling += potency;
	else if (eff == BECOME_INVISIBLE)
		invisibility += potency;
	else if (eff == CHANGE_FRAGMENT_PICKUP_MULT)
		fragmentPickupMultiplier = potency;

	//Debuffs

	else if (eff == SLOWDOWN)
		slowdown += potency;

	//Defensive buffs

	else if (eff == GAIN_DEFENCE)
		addDefence(potency);
	else if (eff == GAIN_ACID_RESIST)
		addDamageResist(DAMAGE_ACID, potency);
	else if (eff == GAIN_COLD_RESIST)
		addDamageResist(DAMAGE_COLD, potency);
	else if (eff == GAIN_ELECTRIC_RESIST)
		addDamageResist(DAMAGE_ELECTRIC, potency);
	else if (eff == GAIN_FIRE_RESIST)
		addDamageResist(DAMAGE_FIRE, potency);
	else if (eff == GAIN_MAGIC_RESIST)
		addDamageResist(DAMAGE_MAGIC, potency);
	else if (eff == REMOVE_BLEED) {
		isBleeding = 0;
		clearBleed();
	}
	else if (eff == REMOVE_POISON) {
		isPoisoned = 0;
		poisonBuildup.clear();
	}
	else if (eff == HURT_BLEEDER) {
		//This only affects bleeding targets
		if (isBleeding > 0) {
			takeDamage(potency, DAMAGE_PROFANE);
		}
	}

	//Spell buffs

	else if (eff == SCALE_NEXT_SPELL)
		scaleNextSpell = potency;
	else if (eff == SCALE_NEXT_PRAYER)
		scaleNextPrayer = potency;
	else if (eff == SPELL_ACID_INFUSION)
		spellAcidInfusion = potency;

	//Damage effects

	else if (eff == APPLY_PHYSICAL_DAMAGE)
		takeDamage(potency, DAMAGE_PHYSICAL);
	else if (eff == APPLY_MAGIC_DAMAGE)
		takeDamage(potency, DAMAGE_MAGIC);
	else if (eff == APPLY_ACID_DAMAGE)
		takeDamage(potency, DAMAGE_ACID);
	else if (eff == APPLY_COLD_DAMAGE)
		takeDamage(potency, DAMAGE_COLD);
	else if (eff == APPLY_FIRE_DAMAGE)
		takeDamage(potency, DAMAGE_FIRE);
	else if (eff == APPLY_ELECTRIC_DAMAGE)
		takeDamage(potency, DAMAGE_ELECTRIC);
	else if (eff == APPLY_PROFANE_DAMAGE)
		takeDamage(potency, DAMAGE_PROFANE);
	else if (eff == APPLY_BLESSED_DAMAGE)
		takeDamage(potency, DAMAGE_BLESSED);

	// Status effects

	else if (eff == APPLY_BLEED_DAMAGE)
		takeStatusEffectDamage(EFFECT_BLEED, potency);
	else if (eff == APPLY_BLINDING)
		blind(potency);

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
Sets current spell by number rather than instance.
*/
void person::setCurrentSpell(int sp)
{
	if (sp < spellsKnown.size())
		selectedSpell = sp;
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
		spellPower += wp->getSpellPower();

	//See if offhand is higher
	if (offhand != nullptr)
		if (offhand->getSpellPower() > spellPower)
			spellPower += offhand->getSpellPower();

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
	int divinePower = baseDivinePower;

	//From weapon
	if (wp != nullptr)
		divinePower += wp->getDivinePower();

	//See if offhand is higher
	if (offhand != nullptr)
		if (offhand->getDivinePower() > divinePower)
			divinePower += offhand->getDivinePower();

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
			//Update our spellstore if the spell was successfully equipped
			addSpellKnown(static_cast<spell*>(which));
		}

		/*
		Numerous item types can apply these effects
		*/

		if (cat == ITEM_WEAPON || cat == ITEM_OFFHAND || cat == ITEM_BODY_ARMOUR || cat == ITEM_HELMET || cat == ITEM_CHARM) {

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

		if (cat == ITEM_CHARM) {

			charm* c = static_cast<charm*>(which);

			//Special effects from charms
			for (int i = 0; i < c->getAllEffects().size(); i++) {
				effect eType = c->getAllEffects().at(i);
				int potency = c->getAllPotencies().at(i);
				applyEffect(eType, potency);
			}

		}

	}
	else {

		//See if we unequipped a spell
		if (which->getCategory() == ITEM_SPELL) {
			spell* sp = static_cast<spell*>(which);
			auto iter = std::find(spellsKnown.begin(), spellsKnown.end(), sp);
			if (iter != spellsKnown.end())
				spellsKnown.erase(iter);
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
	//Reset readied spell, since this could alter it
	selectedSpell = 0;
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
		}

		if (cat == ITEM_CHARM) {
			charm* c = static_cast<charm*>(which);
			//Special effects from charms
			for (int i = 0; i < c->getAllEffects().size(); i++) {
				effect eType = c->getAllEffects().at(i);
				int potency = c->getAllPotencies().at(i);
				applyEffect(eType, -potency);
			}
		}

		if (cat != ITEM_CONSUMABLE || cat != ITEM_SPELL) {

			//Lose damage resistances
			for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
				damageType dr = static_cast<damageType>(r);
				addDamageResist(dr, -which->getDamageResist(dr));
			}

			//Lose status effect resistances
			int bleedResist = which->getBleedResist();
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
	//Item stops glittering once we pick it up.
	which->isGlittery = false;
	//Now we can carry it.
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
	case(ITEM_CHARM): return it == getCharm();
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
	//Poison: constant damage, never goes away on its own
	poisonBuildup.decrease();
	if (isPoisoned) {
		takeDamage(isPoisoned);
	}
	//Blinding: just ticks down
	if (blinding > 0)
		blinding--;
	//Invisibility fades
	if (invisibility)
		invisibility--;
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
