#include "weapon.h"




weapon::weapon(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_WEAPON, description)
{
	//Special damage types
	for (int i = 0; i < ALL_DAMAGE_TYPES; i++) {
		damageTypes.push_back(0);
	}
}


/*
Special constructor just for monsters: adds status effect damage
*/
weapon::weapon(int baseDamage, const int attackSpeed, statusEffects eType, int statusEffectDamage) :
	weapon()
{
	setBasicAttributes(baseDamage, attackSpeed);
	addStatusEffect(eType, statusEffectDamage);
}

/*
Special constructor just for monsters: adds additional damage type
*/
weapon::weapon(int baseDamage, const int attackSpeed, damageType dtype, int specialDamage) :
	weapon()
{
	setBasicAttributes(baseDamage, attackSpeed);
	addDamageType(dtype, specialDamage);
}

weapon::~weapon()
{
}

/*
Can be modified by our runestone.
*/
std::string weapon::getMenuName()
{
	std::string name = getName();
	if (rune != nullptr)
		name = rune->nameMod + " " + name;
	return name;
}

/*
	SETTING
*/

void weapon::setBasicAttributes(int damage, int attackDelay)
{
	this->damage = damage;
	this->attackDelay = attackDelay;
}

void weapon::addDamageType(damageType dtype, int damage)
{
	damageTypes.at(dtype) = damage;
}

/*
Add a status effect, e.g. bleed, to the weapon's standard attack
*/
void weapon::addStatusEffect(statusEffects eType, int damage)
{
	statusEffectType.push_back(eType);
	statusEffectDamage.push_back(damage);
}

int weapon::getDamageOfType(damageType dtype)
{
	//Base
	int dmg = damageTypes.at(dtype);
	//Bonus from buff?
	if (currentBuff.dtype == dtype)
		dmg += currentBuff.bonusDamage;
	//Done
	return dmg;
}

/*
Returns whether we're currently holding the given spell.
*/
bool weapon::hasSpellStored(spell * sp)
{
	auto it = std::find(spellstore.begin(), spellstore.end(), sp);
	return it != spellstore.end();
}

/*
Remove a spell from here.
*/
void weapon::removeSpell(spell * sp)
{
	auto iter = std::find(spellstore.begin(), spellstore.end(), sp);
	if (iter != spellstore.end())
		spellstore.erase(iter);
}



/*
	Status Effects
*/


int weapon::getStatusEffectDamage(int idx)
{
	return statusEffectDamage.at(idx);
}


/*
Returns how much bonus damage per stat point we deal for the given stat.
We do more scaling damage the fewer stats we scale with.
*/
int weapon::getScalingDamage(statScaling st)
{
	int dmg = 0;
	//Inherent to the weapon
	auto it = std::find(scaling.begin(), scaling.end(), st);
	if (it != scaling.end())
		dmg = 5 - scaling.size();
	//From runestone
	if (rune != nullptr) {
		if (rune->addScalingType == st)
			dmg += 5 - scaling.size();
	}
	//Done, return total
	return dmg;
}


/*
For descriptive reasons.
*/
std::string getAttackSpeedName(int del)
{
	switch (del) {
	case(SPEED_FAST): return "Fast";
	case(SPEED_NORMAL): return "Normal";
	case(SPEED_SLOW): return "Slow";
	}
	return "None";
}


/*
	ALL ITEMS
*/

//		Weapons

weapon * weapon_SplinteredSword()
{
	weapon* wp = new weapon("Splintered Sword", SWORD_TILE, TCODColor::grey,
		"Once a treasured heirloom. Now it's just a broken sword, decent at serration, but certainly unfit for a stardrift captain.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addScalingType(SCALE_STR);
	return wp;
}

weapon * weapon_ThinKnife()
{
	weapon* wp = new weapon("Thin Knife", DAGGER_TILE, TCODColor::lightGrey,
		"A weapon for small, quick folks.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_StraightSword()
{
	weapon* wp = new weapon("Straight Sword", SWORD_TILE, TCODColor::lightGrey,
		"It's just a simple sword, but the haft has a nice little dragon face carved in it.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->setSpecialAttack(attack_StrongAttack());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_Warhammer()
{
	weapon* wp = new weapon("Warhammer", HAMMER_TILE, TCODColor::lightSepia,
		"A big ole hammer for smashing things.");
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->setSpecialAttack(attack_Knockback());
	wp->addScalingType(SCALE_STR);
	return wp;
}

weapon * weapon_CrowKnightSword()
{
	weapon* wp = new weapon("Crow Knight Sword", SWORD_TILE, TCODColor::lightGrey,
		"The hilt is carved in the shape of a crow's head, and the blade is tinted black. The crow knights are certainly dedicated to a theme.");
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_CrowKnife()
{
	weapon* wp = new weapon("Crow Knife", DAGGER_TILE, TCODColor::lightGrey,
		"The short blade of this knife is barbed to inflict bleeding.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_CrowHalfsword()
{
	weapon* wp = new weapon("Crow Half-sword", SWORD_TILE, TCODColor::darkCrimson,
		"Serrated half-sword of a crow knight, thirsting for blood.");
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 25);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_OldCrowsLongKnife()
{
	weapon* wp = new weapon("Old Crow's Long Knife", DAGGER_TILE, TCODColor::lightCrimson,
		"Long, serrated knife wielded by the Old Crow. Once he swore an oath to protect the pilgrims who ventured past the Wyrd, \
but over the years his promise was forgotten.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 15);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	return wp;
}

weapon * weapon_NotchedGreatsword()
{
	weapon* wp = new weapon("Notched Greatsword", SWORD_TILE, TCODColor::grey,
		"The blade of this huge sword is notched and marred, but it should still work.");
	wp->setBasicAttributes(100, SPEED_SLOW);
	wp->addScalingType(SCALE_STR);
	wp->setSpecialAttack(attack_MightyCleave());
	return wp;
}

weapon * weapon_FishmansHarpoon()
{
	weapon* wp = new weapon("Fishman's Harpoon", SPEAR_TILE, TCODColor::lime,
		"Its point is barbed to induce bleeding and to drag the victim.");
	wp->setBasicAttributes(40, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Spearfishing());
	return wp;
}

weapon * weapon_FishmansKnife()
{
	weapon* wp = new weapon("Fishman's Knife", DAGGER_TILE, TCODColor::lime,
		"The short knives of the fishmen are weak but incredibly quick.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Fillet());
	return wp;
}

weapon * weapon_CityGuardianWarhammer()
{
	weapon* wp = new weapon("City Guardian Warhammer", HAMMER_TILE, TCODColor::darkYellow,
		"Warhammer wielded by the divine guards of the Winter Court.");
	wp->setBasicAttributes(75, SPEED_SLOW);
	wp->addDamageType(DAMAGE_BLESSED, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_DivineSmite());
	return wp;
}

weapon * weapon_BloodDrinkersKnife()
{
	weapon* wp = new weapon("Blood Drinker's Knife", DAGGER_TILE, TCODColor::crimson,
		"This knife is serrated on both the blade and the grip, for the profane scriptures of Pash decree that to bleed is to \
see the true shape of all things.");
	wp->setBasicAttributes(10, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 20);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_BloodFeast());
	return wp;
}

weapon * weapon_KythielsScythe()
{
	weapon* wp = new weapon("Kythiel's Scythe", SPEAR_TILE, TCODColor::lightYellow,
		"Kythiel, last priestess of the Winter Court, was the only one who stood against the Void when the city fell.");
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_BLESSED, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_HolySummons());
	return wp;
}

weapon * weapon_VoidTouchedKnife()
{
	weapon* wp = new weapon("Void-Touched Knife", DAGGER_TILE, TCODColor::lightPurple,
		"The blade of this knife phases in and out of reality, pulsing with the raging heart of the Void.");
	wp->setBasicAttributes(0, SPEED_FAST);
	wp->addDamageType(DAMAGE_PROFANE, 20);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_VoidSlash());
	return wp;
}

weapon * weapon_VoidCrystalGreatsword()
{
	weapon* wp = new weapon("Void Crystal Greatsword", SWORD_TILE, TCODColor::lightFuchsia,
		"The blade of this enormous sword is forged from pure voidcrystal and phases in and out of existence.");
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addDamageType(DAMAGE_PROFANE, 50);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_VoidSurge());
	return wp;
}

weapon * weapon_ArmOfTheDuke()
{
	weapon* wp = new weapon("Arm of the Duke", SWORD_TILE, TCODColor::lightGreen,
		"The arms of the Duke were rubbery and numerous. Serves as both a weapon and a catalyst of obscene prayer.");
	wp->setBasicAttributes(25, SPEED_SLOW);
	wp->addDamageType(DAMAGE_PROFANE, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(80);
	wp->setSpecialAttack(ability_TentacleSwarm());
	wp->isProfane = true;
	return wp;
}

weapon * weapon_DragonboneSword()
{
	weapon* wp = new weapon("Dragonbone Sword", SWORD_TILE, TCODColor::darkFlame,
		"The bones of dragons are known to possess a profound affinity for fire, and swords forged with this rare material \
contain some of that power.");
	wp->setBasicAttributes(30, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_FIRE, 20);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_FlameSwathe());
	return wp;
}

weapon * weapon_SerpentsTooth()
{
	weapon* wp = new weapon("Serpent's Tooth", DAGGER_TILE, TCODColor::lime,
		"Tiny dagger made from the tooth of a poisonous serpent. Ideal for slipping between an unsuspecting suspect's ribs.");
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_POISON, 15);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_DripVenom());
	return wp;
}

weapon * weapon_CrimsonNail()
{
	weapon* wp = new weapon("Crimson Nail", DAGGER_TILE, TCODColor::lightCrimson,
		"This tiny nail is stained a deep red and is deadly sharp.");
	wp->setBasicAttributes(10, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->scaleWithDamage = true;
	wp->setSpecialAttack(attack_SuckOutLife());
	return wp;
}

weapon * weapon_ProfaneGreatsword()
{
	weapon* wp = new weapon("Profane Greatsword", SWORD_TILE, TCODColor::purple,
		"A greatsword charged with the profane energy of the Void.");
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addDamageType(DAMAGE_PROFANE, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	return wp;
}

//		Shields

weapon * shield_BatteredWoodenShield()
{
	weapon* wp = new weapon("Battered Wooden Shield", SHIELD_TILE, TCODColor::sepia,
		"It's just a scrap of wood, but it might still save your life.");
	wp->setDefence(10);
	wp->makeOffhand();
	return wp;
}

weapon * shield_WoodenWyrdShield()
{
	weapon* wp = new weapon("Wooden Wyrd Shield", SHIELD_TILE, TCODColor::darkSepia,
		"The Wyrd hid behind these shields when the Blind King's arrows rained down on them.");
	wp->setDefence(10);
	wp->setBleedResist(15);
	wp->setDamagePenalty(5);
	wp->makeOffhand();
	return wp;
}

weapon * shield_BatteredSteelShield()
{
	weapon* wp = new weapon("Battered Steel Shield", SHIELD_TILE, TCODColor::grey,
		"Aged, but perhaps still useful, if you can look past the dents.");
	wp->setDefence(10);
	wp->setDamageResist(DAMAGE_ACID, 5);
	wp->setDamageResist(DAMAGE_FIRE, 5);
	wp->setDamageResist(DAMAGE_ELECTRIC, 5);
	wp->setDamageResist(DAMAGE_COLD, 5);
	wp->setDamagePenalty(10);
	wp->makeOffhand();
	return wp;
}

weapon * shield_CityGuardianShield()
{
	weapon* wp = new weapon("City Guardian Shield", SHIELD_TILE, TCODColor::darkYellow,
		"Round shield of the guards of the Winter Court. They were meant to protect the city against the onslaught of the Void, \
but not a single knight survived.");
	wp->setDefence(15);
	wp->setDamageResist(DAMAGE_PROFANE, 15);
	wp->setDamageResist(DAMAGE_FIRE, 10);
	wp->setDamagePenalty(10);
	wp->makeOffhand();
	return wp;
}

weapon * shield_VoidTouchedShield()
{
	weapon* wp = new weapon("Void-Touched Shield", SHIELD_TILE, TCODColor::darkPurple,
		"This shield phases in and out of reality, forming fissures into the Void.");
	wp->setDefence(5);
	wp->setDamageResist(DAMAGE_PROFANE, 50);
	wp->setDamagePenalty(5);
	wp->makeOffhand();
	return wp;
}

weapon * shield_DragonboneShield()
{
	weapon* wp = new weapon("Dragonbone Shield", SHIELD_TILE, TCODColor::darkFlame,
		"This shield, forged from the bones of an ancient dragon, confers powerful protection from fire.");
	wp->setDefence(20);
	wp->setDamageResist(DAMAGE_FIRE, 50);
	wp->setDamagePenalty(15);
	wp->makeOffhand();
	return wp;
}

//		Wands

weapon * wand_DriftwoodWand()
{
	weapon* wp = new weapon("Driftwood Wand", STAFF_TILE, TCODColor::sepia,
		"Someone made this chunk of wand in a desparate hurry.");
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setSpellPower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_PowerSurge());
	return wp;
}

weapon * wand_FishmansToadstaff()
{
	weapon* wp = new weapon("Fishman's Toadstaff", STAFF_TILE, TCODColor::darkLime,
		"Sometimes the toad impaled on the tip twitches. Is it still alive? Best not to dwell on it.");
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(4);
	wp->setSpellPower(75);
	wp->setSpecialAttack(ability_AcidInfusion());
	wp->makeOffhand();
	return wp;
}

weapon * chime_ClericsCrackedChime()
{
	weapon* wp = new weapon("Cleric's Cracked Chime", CHIME_TILE, TCODColor::darkYellow,
		"Ring this broken chime to politely ask favours of the gods.");
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_DivineFavour());
	return wp;
}

weapon * chime_WyrdBellbranch()
{
	weapon* wp = new weapon("Wyrd Bellbranch", CHIME_TILE, TCODColor::sepia,
		"The trees of the Wyrd once grew tiny singing bells on their branches.");
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(100);
	wp->setSpecialAttack(ability_WyrdChannel());
	wp->makeOffhand();
	return wp;
}

weapon * chime_OrsylsProfaneChime()
{
	weapon* wp = new weapon("Orsyl's Profane Chime", CHIME_TILE, TCODColor::lightPurple,
		"Once-holy chime of Orsyl, who stood against the Void for as long as he could. As the nightmare consumed him, he prayed to \
the old gods, but they did not answer.");
	wp->setSpellstoreSize(4);
	wp->setDivinePower(100);
	wp->setSpecialAttack(prayer_ProfaneRadiance());
	wp->makeOffhand();
	wp->isProfane = true;
	return wp;
}
