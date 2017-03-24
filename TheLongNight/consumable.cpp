#include "consumable.h"



consumable::consumable(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_CONSUMABLE, description)
{
	amount.setTo(1);
}

consumable::~consumable()
{
}

/*
Returns amount left combined with name.
*/
std::string consumable::getMenuName()
{
	return getName() + " x" + std::to_string(getAmountLeft());
}

consumableSharedPtr consumable_StarwaterDraught()
{
	consumableSharedPtr c(new consumable("Starwater Draught", FLASK_TILE, TCODColor::cyan,
		"Flask of water touched by starlight, which confers healing properties. (Press [e] to use your selected item.)"));
	c->addEffect(RESTORE_HEALTH, 25);
	return c;
}

consumableSharedPtr consumable_InvigoratingTea()
{
	consumableSharedPtr c(new consumable("Invigorating Tea", FLASK_TILE, TCODColor::green,
		"A specially brewed tea that restores vigour."));
	c->addEffect(RESTORE_VIGOUR, 5);
	return c;
}

consumableSharedPtr consumable_PilgrimsFingerbone()
{
	consumableSharedPtr c(new consumable("Pilgrim's Fingerbone", FINGERBONE_TILE, TCODColor::lightestYellow,
		"Fingerbone of a lonesome pilgrim, who dreamt eternally of returning home. But the way home was not easy to find. \
(You can use this item from your inventory at any time.)"));
	c->addEffect(WARP_TO_SAVE_POINT, 0);
	c->consumeOnUse = false;
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr consumable_TinyRedFlower()
{
	consumableSharedPtr c(new consumable("Tiny Red Flower", FLOWER_TILE, TCODColor::crimson,
		"A small flower of the kind Princess Yulia once wore in her hair. Clears Bleed buildup. (Once the Bleed bar fills, you \
take a large amount of damage on your next two turns. Use this flower to prevent damage or clear the bar.)"));
	c->addEffect(REMOVE_BLEED, 0);
	return c;
}

consumableSharedPtr consumable_TinyGreenFlower()
{
	consumableSharedPtr c(new consumable("Tiny Green Flower", FLOWER_TILE, TCODColor::green,
		"These gentle green flowers purge toxins from the body. (Once the Poison bar fills, you slowly take damage until you die \
or rest. Use this flower to clear the effect.)"));
	c->addEffect(REMOVE_POISON, 0);
	return c;
}

consumableSharedPtr consumable_PutridFlower()
{
	consumableSharedPtr c(new consumable("Putrid Flower", FLOWER_TILE, TCODColor::amber,
		"A foul-smelling flower that grows underground. According to certain storytellers, Farin \
cultivated these in secret, afraid that he would someday regret bringing plague upon the world. (Plague reduces your maximum \
hitpoints until you rest or die. Use this flower to clear the effect.)"));
	c->addEffect(REMOVE_PLAGUE, 0);
	return c;
}

consumableSharedPtr consumable_BloodDrinkersEyes()
{
	consumableSharedPtr c(new consumable("Blood Drinker's Eyes", EYEBALL_TILE, TCODColor::crimson,
		"The reddened eyes of one who has consumed too much blood. According to the profane scriptures of Pash, these eyes \
contain profound power when eaten."));
	c->addEffect(BLEED_DAMAGE_FACTOR, 2);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_IntoxicatingWine()
{
	consumableSharedPtr c(new consumable("Intoxicating Wine", BOTTLE_TILE, TCODColor::darkRed,
		"This heady draught is said to purge the mind of material desires while enhancing one's will."));
	c->addEffect(INCREASE_PRAYER_POWER, 25);
	c->addEffect(APPLY_DAMAGE_PENALTY, 15);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_WitchwaterFlask()
{
	consumableSharedPtr c(new consumable("Witchwater Flask", BOTTLE_TILE, TCODColor::fuchsia,
		"A flask of invigorating witchwater, said to break down the barriers between our world and reality. Moshka \
was a great believer in witchwater, and consumed it in copious amounts."));
	c->addEffect(INCREASE_SPELL_POWER, 25);
	c->addEffect(APPLY_DAMAGE_PENALTY, 15);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_BlackHoney()
{
	consumableSharedPtr c(new consumable("Black Honey", BOTTLE_TILE, TCODColor::darkPurple,
		"Black honey, once the principle export of the village, was said to induce vivid dreams. But if one's life is filled \
with bloodshed, one's dreams are unlikely to bring peace."));
	c->addEffect(GAIN_DAMAGE_BUFF, 25);
	c->addEffect(INCREASE_SPELL_POWER, -15);
	c->addEffect(INCREASE_PRAYER_POWER, -15);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_PutridBrew()
{
	consumableSharedPtr c(new consumable("Putrid Brew", FLASK_TILE, TCODColor::darkLime,
		"A profoundly toxic drink, said to contain the putrid rage of the dead gods that rot in the heart of the Void."));
	c->addEffect(APPLY_POISON_DAMAGE, 40);
	c->addEffect(DAMAGE_WHEN_POISONED, 50);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_GodsbloodBrew()
{
	consumableSharedPtr c(new consumable("Godsblood Brew", FLASK_TILE, TCODColor::lightChartreuse,
		"Brew concocted from the boiling blood of the old gods, whose remains smoulder in the heart of the Void."));
	c->addEffect(GAIN_MAX_HEALTH, 50);
	c->addsPermanentBuff = true;
	return c;
}

consumableSharedPtr consumable_BlackTarLiquor()
{
	consumableSharedPtr c(new consumable("Black Tar Liquor", BOTTLE_TILE, TCODColor::darkPurple,
		"A flagon of spiced liquor, made from the tar that floats in the Void. Foul to drink, but it has certain powerful \
properties that the Blackwatch treasures."));
	c->addEffect(SCALE_NEXT_ATTACK, 100);
	c->setAmount(5);
	return c;
}

consumableSharedPtr consumable_PurifiedStarwater()
{
	consumableSharedPtr c(new consumable("Purified Starwater", FLASK_TILE, TCODColor::lightestCyan,
		"Starwater descends from the emptiness of the Void. This flask of starwater is drawn from the pool at the heart of the world, \
and is impossible to forget."));
	c->addEffect(RESTORE_HEALTH, 500);
	c->addEffect(REMOVE_BLEED);
	c->addEffect(REMOVE_PLAGUE);
	c->addEffect(REMOVE_POISON);
	return c;
}

consumableSharedPtr ranged_ThrowingKnives()
{
	consumableSharedPtr c(new consumable("Throwing Knives", DAGGER_TILE, TCODColor::lightGrey,
		"Short knives to chuck at your enemies. (You can equip up to 5 consumables at once. They are fully refreshed each time you \
die or rest. Select a consumable by pressing [c], and press [e] to use it.)"));
	c->setRangedAttack(spellSharedPtr(new spell("Throwing Knife", c->getColor(), 5, 25)));
	c->add(4); //Comes in a stack
	return c;
}

consumableSharedPtr ranged_HeavyJavelin()
{
	consumableSharedPtr c(new consumable("Heavy Javelin", SPEAR_TILE, TCODColor::sepia,
		"Deadly javelin for throwing at your nemesis."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 8, 75)));
	c->add(1);
	return c;
}

consumableSharedPtr ranged_LaceratingKnives()
{
	consumableSharedPtr c(new consumable("Lacerating Knives", DAGGER_TILE, TCODColor::lightCrimson,
		"Short knives with lacerating spines, designed to carve the flesh of your enemies."));
	c->setRangedAttack(spellSharedPtr(new spell("Lacerating Knife", c->getColor(), 5, 10, APPLY_BLEED_DAMAGE, 15)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_PoisonThrowingKnives()
{
	consumableSharedPtr c(new consumable("Poison Throwing Knives", DAGGER_TILE, TCODColor::green,
		"Knives coated in poison. Preferred weapon of the grey thieves that escaped from Hightower."));
	c->setRangedAttack(spellSharedPtr(new spell("Poison Throwing Knives", c->getColor(), 5, 10, APPLY_POISON_DAMAGE, 15)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_CorrodingJar()
{
	consumableSharedPtr c(new consumable("Corroding Jar", BOMB_TILE, TCODColor::lime,
		"A jar filled with acid. Throw at armoured enemies to penetrate their defences."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_ACID_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_VoidEssenceJar()
{
	consumableSharedPtr c(new consumable("Void-Essence Jar", BOMB_TILE, TCODColor::purple,
		"A jar filled with cursed oil, designed to shatter on contact."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_PROFANE_DAMAGE, 45)));
	c->add(2);
	return c;
}

consumableSharedPtr ranged_PyromancersFlask()
{
	consumableSharedPtr c(new consumable("Pyromancer's Flask", BOMB_TILE, TCODColor::flame,
		"A jar of volatile oil, ready to burst into flame on contact."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_FIRE_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_WitchsJar()
{
	consumableSharedPtr c(new consumable("Witch's Jar", BOMB_TILE, TCODColor::magenta,
		"Jars of magical oil, often carried by wizards for use as a last resort."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_MAGIC_DAMAGE, 40)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_LightingJavelin()
{
	consumableSharedPtr c(new consumable("Lightning Javelin", SPEAR_TILE, TCODColor::yellow,
		"Javelin tipped with electric oil."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 25, APPLY_ELECTRIC_DAMAGE, 25)));
	c->add(3);
	return c;
}

consumableSharedPtr ranged_FrostKnives()
{
	consumableSharedPtr c(new consumable("Frost Knives", DAGGER_TILE, TCODColor::cyan,
		"Knives tipped with frozen oil to chill the bones."));
	c->setRangedAttack(spellSharedPtr(new spell(c->getName(), c->getColor(), 5, 5, APPLY_COLD_DAMAGE, 20)));
	c->add(4);
	return c;
}

consumableSharedPtr ranged_WarpingJavelin()
{
	consumableSharedPtr c(new consumable("Warping Javelin", SPEAR_TILE, TCODColor::lighterBlue,
		"These javelins, infused with the power of the void, phase in and out of reality, and always hit their target."));
	c->setRangedAttack(ability_WarpingJavelin());
	c->add(4);
	return c;
}

consumableSharedPtr oil_PyromancersOil()
{
	consumableSharedPtr c(new consumable("Pyromancer's Oil", VIAL_TILE, TCODColor::darkFlame,
		"If properly treated, pyromancer's oil can be convinced to burn rather than explode. (A burning creature takes extra \
damage the round after.)"));
	c->setWeaponBuff(new weaponBuff(DAMAGE_FIRE, 15));
	return c;
}

consumableSharedPtr oil_CorrosiveOil()
{
	consumableSharedPtr c(new consumable("Corrosive Oil", VIAL_TILE, TCODColor::lime,
		"Somehow, rather than corrode the blade, this oil coats targets in caustic acid."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_ACID, 15));
	return c;
}

consumableSharedPtr oil_FrozenOil()
{
	consumableSharedPtr c(new consumable("Frozen Oil", VIAL_TILE, TCODColor::lightBlue,
		"The alchemists of the Winter Court created this curious oil, which coats a blade in ice."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_COLD, 15));
	return c;
}

consumableSharedPtr oil_HolyWater()
{
	consumableSharedPtr c(new consumable("Holy Water", VIAL_TILE, TCODColor::lightestBlue,
		"This vial of blessed water adds a sheen of pure holy energy to a blade."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_BLESSED, 15));
	return c;
}

consumableSharedPtr oil_CursedWater()
{
	consumableSharedPtr c(new consumable("Cursed Water", VIAL_TILE, TCODColor::lightPurple,
		"Vial of water cursed by profane priests, which now bubbles with unholy ichor."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_PROFANE, 15));
	return c;
}

consumableSharedPtr oil_HangmansBlood()
{
	consumableSharedPtr c(new consumable("Hangman's Blood", VIAL_TILE, TCODColor::darkerRed,
		"The pure blood of a hangman is enriched by those he kills, and in death, a hangman's blood \
is said to carry the stench of a thousand graves."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_PHYSICAL, 15));
	return c;
}

consumableSharedPtr oil_WitchsOoze()
{
	consumableSharedPtr c(new consumable("Witch's Ooze", VIAL_TILE, TCODColor::magenta,
		"The blood of those who dabble in the arcane slowly thickens, and congeals into a magic-drenched paste."));
	c->setWeaponBuff(new weaponBuff(DAMAGE_MAGIC, 15));
	return c;
}

consumableSharedPtr oil_LightningOil()
{
	consumable* c = new consumable("Lightning Oil", VIAL_TILE, TCODColor::lightPurple,
		"Oil infused with lightning, used by the bone-whisperers of the deep.");
	c->setWeaponBuff(new weaponBuff(DAMAGE_ELECTRIC, 15));
	return consumableSharedPtr(c);
}

consumableSharedPtr oil_DeepRedOil()
{
	consumableSharedPtr c(new consumable("Deep Red Oil", VIAL_TILE, TCODColor::crimson,
		"Oil of a deep red shade. Not made from blood, but certainly has an affinity for it."));
	c->setWeaponBuff(new weaponBuff(EFFECT_BLEED, 10));
	return c;
}

consumableSharedPtr oil_RotbloodOil()
{
	consumableSharedPtr c(new consumable("Rotblood Oil", VIAL_TILE, TCODColor::sepia,
		"Oil made from the rotten blood of the vile dead. Use of this oil is forbidden in any land the sunlight touched."));
	c->setWeaponBuff(new weaponBuff(EFFECT_PLAGUE, 10));
	return c;
}


/*
	Items that give fragments when devoured
*/

consumableSharedPtr gem_DullGreyGemstone()
{
	consumableSharedPtr c(new consumable("Dull Grey Gemstone", FRAGMENT_GLYPH, TCODColor::lighterGrey,
		"Meagre gemstone, perhaps treasured by a peasant. (You can use this from your inventory at any time to gain fragments. \
Recall that fragments are lost upon death.)"));
	c->addEffect(ACQUIRE_FRAGMENTS, 300);
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr gem_SmallShiningGemstone()
{
	consumableSharedPtr c(new consumable("Small Shining Gemstone", FRAGMENT_GLYPH, TCODColor::lightPink,
		"Minor gemstone that shines with inner glory."));
	c->addEffect(ACQUIRE_FRAGMENTS, 600);
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr gem_NobleGlowingGemstone()
{
	consumableSharedPtr c(new consumable("Noble Glowing Gemstone", FRAGMENT_GLYPH, TCODColor::lightOrange,
		"Glorious gemstone of a noble aristocrat."));
	c->addEffect(ACQUIRE_FRAGMENTS, 1200);
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr gem_KingsGiantGemstone()
{
	consumableSharedPtr c(new consumable("King's Giant Gemstone", FRAGMENT_GLYPH, TCODColor::lighterPurple,
		"Gemstone once embedded in a king's crown, imbued with ancient power."));
	c->addEffect(ACQUIRE_FRAGMENTS, 1800);
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr gem_GodlessGemstone()
{
	consumableSharedPtr c(new consumable("Godless Gemstone", FRAGMENT_GLYPH, TCODColor::lightestYellow,
		"Gemstone engorged with the lost power of the old gods."));
	c->addEffect(ACQUIRE_FRAGMENTS, 2400);
	c->oneUseOnly = true;
	return c;
}

consumableSharedPtr bell_VoidwalkersDancingBell()
{
	consumableSharedPtr c(new consumable("Voidwalker's Dancing Bell", BELL_TILE, TCODColor::purple,
		"This bell pulses with the energy of the Void. You sense that to use it would be a dire sin. (You can use this directly \
from your inventory.)"));
	c->consumeOnUse = false;
	c->oneUseOnly = true;
	c->addEffect(TELEPORT_TO_VOID);
	return c;
}

consumableSharedPtr bell_VoidwalkersReturningBell()
{
	consumableSharedPtr c(new consumable("Voidwalker's Returning Bell", BELL_TILE, TCODColor::lightPurple,
		"This bell radiates warmth. From within the void, it whispers to every walker of the dark, promising them that \
they will return home someday. (You can use this directly from your inventory.)"));
	c->consumeOnUse = false;
	c->oneUseOnly = true;
	c->addEffect(TELEPORT_BACK_FROM_VOID);
	return c;
}

consumableSharedPtr horn_HeraldsWhiteHorn()
{
	consumableSharedPtr c(new consumable("Herald's White Horn", BELL_TILE, TCODColor::white,
		"The silver horn of the Herald, who comes in the White Fog. According to certain storytellers, his presence signifies the coming of the \
Lord of Fallow Fields, who rises from the realms of the old gods to bring final judgement upon the world."));
	c->consumeOnUse = false;
	c->addEffect(RESTORE_VIGOUR, 10);
	c->addEffect(APPLY_UNTYPED_DAMAGE, 100);
	return c;
}

consumableSharedPtr misc_BrandOfTheEmissary()
{
	consumableSharedPtr c(new consumable("Brand of the Emissary", BRAND_TILE, TCODColor::darkFlame,
		"Brand used by the Emissaries to mark sinners. The brand is agonizing, but it inspired a spiritual fervour in its victims, \
proof of the true power of the old gods."));
	c->consumeOnUse = false;
	c->addEffect(APPLY_FIRE_DAMAGE, 30);
	c->addEffect(SCALE_NEXT_PRAYER, 50);
	return c;
}
