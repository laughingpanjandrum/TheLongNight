#include "person.h"

/*
Constructor you should probably use.
Also sets a default health value, which you'll probably want to change!
*/
person::person(std::string name, int tileCode, TCODColor color) : element(name, tileCode, color)
{
	//Default values for stats/resistances
	health.setTo(100);
	vigour.setTo(10);
	bleedBuildup.setTo(20, 0);
}

person::~person()
{
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
	int base = 0;
	//Bonus from armour
	armour* ar = getArmour();
	if (ar != nullptr)
		base += ar->getDefence();
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
void person::takeDamage(int amount)
{
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
	MAGIC
*/


/*
Forget a spell we know (e.g. because we un-equipped a wand)
*/
void person::removeSpellKnown(spell * sp)
{
	auto iter = std::find(spellsKnown.begin(), spellsKnown.end(), sp);
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
Cycle to the next spell
*/
void person::cycleSelectedSpell()
{
	selectedSpell++;
	if (selectedSpell >= spellsKnown.size())
		selectedSpell = 0;
}



/*
	INVENTORY, EQUIPMENT
*/

/*
Equip the given item, if possible.
*/
void person::equipItem(item * which)
{
	items.equipItem(which);
	//Other consequences of equipping something
	if (which->getCategory() == ITEM_WEAPON) {
		weapon* wp = static_cast<weapon*>(which);
		//Special weapon attack is a spell
		if (wp->getSpecialAttack() != nullptr)
			addSpellKnown(wp->getSpecialAttack());
		//See if it contains any spells we can memorize
		for (auto sp : wp->getSpells())
			addSpellKnown(sp);
	}
}

/*
Add the given item to our items carried.
*/
void person::addItem(item * which)
{
	items.addItem(which);
}

/*
Returns our equipped weapon, or nullptr if we don't have one equipped.
*/
weapon * person::getWeapon()
{
	return items.getWeapon();
}

/*
Returns our equipped body armour.
*/
armour * person::getArmour()
{
	return items.getArmour();
}

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
	/*consumableVector clist;
	auto equipmentList = items.getAllEquipped();
	for (auto i = equipmentList.begin(); i != equipmentList.end(); i++) {
		if ((*i).category == ITEM_CONSUMABLE) {
			consumable* cons = static_cast<consumable*>((*i).equipped);
			clist.push_back(cons);
		}
	}
	return clist;*/
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
	applyStatusEffects();
}
