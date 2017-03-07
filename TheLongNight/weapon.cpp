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
		if (rune->addScalingType == SCALE_POISON && etype == EFFECT_POISON)
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
		else if (runestone->addScalingType == SCALE_ACID && dtype == DAMAGE_ACID)
			return true;
		else if (runestone->addScalingType == SCALE_COLD && dtype == DAMAGE_COLD)
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
	wp->setImage(loadWeaponImage("W_splintered sword"));
	return wp;
}

weaponSharedPtr weapon_ThinKnife()
{
	weaponSharedPtr wp(new weapon("Thin Knife", DAGGER_TILE, TCODColor::lightGrey,
		"A weapon for small, quick folks."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->setSpecialAttack(attack_Quickstep());
	wp->addScalingType(SCALE_DEX);
	wp->setImage(loadWeaponImage("W_thin knife"));
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
	wp->setImage(loadWeaponImage("W_straight sword"));
	return wp;
}

weaponSharedPtr weapon_Warhammer()
{
	weaponSharedPtr wp(new weapon("Warhammer", HAMMER_TILE, TCODColor::lightSepia,
		"A big ole hammer for smashing things."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->setSpecialAttack(attack_Knockback());
	wp->addScalingType(SCALE_STR);
	wp->setImage(loadWeaponImage("W_warhammer"));
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
	wp->setImage(loadWeaponImage("W_crow knight sword"));
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
	wp->setImage(loadWeaponImage("W_crow knife"));
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
	wp->setImage(loadWeaponImage("W_crow halfsword"));
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
	wp->setImage(loadWeaponImage("W_old crow's long knife"));
	return wp;
}

weaponSharedPtr weapon_NotchedGreatsword()
{
	weaponSharedPtr wp(new weapon("Notched Greatsword", SWORD_TILE, TCODColor::grey,
		"The blade of this huge sword is notched and marred, but it should still work."));
	wp->setBasicAttributes(100, SPEED_SLOW);
	wp->addScalingType(SCALE_STR);
	wp->setSpecialAttack(attack_MightyCleave());
	wp->setImage(loadWeaponImage("W_notched greatsword"));
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
	wp->setImage(loadWeaponImage("W_fishmans harpoon"));
	return wp;
}

weaponSharedPtr weapon_FishmansKnife()
{
	weaponSharedPtr wp(new weapon("Fishman's Knife", DAGGER_TILE, TCODColor::lime,
		"The short knives of the fishmen are weak but incredibly quick."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Fillet());
	wp->setImage(loadWeaponImage("W_fishmans knife"));
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
	wp->setImage(loadWeaponImage("W_holy warhammer"));
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
	wp->setImage(loadWeaponImage("W_blood knife"));
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
	wp->setImage(loadWeaponImage("W_holy scythe"));
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
	wp->setImage(loadWeaponImage("W_ethereal knife"));
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
	wp->setImage(loadWeaponImage("W_profane greatsword"));
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
	wp->setImage(loadWeaponImage("W_tentacle arm"));
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
	wp->setImage(loadWeaponImage("W_dragonbone sword"));
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
	wp->setImage(loadWeaponImage("W_serpent tooth"));
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
	wp->setImage(loadWeaponImage("W_crimson nail"));
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
	wp->setImage(loadWeaponImage("W_spiderbone shard"));
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
	wp->setImage(loadWeaponImage("W_sacrificial knife"));
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
	wp->setImage(loadWeaponImage("W_slave master whip"));
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
	wp->setImage(loadWeaponImage("W_poisonous claws"));
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
	wp->setImage(loadWeaponImage("W_energy sword"));
	return wp;
}

weaponSharedPtr weapon_SentinelsPike()
{
	weaponSharedPtr wp(new weapon("Sentinel's Pike", SPEAR_TILE, TCODColor::lightBlue,
		"Long pike of the Sentinels, set to guard the Bridge of Lords, who never flagged in their ancient duty."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_ELECTRIC, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Penetrate());
	wp->setImage(loadWeaponImage("W_sentinelspear"));
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
	wp->setImage(loadWeaponImage("W_bloody greataxe"));
	return wp;
}

weaponSharedPtr weapon_KhallesHolyScythe()
{
	weaponSharedPtr wp(new weapon("Khalle's Holy Scythe", SWORD_TILE, TCODColor::darkYellow,
		"The scythe of Khalle, who devoutly opposed the void, but eventually saw the uselessness of the old gods. \
According to certain storytellers, she once peered into Heaven itself, and what she saw there drove her to despair."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_ELECTRIC, 25);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_DEV);
	wp->setSpellstoreSize(1);
	wp->setDivinePower(50);
	wp->setSpecialAttack(prayer_BlessedRadiance());
	wp->setImage(loadWeaponImage("W_super holy scythe"));
	return wp;
}

weaponSharedPtr weapon_SirPercivelsSword()
{
	weaponSharedPtr wp(new weapon("Sir Percivel's Sword", SWORD_TILE, TCODColor::green,
		"Sir Percivel sought the truth of the Rose's decline. Amongst all the ruined souls \
in the forsaken mausoleums of the lowlands, he found only one who would still heed him."));
	wp->setBasicAttributes(75, SPEED_NORMAL);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_PercivelsFire());
	wp->setImage(loadWeaponImage("W_green knight sword"));
	wp->inSirPercivelsSet = true;
	return wp;
}

weaponSharedPtr weapon_VoidfireKnife()
{
	weaponSharedPtr wp(new weapon("Voidfire Knife", DAGGER_TILE, TCODColor::lighterPurple,
		"A short knife that glows with dark flame."));
	wp->setBasicAttributes(0, SPEED_FAST);
	wp->addDamageType(DAMAGE_PROFANE, 10);
	wp->addDamageType(DAMAGE_FIRE, 15);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_KnifeCast());
	wp->setImage(loadWeaponImage("W_voidfire knife"));
	return wp;
}

weaponSharedPtr weapon_LightningWarhammer()
{
	weaponSharedPtr wp(new weapon("Lightning Warhammer", HAMMER_TILE, TCODColor::lightPurple,
		"Massive warhammer of the Void, flashing with arcane lightning."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addDamageType(DAMAGE_ELECTRIC, 50);
	wp->addScalingType(SCALE_STR);
	wp->setSpecialAttack(attack_LightningSlam());
	wp->setImage(loadWeaponImage("W_lightning warhammer"));
	return wp;
}

weaponSharedPtr weapon_IetrasFlameScythe()
{
	weaponSharedPtr wp(new weapon("Ietra's Flame Scythe", SPEAR_TILE, TCODColor::flame,
		"Scythe of Ietra, imbued with the secret fire that lies at the heart of the void. When \
her tomb was sealed, she resolved that the secret of the Angel would never leave its depths."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_PROFANE, 25);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_BillowingFlames());
	wp->setImage(loadWeaponImage("W_iteras scythe"));
	return wp;
}

weaponSharedPtr weapon_EtherealGreatsword()
{
	weaponSharedPtr wp(new weapon("Ethereal Greatsword", SWORD_TILE, TCODColor::lightBlue,
		"An unreal blade infused with potent magical energy."));
	wp->setBasicAttributes(0, SPEED_SLOW);
	wp->addDamageType(DAMAGE_MAGIC, 100);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_ARC);
	wp->setSpecialAttack(attack_EtherealRay());
	return wp;
}

weaponSharedPtr weapon_MoonpaleScythe()
{
	weaponSharedPtr wp(new weapon("Moon-Pale Scythe", SPEAR_TILE, TCODColor::lighterBlue,
		"Half-unreal scythe wielded by the Pale Shade. According to certain storytellers, the Shade was all \
that remained of the Moon-Pale King, who wasted away in his abandoned palace as it drifted through the Void."));
	wp->setBasicAttributes(0, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_MAGIC, 25);
	wp->addScalingType(SCALE_ARC);
	wp->setSpecialAttack(attack_PaleSlash());
	wp->setImage(loadWeaponImage("W_moonpale scythe"));
	return wp;
}

weaponSharedPtr weapon_GravekeeperSpear()
{
	weaponSharedPtr wp(new weapon("Gravekeeper Spear", SPEAR_TILE, TCODColor::lightSepia,
		"Spear infected with rot. The gravekeepers, abandoned by Farin, still fulfill their ancient purpose."));
	wp->setBasicAttributes(25, SPEED_NORMAL);
	wp->addStatusEffect(EFFECT_PLAGUE, 20);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_Quickstep());
	return wp;
}

weaponSharedPtr weapon_SelvixsHookblade()
{
	weaponSharedPtr wp(new weapon("Selvix's Hookblade", SWORD_TILE, TCODColor::lightAmber,
		"Curved shortsword used by Gravekeeper Selvix, infused with poison and rot."));
	wp->setBasicAttributes(15, SPEED_FAST);
	wp->addStatusEffect(EFFECT_PLAGUE, 10);
	wp->addScalingType(SCALE_DEX);
	wp->setSpecialAttack(attack_PoisonHook());
	return wp;
}

weaponSharedPtr weapon_FarinsTorch()
{
	weaponSharedPtr wp(new weapon("Farin's Torch", DAGGER_TILE, TCODColor::darkFlame,
		"Enchanted torch of Farin, who carried it into the depths of the earth, promising that he would bring light to the dead."));
	wp->setBasicAttributes(0, SPEED_NORMAL);
	wp->addDamageType(DAMAGE_FIRE, 25);
	wp->addScalingType(SCALE_ARC);
	wp->setSpecialAttack(attack_FlameBreath());
	return wp;
}

weaponSharedPtr weapon_RatboneCleaver()
{
	weaponSharedPtr wp(new weapon("Ratbone Cleaver", SWORD_TILE, TCODColor::amber,
		"Cleaver forged from shards of ratbone, drenched with ichorous toxins and crawling plague."));
	wp->setBasicAttributes(50, SPEED_SLOW);
	wp->addStatusEffect(EFFECT_POISON, 20);
	wp->addScalingType(SCALE_STR);
	wp->setSpecialAttack(attack_RatCleave());
	return wp;
}

weaponSharedPtr weapon_DeathlessBlade()
{
	weaponSharedPtr wp(new weapon("Deathless Blade", SWORD_TILE, TCODColor::pink,
		"Glowing sword of the Deathless Knight, infused with the raw energy of obliteration."));
	wp->setBasicAttributes(50, SPEED_NORMAL);
	wp->setSpecialAttack(attack_Deathstrike());
	return wp;
}

weaponSharedPtr weapon_GreatswordOfTheAncientLords()
{
	weaponSharedPtr wp(new weapon("Greatsword of the Ancient Lords", SWORD_TILE, TCODColor::lightFuchsia,
		"The ancient sword of the first lords. Its eternal flame has been corrupted by death and plague."));
	wp->setBasicAttributes(20, SPEED_SLOW);
	wp->addStatusEffect(EFFECT_PLAGUE, 20);
	wp->addDamageType(DAMAGE_FIRE, 20);
	wp->addDamageType(DAMAGE_ACID, 20);
	wp->addDamageType(DAMAGE_COLD, 20);
	wp->addDamageType(DAMAGE_ELECTRIC, 20);
	wp->addScalingType(SCALE_STR);
	wp->addScalingType(SCALE_DEX);
	wp->addScalingType(SCALE_ARC);
	wp->addScalingType(SCALE_DEV);
	wp->setSpecialAttack(attack_BlightingInfusion());
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
	wp->setImage(loadWeaponImage("S_battered wooden shield"));
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
	wp->setImage(loadWeaponImage("S_wyrd shield"));
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
	wp->setImage(loadWeaponImage("S_battered steel shield"));
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
	wp->setImage(loadWeaponImage("S_holy shield"));
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
	wp->setImage(loadWeaponImage("S_ethereal shield"));
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
	wp->setImage(loadWeaponImage("S_dragonbone shield"));
	return wp;
}

weaponSharedPtr shield_EtherealShield()
{
	weaponSharedPtr wp(new weapon("Ethereal Shield", SHIELD_TILE, TCODColor::magenta,
		"A shield half-vanished from reality, pulsing with magical energy."));
	wp->setDefence(0);
	wp->setDamageResist(DAMAGE_MAGIC, 25);
	wp->makeOffhand();
	wp->setImage(loadWeaponImage("S_ethereal shield"));
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

weaponSharedPtr shield_GhostlyShield()
{
	weaponSharedPtr wp(new weapon("Ghostly Shield", SHIELD_TILE, TCODColor::lightestPink,
		"Half-unreal shield, engraved with the crest of the Moon-Pale King. As his palace faded into the moonlight, \
the crest slowly changed into the shape of a crooked finger."));
	wp->setDamageResist(DAMAGE_MAGIC, 15);
	wp->setDamageResist(DAMAGE_COLD, 15);
	wp->setDamageResist(DAMAGE_ACID, 15);
	wp->makeOffhand();
	wp->setImage(loadWeaponImage("S_ghost shield"));
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
	wp->setImage(loadWeaponImage("S_green knight shield"));
	wp->inSirPercivelsSet = true;
	return wp;
}

weaponSharedPtr shield_RottenBeastShield()
{
	weaponSharedPtr wp(new weapon("Rotten Beast Shield", SHIELD_TILE, TCODColor::lightLime,
		"Shield rotted from extensive contact with the toxins of the tombs."));
	wp->setDefence(10);
	wp->setPoisonResist(20);
	wp->setPlagueResist(20);
	wp->makeOffhand();
	wp->setDamagePenalty(5);
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
	wp->setImage(loadWeaponImage("M_driftwood wand"));
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
	wp->setImage(loadWeaponImage("M_toadstaff"));
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
	wp->setImage(loadWeaponImage("M_ethereal wand"));
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

weaponSharedPtr wand_GottricsWand()
{
	weaponSharedPtr wp(new weapon("Gottric's Wand", STAFF_TILE, TCODColor::lightPurple,
		"Quite a simple wand, after all. Gottric was no great master of the art - only a curious scholar."));
	wp->setBasicAttributes(5, SPEED_NORMAL);
	wp->setSpellstoreSize(1);
	wp->setSpellPower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_GottricsPartingCall());
	return wp;
}

weaponSharedPtr chime_ClericsCrackedChime()
{
	weaponSharedPtr wp(new weapon("Cleric's Cracked Chime", CHIME_TILE, TCODColor::darkYellow,
		"Ring this broken chime to politely ask favours of the gods."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(6);
	wp->setDivinePower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_DivineFavour());
	wp->setImage(loadWeaponImage("M_cracked chime"));
	return wp;
}

weaponSharedPtr chime_GreenKnightsChime()
{
	weaponSharedPtr wp(new weapon("Green Knight's Chime", CHIME_TILE, TCODColor::green,
		"Relic of a time before the decline of the Rose."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(3);
	wp->setDivinePower(120);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_MemoryOfLight());
	return wp;
}

weaponSharedPtr chime_RotbloodBell()
{
	weaponSharedPtr wp(new weapon("Rotblood Bell", CHIME_TILE, TCODColor::amber,
		"A bell drenched in thick poison. The cry of this toxic chime calls to the old gods, who slumber in their putrid caves, \
waiting for the end of the world."));
	wp->setBasicAttributes(5, SPEED_SLOW);
	wp->setSpellstoreSize(4);
	wp->setDivinePower(100);
	wp->makeOffhand();
	wp->setSpecialAttack(ability_Rotcall());
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
	wp->setImage(loadWeaponImage("M_wyrd bellbranch"));
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
	wp->setImage(loadWeaponImage("M_unholy chime"));
	return wp;
}
