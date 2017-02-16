#include "weapon.h"




weapon::weapon(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_WEAPON, description)
{
	//Special damage types
	for (int i = 0; i < ALL_DAMAGE_TYPES; i++) {
		damageTypes.push_back(0);
	}
	//Status effect types
	for (int i = 0; i != EFFECT_NONE; i++) {
		statusEffectDamage.push_back(0);
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
	statusEffectDamage.at(eType) += damage;
}


/*
Damage, plus bonus from runestone, if any
*/
int weapon::getDamage()
{
	int total = damage;
	auto r = getRune();
	if (r != nullptr) {
		if (r->addScalingType == SCALE_PHYSICAL)
			total += (float)total * 0.2;
	}
	return total;
}

/*
Other damage types
*/
int weapon::getDamageOfType(damageType dtype)
{
	
	//Base
	int dmg = damageTypes.at(dtype);
	
	//Bonus from buff?
	if (currentBuff.dtype == dtype)
		dmg += currentBuff.bonusDamage;

	//Bonus from runestone
	if (getRunestoneDamage(dtype)) {
		dmg += (float)getDamage() * 0.4;
	}
	
	//Done
	return dmg;
}

/*
Returns whether we're currently holding the given spell.
*/
bool weapon::hasSpellStored(spellSharedPtr sp)
{
	auto it = std::find(spellstore.begin(), spellstore.end(), sp);
	return it != spellstore.end();
}

/*
Remove a spell from here.
*/
void weapon::removeSpell(spellSharedPtr sp)
{
	auto iter = std::find(spellstore.begin(), spellstore.end(), sp);
	if (iter != spellstore.end())
		spellstore.erase(iter);
}



/*
	Status Effects
*/


int weapon::getStatusEffectDamage(statusEffects etype)
{
	int total = statusEffectDamage.at(etype);
	//Check for bonus from buffs
	if (currentBuff.etype == etype) {
		total += currentBuff.bonusDamage;
	}
	//Check for bonus from runestones
	if (rune != nullptr) {
		if (rune->addScalingType == SCALE_BLEED && etype == EFFECT_BLEED)
			total += 10 + total / 5;
	}
	//Done, return
	return total;
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
Returns how much bonus damage of the given type we do based on our equipped runestone.
*/
bool weapon::getRunestoneDamage(damageType dtype)
{
	auto runestone = getRune();
	if (runestone != nullptr) {
		if (runestone->addScalingType == SCALE_FIRE && dtype == DAMAGE_FIRE)
			return true;
		else if (runestone->addScalingType == SCALE_ELECTRIC && dtype == DAMAGE_ELECTRIC)
			return true;
		else if (runestone->addScalingType == SCALE_MAGIC && dtype == DAMAGE_MAGIC)
			return true;
	}
	return false;
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

weaponSharedPtr weapon_SplinteredSword()
{
	weaponSharedPtr wp(new weapon("Splintered Sword", SWORD_TILE, TCODColor::grey,
		"Once a treasured heirloom. Now it's just a broken sword, decent at serration, but certainly unfit for a stardrift captain."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addScalingType(SCALE_STR);
	wp->setImage(IMAGE_SPLINTERED_SWORD);
	return wp;
}

weaponSharedPtr weapon_ThinKnife()
{
	weaponSharedPtr wp(new weapon("Thin Knife", DAGGER_TILE, TCODColor::lightGrey,
		"A weapon for small, quick folks."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_THIN_KNIFE);
	return wp;
}

weaponSharedPtr weapon_StraightSword()
{
	weaponSharedPtr wp(new weapon("Straight Sword", SWORD_TILE, TCODColor::lightGrey,
		"It's just a simple sword, but the haft has a nice little dragon face carved in it."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->setSpecialAttack(attack_StrongAttack());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_STRAIGHT_SWORD);
	return wp;
}

weaponSharedPtr weapon_Warhammer()
{
	weaponSharedPtr wp(new weapon("Warhammer", HAMMER_TILE, TCODColor::lightSepia,
		"A big ole hammer for smashing things."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->setSpecialAttack(attack_Knockback());
	wp->addScalingType(SCALE_STR);
	wp->setImage(IMAGE_WARHAMMER);
	return wp;
}

weaponSharedPtr weapon_CrowKnightSword()
{
	weaponSharedPtr wp(new weapon("Crow Knight Sword", SWORD_TILE, TCODColor::lightGrey,
		"The hilt is carved in the shape of a crow's head, and the blade is tinted black. The crow knights are certainly dedicated to a theme."));
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_CROW_KNIGHT_SWORD);
	return wp;
}

weaponSharedPtr weapon_CrowKnife()
{
	weaponSharedPtr wp(new weapon("Crow Knife", DAGGER_TILE, TCODColor::lightGrey,
		"The short blade of this knife is barbed to inflict bleeding."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_CROW_KNIFE);
	return wp;
}

weaponSharedPtr weapon_CrowHalfsword()
{
	weaponSharedPtr wp(new weapon("Crow Half-sword", SWORD_TILE, TCODColor::darkCrimson,
		"Serrated half-sword of a crow knight, thirsting for blood."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 25);
	wp->setSpecialAttack(attack_Splintering());
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_CROW_HALFSWORD);
	return wp;
}

weaponSharedPtr weapon_OldCrowsLongKnife()
{
	weaponSharedPtr wp(new weapon("Old Crow's Long Knife", DAGGER_TILE, TCODColor::lightCrimson,
		"Long, serrated knife wielded by the Old Crow. Once he swore an oath to protect the pilgrims who ventured past the Wyrd, \
but over the years his promise was forgotten."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 15);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	wp->setImage(IMAGE_OLD_CROWS_LONG_KNIFE);
	return wp;
}

weaponSharedPtr weapon_NotchedGreatsword()
{
	weaponSharedPtr wp(new weapon("Notched Greatsword", SWORD_TILE, TCODColor::grey,
		"The blade of this huge sword is notched and marred, but it should still work."));
	wp->setBasicAttributes(100, SPEED_SLOW);
	wp->addScalingType(SCALE_STR);
	wp->setSpecialAttack(attack_MightyCleave());
	wp->setImage(IMAGE_NOTCHED_GREATSWORD);
	return wp;
}

weaponSharedPtr weapon_FishmansHarpoon()
{
	weaponSharedPtr wp(new weapon("Fishman's Harpoon", SPEAR_TILE, TCODColor::lime,
		"Its point is barbed to induce bleeding and to drag the victim."));
	wp->setBasicAttributes(40, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Spearfishing());
	return wp;
}

weaponSharedPtr weapon_FishmansKnife()
{
	weaponSharedPtr wp(new weapon("Fishman's Knife", DAGGER_TILE, TCODColor::lime,
		"The short knives of the fishmen are weak but incredibly quick."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Fillet());
	return wp;
}

weaponSharedPtr weapon_CityGuardianWarhammer()
{
	weaponSharedPtr wp(new weapon("City Guardian Warhammer", HAMMER_TILE, TCODColor::darkYellow,
		"Warhammer wielded by the divine guards of the Winter Court."));
	wp->setBasicAttributes(75, SPEED_SLOW);
	wp->addDamageType(DAMAGE_BLESSED, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_DivineSmite());
	return wp;
}

weaponSharedPtr weapon_BloodDrinkersKnife()
{
	weaponSharedPtr wp(new weapon("Blood Drinker's Knife", DAGGER_TILE, TCODColor::crimson,
		"This knife is serrated on both the blade and the grip, for the profane scriptures of Pash decree that to bleed is to \
see the true shape of all things."));
	wp->setBasicAttributes(10, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 20);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_BloodFeast());
	return wp;
}

weaponSharedPtr weapon_KythielsScythe()
{
	weaponSharedPtr wp(new weapon("Kythiel's Scythe", SPEAR_TILE, TCODColor::lightYellow,
		"Kythiel, last priestess of the Winter Court, was the only one who stood against the Void when the city fell."));
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_BLESSED, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_HolySummons());
	return wp;
}

weaponSharedPtr weapon_VoidTouchedKnife()
{
	weaponSharedPtr wp(new weapon("Void-Touched Knife", DAGGER_TILE, TCODColor::lightPurple,
		"The blade of this knife phases in and out of reality, pulsing with the raging heart of the Void."));
	wp->setBasicAttributes(0, SPEED_FAST);
	wp->addDamageType(DAMAGE_PROFANE, 20);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_VoidSlash());
	return wp;
}

weaponSharedPtr weapon_VoidCrystalGreatsword()
{
	weaponSharedPtr wp(new weapon("Void Crystal Greatsword", SWORD_TILE, TCODColor::lightFuchsia,
		"The blade of this enormous sword is forged from pure voidcrystal and phases in and out of existence."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addDamageType(DAMAGE_PROFANE, 50);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_VoidSurge());
	return wp;
}

weaponSharedPtr weapon_ArmOfTheDuke()
{
	weaponSharedPtr wp(new weapon("Arm of the Duke", SWORD_TILE, TCODColor::lightGreen,
		"The arms of the Duke were rubbery and numerous. Serves as both a weapon and a catalyst of obscene prayer."));
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

weaponSharedPtr weapon_DragonboneSword()
{
	weaponSharedPtr wp(new weapon("Dragonbone Sword", SWORD_TILE, TCODColor::darkFlame,
		"The bones of dragons are known to possess a profound affinity for fire, and swords forged with this rare material \
contain some of that power."));
	wp->setBasicAttributes(30, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_FIRE, 20);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_FlameSwathe());
	return wp;
}

weaponSharedPtr weapon_SerpentsTooth()
{
	weaponSharedPtr wp(new weapon("Serpent's Tooth", DAGGER_TILE, TCODColor::lime,
		"Tiny dagger made from the tooth of a poisonous serpent. Ideal for slipping between an unsuspecting suspect's ribs."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_POISON, 15);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_DripVenom());
	return wp;
}

weaponSharedPtr weapon_CrimsonNail()
{
	weaponSharedPtr wp(new weapon("Crimson Nail", DAGGER_TILE, TCODColor::lightCrimson,
		"This tiny nail is stained a deep red and is deadly sharp."));
	wp->setBasicAttributes(10, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->scaleWithDamage = true;
	wp->setSpecialAttack(attack_SuckOutLife());
	return wp;
}

weaponSharedPtr weapon_SpiderboneShard()
{
	weaponSharedPtr wp(new weapon("Spiderbone Shard", DAGGER_TILE, TCODColor::white,
		"A sharpened piece of spider bone, infused with the ancient rage of Qom."));
	wp->setBasicAttributes(20, SPEED_FAST);
	wp->addStatusEffect(EFFECT_POISON, 20);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_SpiderStrike());
	return wp;
}

weaponSharedPtr weapon_SacrificialKnife()
{
	weaponSharedPtr wp(new weapon("Sacrificial Knife", DAGGER_TILE, TCODColor::orange,
		"Lady Tvert sent the Emissaries to bring the word of the gods to the people of the Valley. \
Those who refused to heed the call were sacrificed to the Gaping Maw to the benefit of the living."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->setDamageToSelf(5);
	wp->setSpecialAttack(attack_DeepCut());
	return wp;
}

weaponSharedPtr weapon_SlaveMastersWhip()
{
	weaponSharedPtr wp(new weapon("Slave Master's Whip", WHIP_TILE, TCODColor::lightRed,
		"Long whip of the slave-drivers of the Valley, barbed to induce bleeding. The prisoners of the Valley served Hightower \
until death, but this did not satisfy Lady Tvert, whose made a pact to ensure their eternal servitude."));
	wp->setBasicAttributes(5, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 20);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Lash());
	return wp;
}

weaponSharedPtr weapon_LadyTvertsClaws()
{
	weaponSharedPtr wp(new weapon("Lady Tvert's Claws", DAGGER_TILE, TCODColor::lightBlue,
		"Lady Tvert's pact with the Herald caused her body to deform, but she herself, \
and her followers, saw a peculiar beauty in the transformation."));
	wp->setBasicAttributes(10, SPEED_FAST);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->addStatusEffect(EFFECT_POISON, 10);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_RavenousHunger());
	return wp;
}

weaponSharedPtr weapon_EtherealSword()
{
	weaponSharedPtr wp(new weapon("Ethereal Sword", SWORD_TILE, TCODColor::lightMagenta,
		"Half-unreal sword of the Sparrow Knights. These blades were once made of steel, but now \
little of their original form remains."));
	wp->setBasicAttributes(0, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_MAGIC, 50);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_ARC);
	wp->setSpecialAttack(attack_EtherealStrike());
	return wp;
}

weaponSharedPtr weapon_CorensGreataxe()
{
	weaponSharedPtr wp(new weapon("Coren's Greataxe", SWORD_TILE, TCODColor::darkCrimson,
		"The enormous greataxe of Coren, whose thirst for blood was insatiable. Eventually the gods gave him \
what he desired, and it drove him mad."));
	wp->setBasicAttributes(100, SPEED_SLOW);
	wp->addScalingType(SCALE_STR);
	wp->addStatusEffect(EFFECT_BLEED, 10);
	wp->setSpecialAttack(attack_Frenzy());
	return wp;
}

weaponSharedPtr weapon_ProfaneGreatsword()
{
	weaponSharedPtr wp(new weapon("Profane Greatsword", SWORD_TILE, TCODColor::purple,
		"A greatsword charged with the profane energy of the Void."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addDamageType(DAMAGE_PROFANE, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEV);
	return wp;
}

//		Shields

weaponSharedPtr shield_BatteredWoodenShield()
{
	weaponSharedPtr wp(new weapon("Battered Wooden Shield", SHIELD_TILE, TCODColor::sepia,
		"It's just a scrap of wood, but it might still save your life."));
	wp->setDefence(10);
	wp->makeOffhand();
	wp->setImage(IMAGE_BATTERED_WOODEN_SHIELD);
	return wp;
}

weaponSharedPtr shield_WoodenWyrdShield()
{
	weaponSharedPtr wp(new weapon("Wooden Wyrd Shield", SHIELD_TILE, TCODColor::darkSepia,
		"The Wyrd hid behind these shields when the Blind King's arrows rained down on them."));
	wp->setDefence(10);
	wp->setBleedResist(15);
	wp->setDamagePenalty(5);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_BatteredSteelShield()
{
	weaponSharedPtr wp(new weapon("Battered Steel Shield", SHIELD_TILE, TCODColor::grey,
		"Aged, but perhaps still useful, if you can look past the dents."));
	wp->setDefence(10);
	wp->setDamageResist(DAMAGE_ACID, 5);
	wp->setDamageResist(DAMAGE_FIRE, 5);
	wp->setDamageResist(DAMAGE_ELECTRIC, 5);
	wp->setDamageResist(DAMAGE_COLD, 5);
	wp->setDamagePenalty(10);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_CityGuardianShield()
{
	weaponSharedPtr wp(new weapon("City Guardian Shield", SHIELD_TILE, TCODColor::darkYellow,
		"Round shield of the guards of the Winter Court. They were meant to protect the city against the onslaught of the Void, \
but not a single knight survived."));
	wp->setDefence(15);
	wp->setDamageResist(DAMAGE_PROFANE, 15);
	wp->setDamageResist(DAMAGE_FIRE, 10);
	wp->setDamagePenalty(10);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_VoidTouchedShield()
{
	weaponSharedPtr wp(new weapon("Void-Touched Shield", SHIELD_TILE, TCODColor::darkPurple,
		"This shield phases in and out of reality, forming fissures into the Void."));
	wp->setDefence(5);
	wp->setDamageResist(DAMAGE_PROFANE, 50);
	wp->setDamagePenalty(5);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_DragonboneShield()
{
	weaponSharedPtr wp(new weapon("Dragonbone Shield", SHIELD_TILE, TCODColor::darkFlame,
		"This shield, forged from the bones of an ancient dragon, confers powerful protection from fire."));
	wp->setDefence(20);
	wp->setDamageResist(DAMAGE_FIRE, 50);
	wp->setDamagePenalty(15);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_EtherealShield()
{
	weaponSharedPtr wp(new weapon("Ethereal Shield", SHIELD_TILE, TCODColor::magenta,
		"A shield half-vanished from reality, pulsing with magical energy."));
	wp->setDefence(0);
	wp->setDamageResist(DAMAGE_MAGIC, 25);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_KhallesBellShield()
{
	weaponSharedPtr wp(new weapon("Khalle's Bell Shield", SHIELD_TILE, TCODColor::darkYellow,
		"The bent shield of Khalle, devout priestess of the lowlands, who decorated it with dozens of tiny bells. \
They ring in times of danger, bringing the favour of the old gods upon the lost."));
	wp->setDefence(10);
	wp->setDamageResist(DAMAGE_PROFANE, 10);
	wp->setDamageResist(DAMAGE_ELECTRIC, 10);
	wp->setDamagePenalty(10);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(75);
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr shield_SirPercivelsShield()
{
	weaponSharedPtr wp(new weapon("Sir Percivel's Shield", SHIELD_TILE, TCODColor::lightGreen,
		"Sir Percivel sought the truth of the Rose's decline. Among the ruins of Coren's former victories, he discovered \
what comes of those who rely on the strength of the old gods, and was immensely disturbed."));
	wp->setDefence(20);
	wp->setBleedResist(40);
	wp->setDamagePenalty(10);
	wp->makeOffhand();
	return wp;
}

//		Wands

weaponSharedPtr wand_DriftwoodWand()
{
	weaponSharedPtr wp(new weapon("Driftwood Wand", STAFF_TILE, TCODColor::sepia,
		"Someone made this chunk of wand in a desparate hurry."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setSpellPower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_PowerSurge());
	return wp;
}

weaponSharedPtr wand_FishmansToadstaff()
{
	weaponSharedPtr wp(new weapon("Fishman's Toadstaff", STAFF_TILE, TCODColor::darkLime,
		"Sometimes the toad impaled on the tip twitches. Is it still alive? Best not to dwell on it."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(4);
	wp->setSpellPower(75);
	wp->setSpecialAttack(ability_AcidInfusion());
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr wand_EtherealWand()
{
	weaponSharedPtr wp(new weapon("Ethereal Wand", STAFF_TILE, TCODColor::magenta,
		"A barely-visible ethereal wand, created by one of the Sparrows to channel arcane power. These spells were swift, but weak."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(4);
	wp->setSpellPower(80);
	wp->setSpecialAttack(ability_EtherealSurge());
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr wand_BleachwoodWand()
{
	weaponSharedPtr wp(new weapon("Bleachwood Wand", STAFF_TILE, TCODColor::lighterGrey,
		"The pale white wood of this wand is associated with the Wyrd, where white trees kept the shadows contained."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(4);
	wp->setSpellPower(120);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_Metamagic());
	return wp;
}

weaponSharedPtr wand_SparrowsStaff()
{
	weaponSharedPtr wp(new weapon("Sparrow's Staff", STAFF_TILE, TCODColor::fuchsia,
		"The crooked staff of the Dead Sparrow, who wrought it during his transitory visits to worlds beyond our own."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(8);
	wp->setSpellPower(80);
	wp->setSpecialAttack(ability_TransitorySurge());
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr wand_MoshkasSingingStaff()
{
	weaponSharedPtr wp(new weapon("Moshka's Singing Staff", STAFF_TILE, TCODColor::orange,
		"The staff of Moshka, who delved deep into arcane mysteries. In the end he claimed that the old gods themselves \
oversaw every realm of magic, but he went mad before his accusations could be proven. His unusual staff can cast both spells \
and divine prayers."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(4);
	wp->setSpellPower(80);
	wp->setDivinePower(80);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_StrengthOfMind());
	return wp;
}

weaponSharedPtr chime_ClericsCrackedChime()
{
	weaponSharedPtr wp(new weapon("Cleric's Cracked Chime", CHIME_TILE, TCODColor::darkYellow,
		"Ring this broken chime to politely ask favours of the gods."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_DivineFavour());
	return wp;
}

weaponSharedPtr chime_WyrdBellbranch()
{
	weaponSharedPtr wp(new weapon("Wyrd Bellbranch", CHIME_TILE, TCODColor::sepia,
		"The trees of the Wyrd once grew tiny singing bells on their branches."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(2);
	wp->setDivinePower(100);
	wp->setSpecialAttack(ability_WyrdChannel());
	wp->makeOffhand();
	return wp;
}

weaponSharedPtr chime_OrsylsProfaneChime()
{
	weaponSharedPtr wp(new weapon("Orsyl's Profane Chime", CHIME_TILE, TCODColor::lightPurple,
		"Once-holy chime of Orsyl, who stood against the Void for as long as he could. As the nightmare consumed him, he prayed to \
the old gods, but they did not answer."));
	wp->setSpellstoreSize(4);
	wp->setDivinePower(100);
	wp->setSpecialAttack(prayer_ProfaneRadiance());
	wp->makeOffhand();
	wp->isProfane = true;
	return wp;
}
