#include "monster.h"




monster::monster() : person()
{
}

monster::monster(std::string name, int tileCode, TCODColor color, std::string description) : 
	person(name, tileCode, color, description)
{
	//We're not the player! That's kind of our defining feature
	isPlayer = false;
	//Default to 100% spell power
	setSpellPower(100);
	setDivinePower(100);
}

/*
Quick and easy way to set our basic melee stats.
Meant for monster constructors.
*/
void monster::setMeleeStats(int damage, int speed)
{
	this->baseMeleeDamage = damage;
	this->baseAttackSpeed = speed;
}

/*
Quick and easy way to set stats related to movement.
*/
void monster::setMoveStats(int speed)
{
	this->baseMoveSpeed = speed;
}

/*
Quick and easy way to set defence/damage resist.
*/
void monster::setDefence(damageType dtype, int defence)
{
	if (dtype == DAMAGE_PHYSICAL)
		baseDefence = defence;
	else
		damageResist.at(dtype) = defence;
}

void monster::setHealth(int hp)
{
	this->health.setTo(hp);
}


/*
	SPAWNING NEW MONSTERS
*/


/*
Random roll to decide if we want to spawn a thing
*/
bool monster::wantsToSpawn()
{
	//We sometimes spawn creatures at a certain HP threshold
	if (health.getValue() <= spawnAtHealthThreshold) {
		spawnAtHealthThreshold = 0;
		return true;
	}
	//Otherwise, random odds
	int r = randint(1, 100);
	return r <= spawnChance;
}


/*
Pick a random monster from our spawn list.
*/
monsterSharedPtr monster::getRandomSpawn()
{
	int i = randrange(spawnList.size());
	std::string handle = spawnList.at(i);
	return getMonsterByHandle(handle);
}



///*
//For shopkeeper NPCs.
//*/
//void monster::addItemToStock(itemSharedPtr it, int price)
//{
//	it->isGlittery = false;
//	it->setPrice(price);
//	stock.push_back(it);
//}
//
///*
//These items are unlocked if the player has a specific key.
//*/
//void monster::addStockUnlock(itemSharedPtr it, int price, std::string unlockCode)
//{
//	it->isGlittery = false;
//	it->setPrice(price);
//	stockUnlocks.push_back(new unlockableStock(it,unlockCode));
//}
//
///*
//Cause we bought something!
//*/
//void monster::removeItemFromStock(itemSharedPtr it)
//{
//	auto iter = std::find(stock.begin(), stock.end(), it);
//	if (iter != stock.end())
//		stock.erase(iter);
//}
//
//
///*
//See if the given person has a key that will unlock additional stock items.
//*/
//void monster::checkForStockUnlocks(personSharedPtr unlocker)
//{
//	//Keep track of stuff to remove, if any
//	stockUnlockVector toRemove;
//	//Have a look
//	for (auto unlock : stockUnlocks) {
//		if (unlocker->hasKey(unlock->unlockCode)) {
//			//Yup, here's one!
//			stock.push_back(unlock->it);
//			//And make sure we only add it once!
//			toRemove.push_back(unlock);
//		}
//	}
//	//Remove anything that we unlocked from the list to ensure it can't be unlocked twice
//	for (auto rem : toRemove) {
//		auto iter = std::find(stockUnlocks.begin(), stockUnlocks.end(), rem);
//		stockUnlocks.erase(iter);
//	}
//}


/*
	CHIT CHAT
*/


/*
Load in dialogue from a file.
*/
void monster::loadDialogue(std::string filename)
{
	//Create the file
	std::ifstream dfile(filename);
	//Load in lines of dialogue, one at a time
	std::string line;
	while (getline(dfile, line)) {
		addDialogue(line);
	}
	//Done, close file
	dfile.close();
}


/*
Returns the next line of dialogue that we'll say.
*/
std::string monster::getNextDialogueLine()
{
	//See if we even have any dialogue
	if (dialogue.size() == 0)
		return "";
	//If we do, figure out what line we're on
	if (atChatLine < dialogue.size() - 1)
		atChatLine++;
	std::string line = dialogue.at(atChatLine);
	return line;
}


/*
Special unlocks
*/


void monster::addSpecialDialogue(std::string key, std::string text)
{
	itemUnlockKeys.push_back(key);
	itemUnlockDialogue.push_back(text);
}


/*
Returns dialogue for the given key-item, or a blank string if there isn't any.
*/
std::string monster::getDialogueForKey(std::string key)
{
	for (int i = 0; i < itemUnlockKeys.size(); i++) {
		if (itemUnlockKeys.at(i) == key)
			return itemUnlockDialogue.at(i);
	}
	return "";
}






/*
Monster definitions
*/




monsterSharedPtr monster_DrownedCrew()
{
	monsterSharedPtr m(new monster("Drowned Crew", DROWNED_DEAD_TILE, TCODColor::lightGreen,
		"Rotted ruins of a crewmember. How tragic!"));
	m->setMeleeStats(10, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(10);
	m->setFragmentsDropped(1);
	return m;
}

monsterSharedPtr monster_ThinWretch()
{
	monsterSharedPtr m(new monster("Thin Wretch", WRETCH_TILE, TCODColor::darkGreen,
		"The haggard eyes of this twisted wretch suggests that it knows something you don't."));
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(50);
	m->setFragmentsDropped(5);
	return m;
}

monsterSharedPtr monster_TimidWretch()
{
	monsterSharedPtr m(new monster("Timid Wretch", WRETCH_TILE, TCODColor::darkerGreen,
		"This wretched creature hangs back, its eyes glowing with wrath."));
	m->setMeleeStats(15, SPEED_FAST);
	m->setMoveStats(SPEED_SLOW);
	m->setHealth(50);
	m->addSpellKnown(ability_Charge());
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(10);
	return m;
}

monsterSharedPtr monster_BloatedWretch()
{
	monsterSharedPtr m(new monster("Bloated Wretch", BLOATED_WRETCH_TILE, TCODColor::darkLime,
		"This bloated horror is swollen with seawater. Perhaps it was hungrier than the others."));
	m->setMeleeStats(25, SPEED_SLOW);
	m->setMoveStats(SPEED_SLOW);
	m->setHealth(100);
	m->addSpellKnown(ability_Burst());
	m->setSpellCastChance(25);
	m->setFragmentsDropped(20);
	return m;
}

/*
Coruscating Beach boss
*/
monsterSharedPtr boss_TheWretchedMass()
{
	monsterSharedPtr m(new monster("The Wretched Mass", WRETCHED_MASS_TILE, TCODColor::darkestGreen,
		"A mass of wretches fused together into a gruesome amalgamation. It shrieks and flops towards you, its many limbs \
flailing."));
	m->setHealth(400);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_SLOW);
	m->addSpawnableCreature("thin_wretch");
	m->setSpawnChance(25);
	m->addItemDrop(key_WretchsKey());
	m->addItemDrop(heart_WretchedHeart());
	m->isBoss = true;
	m->setFragmentsDropped(200);
	return m;
}

/*
Pilgrim's Road
*/

monsterSharedPtr monster_CrowKnight()
{
	monsterSharedPtr m(new monster("Crow Knight", CROW_KNIGHT_TILE, TCODColor::lightGrey,
		"A knight in dark-feathered armour, holding a broken sword."));
	m->setHealth(100);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(20, SPEED_NORMAL);
	m->equipItem(weapon_CrowKnightSword());
	m->setFragmentsDropped(25);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	return m;
}

monsterSharedPtr monster_CrowThief()
{
	monsterSharedPtr m(new monster("Crow Thief", CROW_THIEF_TILE, TCODColor::lightGreen,
		"A feathery mask covers this sneakthief's face, and yellow orbs peer through. Surely this is not a human creature?"));
	m->setHealth(50);
	m->setMoveStats(SPEED_FAST);
	m->setMeleeStats(10, SPEED_FAST);
	m->equipItem(weapon_CrowKnife());
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_CrowArcher()
{
	monsterSharedPtr m(new monster("Crow Archer", CROW_KNIGHT_TILE, TCODColor::lightBlue,
		"A knight in dark-feathered armour, holding a battered crossbow."));
	m->setHealth(100);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->addSpellKnown(spellSharedPtr(new spell("Crow Arrow", TCODColor::grey, 8, 10)));
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_ForlornCrowKnight()
{
	monsterSharedPtr m(new monster("Forlorn Crow Knight", CROW_KNIGHT_TILE, TCODColor::crimson,
		"Something has made this knight sad."));
	m->setHealth(200);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weapon_CrowHalfsword());
	m->addSpellKnown(attack_Quickstep());
	m->setSpellCastChance(20);
	m->setFragmentsDropped(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->addItemDrop(weapon_CrowHalfsword());
	return m;
}

monsterSharedPtr monster_TheOldCrow()
{
	monsterSharedPtr m(new monster("The Old Crow", THE_OLD_CROW_TILE, TCODColor::lightCrimson,
		"The tallest of the crow knights moves with gaunt grace, and flicks a knife between his long fingers."));
	m->setHealth(500);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weapon_CrowKnife());
	m->addSpellKnown(attack_Quickstep());
	m->setSpellCastChance(20);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setBleedResist(45);
	m->isBoss = true;
	m->setFragmentsDropped(400);
	m->addItemDrop(key_OldCrowsKey());
	m->addItemDrop(heart_OldCrowsHeart());
	return m;
}


//Flooded Lowlands


monsterSharedPtr monster_StarvingFishman()
{
	monsterSharedPtr m(new monster("Starving Fishman", FISHMAN_TILE, TCODColor::lightGreen,
		"Thickly scaled and sunken-eyed. It stalks the ruins as though searching desperately for something."));
	m->setHealth(50);
	m->setDefence(DAMAGE_PHYSICAL, 20);
	m->setMeleeStats(50, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->addSpellKnown(spell_AcidSpit());
	m->setSpellPower(75);
	m->setSpellCastChance(15);
	m->setBleedResist(60);
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_FishmanDoomPreacher()
{
	monsterSharedPtr m(new monster("Fishman Doom-Preacher", FISHMAN_TILE, TCODColor::lime,
		"Wields a staff with a toad impaled on the tip."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setBleedResist(60);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->addSpellKnown(ability_FeedingFrenzy());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_FishmanSpearfisher()
{
	monsterSharedPtr m(new monster("Fishman Spearfisher", FISHMAN_TILE, TCODColor::lightLime,
		"It wields a barbed harpoon, and its lamplike eyes search for prey."));
	m->setHealth(75);
	m->setDefence(DAMAGE_PHYSICAL, 20);
	m->setBleedResist(60);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weapon_FishmansHarpoon());
	m->addSpellKnown(attack_Spearfishing());
	m->setSpellCastChance(40);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_GuardianGolem()
{
	monsterSharedPtr m(new monster("Guardian Golem", GOREM_TILE, TCODColor::lightLime,
		"A massive stone creature stalks the flooded streets."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(100, SPEED_SLOW);
	m->setFragmentsDropped(200);
	m->addItemDrop(key_RuinedTownshipKey());
	return m;
}

monsterSharedPtr monster_DegenerateFishman()
{
	monsterSharedPtr m(new monster("Degenerate Fishman", FISHMAN_TILE, TCODColor::lightestLime,
		"The fishmen were never meant to be, but this one is truly mad."));
	m->setHealth(25);
	m->setBleedResist(75);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_FAST);
	m->addSpellKnown(attack_Quickstep());
	m->setSpellCastChance(15);
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_SkinlessKnight()
{
	monsterSharedPtr m(new monster("Skinless Knight", KNIGHT_TILE, TCODColor::lightPink,
		"The armour stained and dented, and beneath the slitted helm, two bloated eyes sit in a skinless skull."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_MAGIC, 25);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(50, SPEED_NORMAL);
	m->addSpellKnown(prayer_ProfaneRadiance());
	m->setSpellCastChance(15);
	m->makeProfane();
	m->isBoss = true;
	m->showBossHealthBar = false;
	return m;
}

monsterSharedPtr monster_FishbornGoddess()
{
	monsterSharedPtr m(new monster("Fishborn Goddess", FISHMAN_TILE, TCODColor::lighterGreen,
		"A woman with trailing seaweed hair, whose unnaturally long arms drip acid."));
	m->setHealth(700);
	m->setBleedResist(50);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_ACID, 10);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(30, SPEED_SLOW);
	m->addSpellKnown(prayer_SinkBeneath());
	m->addSpellKnown(spell_AcidSpit());
	m->addSpellKnown(spell_AcidBurst());
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->setFragmentsDropped(600);
	m->addItemDrop(key_WyrdKey());
	m->addItemDrop(heart_FishBornGoddessesHeart());
	return m;
}


/*
	Darkwater Forest
*/

monsterSharedPtr monster_GiantRat()
{
	monsterSharedPtr m(new monster("Giant Rat", RAT_TILE, TCODColor::lightSepia,
		"A dog-sized rat with venom dripping from its teeth."));
	m->setHealth(25);
	m->setPoisonResist(1000);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_FAST, EFFECT_POISON, 5)));
	m->setFragmentsDropped(5);
	return m;
}

monsterSharedPtr monster_ProfaneRatPriest()
{
	monsterSharedPtr m(new monster("Profane Rat Priest", RAT_TILE, TCODColor::lightestSepia,
		"You can see a rat's snout protruding from beneath the cowl of this hooded figure."));
	m->setHealth(100);
	m->setPoisonResist(1000);
	m->addDamageResist(DAMAGE_MAGIC, 50);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(10, SPEED_SLOW);
	m->addSpawnableCreature("giant_rat");
	m->setSpawnChance(20);
	m->keepsDistance = true;
	m->makeProfane();
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_RatPrince()
{
	monsterSharedPtr m(new monster("Rat Prince Pollug", RAT_TILE, TCODColor::green,
		"This rat, shaped like a man, swings a holy bell and cackles incoherently."));
	m->setHealth(400);
	m->setPoisonResist(1000);
	m->setMoveStats(SPEED_FAST);
	m->keepsDistance = true;
	m->makeProfane();
	m->addSpellKnown(spellSharedPtr(new spell("Envenom", TCODColor::green, 3, 0, APPLY_POISON_DAMAGE, 10)));
	m->setSpellCastChance(25);
	m->setFragmentsDropped(200);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->addItemDrop(chime_GreenKnightsChime());
	return m;
}


/*
	Crumbling City
*/


monsterSharedPtr monster_CrawlingZombie()
{
	monsterSharedPtr m(new monster("Crawling Zombie", ZOMBIE_TILE, TCODColor::darkGreen,
		"A half-rotten creature, missing both of its legs, which crawls along the ground."));
	m->setHealth(150);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(50, SPEED_SLOW);
	m->setPoisonResist(1000);
	m->setFragmentsDropped(15);
	return m;
}

monsterSharedPtr monster_FirespitterZombie()
{
	monsterSharedPtr m(new monster("Firespitter Zombie", ZOMBIE_TILE, TCODColor::flame,
		"A half-rotten creature imbued with a mysterious inner fire."));
	m->setHealth(150);
	m->setPoisonResist(1000);
	m->addDamageResist(DAMAGE_FIRE, 25);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(50, SPEED_SLOW);
	m->setFragmentsDropped(75);
	m->addSpellKnown(spell_SpitFire());
	m->setSpellCastChance(20);
	return m;
}

monsterSharedPtr monster_OozingZombie()
{
	monsterSharedPtr m(new monster("Oozing Zombie", ZOMBIE_TILE, TCODColor::sepia,
		"A half-rotten creature oozing slime from its pores."));
	m->setHealth(150);
	m->setPoisonResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(50, SPEED_SLOW);
	m->setFragmentsDropped(75);
	m->addSpellKnown(ability_DropOoze());
	m->setSpellCastChance(50);
	return m;
}

monsterSharedPtr monster_DevoteeOfPash()
{
	monsterSharedPtr m(new monster("Devotee of Pash", PRIEST_TILE, TCODColor::purple,
		"A tall figure in robes marked with profane symbols. It clutches an idol of Pash, wreathed in evil energy."));
	m->setHealth(200);
	m->addDamageResist(DAMAGE_MAGIC, 25);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_SLOW);
	m->addSpellKnown(prayer_ProfaneRay());
	m->setSpellCastChance(25);
	m->keepsDistance = true;
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_VoidwalkerMinas()
{
	monsterSharedPtr m(new monster("Voidwalker Minas", PLAYER_TILE, TCODColor::lightPurple,
		"His armour glows with profane energy, and his jet-black sword pulses with dark radiance."));
	m->setHealth(250);
	m->setMoveStats(SPEED_SLOW);
	m->setDefence(DAMAGE_PHYSICAL, 30);
	m->addDamageResist(DAMAGE_COLD, 50);
	m->makeProfane();
	m->equipItem(weapon_ProfaneGreatsword());
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(25);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->setFragmentsDropped(300);
	m->addItemDrop(bell_VoidwalkersDancingBell());
	return m;
}

monsterSharedPtr monster_BloodthirstyHound()
{
	monsterSharedPtr m(new monster("Bloodthirsty Hound", HOUND_TILE, TCODColor::crimson,
		"A skeletal hound whose serrated teeth drip blood."));
	m->setHealth(50);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_FAST, EFFECT_BLEED, 15)));
	m->setFragmentsDropped(20);
	return m;
}

monsterSharedPtr monster_BloodstarvedZombie()
{
	monsterSharedPtr m(new monster("Bloodstarved Zombie", ZOMBIE_TILE, TCODColor::crimson,
		"This rotted creature's distended jaw drips with fresh blood."));
	m->setHealth(150);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_BLEED, 10)));
	m->bleedScaling = 25;
	m->setBleedResist(60);
	m->addSpellKnown(attack_BloodFeast());
	m->setSpellCastChance(25);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_HighClericOrsylTheProfaned()
{
	monsterSharedPtr m(new monster("High Cleric Orsyl the Profaned", ORSYL_TILE, TCODColor::silver,
		"The High Clerics robes are tattered and blood-drenched; his eyes are blank, and putrid tendrils of the Void crawl up \
his flesh."));
	m->setHealth(800);
	m->bleedScaling = 50;
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_BLESSED, 25);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_NORMAL, EFFECT_BLEED, 15)));
	m->addSpellKnown(prayer_ProfaneRadiance());
	m->addSpellKnown(attack_BloodFeast());
	m->addSpellKnown(prayer_DrawOutTheBlood());
	m->setSpellCastChance(90);
	m->setDivinePower(120);
	m->isBoss = true;
	m->setFragmentsDropped(1000);
	m->addItemDrop(key_SpinalColumnShard());
	m->addItemDrop(heart_OrsylsShriveledHeart());
	return m;
}

monsterSharedPtr monster_CursedKnight()
{
	monsterSharedPtr m(new monster("Cursed Knight", KNIGHT_TILE, TCODColor::purple,
		"The holy symbols on its armour are drenched in the Void."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 20);
	m->setDefence(DAMAGE_FIRE, 20);
	m->setDefence(DAMAGE_COLD, 20);
	m->makeProfane();
	m->setMeleeStats(50, SPEED_NORMAL);
	m->setMoveStats(SPEED_SLOW);
	m->addSpellKnown(attack_Quickstep());
	m->setSpellCastChance(25);
	m->setFragmentsDropped(150);
	return m;
}

monsterSharedPtr monster_CursedArcher()
{
	monsterSharedPtr m(new monster("Cursed Archer", KNIGHT_TILE, TCODColor::lightPurple,
		"Archers armoured in the garb of the knights of the Winter Court."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 20);
	m->setDefence(DAMAGE_FIRE, 20);
	m->setDefence(DAMAGE_COLD, 20);
	m->makeProfane();
	m->setMeleeStats(15, SPEED_NORMAL);
	m->setMoveStats(SPEED_SLOW);
	m->keepsDistance = true;
	m->addSpellKnown(spellSharedPtr(new spell("Profane Arrow", TCODColor::lightPurple, 8, 10, APPLY_PROFANE_DAMAGE, 15)));
	m->setSpellCastChance(50);
	m->setFragmentsDropped(150);
	return m;
}

monsterSharedPtr monster_Enkidu()
{
	monsterSharedPtr m(new monster("Enkidu, Seeker of the Deathless", KNIGHT_TILE, TCODColor::lightFuchsia,
		"His armour glows with the symbols of the Void. From beneath his mask, a single eye glows with furious \
determination."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 60);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setDefence(DAMAGE_ELECTRIC, 25);
	m->makeProfane();
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weapon_VoidCrystalGreatsword());
	m->setFragmentsDropped(500);
	m->addItemDrop(weapon_VoidCrystalGreatsword());
	m->addSpellKnown(prayer_ProfaneRay());
	m->addSpellKnown(attack_VoidSurge());
	m->setSpellCastChance(20);
	m->isBoss = true;
	m->showBossHealthBar = false;
	return m;
}

monsterSharedPtr monster_DukeVortenTheWatchful()
{
	monsterSharedPtr m(new monster("Duke Vorten the Watchful", DUKE_TILE, TCODColor::lightFuchsia,
		"A bloated figure rises from the floor. His flesh is a mass of eyestalks and protruding shards of bone; a swarm of \
tentacles emerge from the ruin of his head, and at the centre of his chest sits a gaping maw."));
	m->setHealth(800);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_ACID, 25);
	m->setDefence(DAMAGE_MAGIC, 25);
	m->setMeleeStats(50, SPEED_NORMAL);
	m->setMoveStats(SPEED_NORMAL);
	m->makeProfane();
	m->addSpellKnown(ability_Throw());
	m->addSpellKnown(ability_VoidStep());
	m->addSpellKnown(ability_TentacleSwarm());
	m->setSpellCastChance(60);
	m->isBoss = true;
	m->setFragmentsDropped(1200);
	m->addItemDrop(heart_VortensShriveledHeart());
	m->addItemDrop(key_WatchfulEyestalk());
	return m;
}


/*
	The Winter Court
*/

monsterSharedPtr monster_WinterShade()
{
	monsterSharedPtr m(new monster("Winter Shade", GHOST_TILE, TCODColor::cyan,
		"The spectre of a noble, trailing frost from its tattered robes."));
	m->setHealth(50);
	m->setDefence(DAMAGE_PHYSICAL, 90);
	m->setDefence(DAMAGE_COLD, 100);
	m->addWeakness(DAMAGE_FIRE);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_NORMAL, DAMAGE_COLD, 50)));
	m->addSpellKnown(ability_WinterBlast());
	m->setSpellCastChance(20);
	m->setFragmentsDropped(75);
	return m;
}

monsterSharedPtr monster_HollowfacedKnight()
{
	monsterSharedPtr m(new monster("Hollowfaced Knight", KNIGHT_TILE, TCODColor::lightCyan,
		"This knight is in shambles, its frozen mace drooping limply from one hand. Only the sight of an intruder seems to \
arouse its rage; its eyes glow red for an instant."));
	m->setHealth(150);
	m->setDefence(DAMAGE_PHYSICAL, 40);
	m->setDefence(DAMAGE_COLD, 50);
	m->addWeakness(DAMAGE_FIRE);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_SLOW, DAMAGE_COLD, 25)));
	m->addSpellKnown(spellSharedPtr(new spell("Punch", TCODColor::lightCyan, 1, 25, KNOCKBACK_TARGET, 3)));
	m->setSpellCastChance(20);
	m->setFragmentsDropped(100);
	return m;
}




/*
	The Murdermire
*/



monsterSharedPtr monster_Wisp()
{
	monsterSharedPtr m(new monster("Wisp", WISP_TILE, TCODColor::flame,
		"A point of drifting light."));
	m->setHealth(25);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_FIRE, 90);
	m->setBleedResist(1000);
	m->setPoisonResist(1000);
	m->addWeakness(DAMAGE_ELECTRIC);
	m->addWeakness(DAMAGE_COLD);
	m->setMeleeStats(5, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->keepsDistance = true;
	m->addSpellKnown(ability_WreatheInFlame());
	m->setSpellCastChance(20);
	m->setFragmentsDropped(15);
	m->setLightEmitted(0.5);
	return m;
}

monsterSharedPtr monster_DrownedDead()
{
	monsterSharedPtr m(new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::sea,
		"A rotten and barnacle-encrusted figure."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 20);
	m->setPoisonResist(1000);
	m->addWeakness(DAMAGE_FIRE);
	m->addWeakness(DAMAGE_BLESSED);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_SLOW, EFFECT_POISON, 10)));
	m->addSpellKnown(ability_DragBelow());
	m->setSpellCastChance(25);
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_SwampSpider()
{
	monsterSharedPtr m(new monster("Swamp Spider", SPIDER_TILE, TCODColor::sea,
		"A bloated body the size of a dog sits upon eight spindly legs. A mass of pale green eyes regard you hungrily."));
	m->setHealth(100);
	m->setPoisonResist(1000);
	m->addWeakness(DAMAGE_FIRE);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_NORMAL, EFFECT_POISON, 25)));
	m->addSpellKnown(ability_SpitWeb());
	m->setSpellCastChance(25);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_AdherentOfGlug()
{
	monsterSharedPtr m(new monster("Adherent of Glurg", TOAD_TILE, TCODColor::sea,
		"Under the mud-soaked hood, a toad's bulging eyes leer at you. The \
creature rings a filth-stained chime and chants in an unknown tongue."));
	m->setHealth(100);
	m->setPoisonResist(1000);
	m->setBleedResist(60);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_SLOW);
	m->addSpellKnown(spellSharedPtr(new spell("Poison Spit", TCODColor::lime, 5, 0, APPLY_POISON_DAMAGE, 25)));
	m->setSpellCastChance(50);
	m->setFragmentsDropped(40);
	return m;
}

monsterSharedPtr monster_GhorthTheBloatedSpider()
{
	monsterSharedPtr m(new monster("Ghorth the Bloated Spider", SPIDER_TILE, TCODColor::purple,
		"This monstrous creature has a body the size of a horse, on which rest a dozen egg-sized green eyes. \
Venom drips from its fangs, and it drags itself along the ground with eight spindly arms."));
	m->setHealth(800);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_SLOW, EFFECT_POISON, 25)));
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setPoisonResist(1000);
	m->addWeakness(DAMAGE_FIRE);
	m->addSpawnableCreature("swamp_spider");
	m->setSpawnChance(40);
	m->immuneToEntangle = true;
	m->keepsDistance = true;
	m->isBoss = true;
	m->canSwapWithAllies = true;
	m->setFragmentsDropped(800);
	m->addItemDrop(heart_VenomousSpiderHeart());
	m->addItemDrop(key_OldValleyKey());
	return m;
}



/*
	Tear-Stained Valley
*/



monsterSharedPtr monster_UndeadPrisoner()
{
	monsterSharedPtr m(new monster("Undead Prisoner", UNDEAD_TILE, TCODColor::lightGreen,
		"Undying prisoner, restrained by heavy iron chains."));
	m->setHealth(150);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_COLD, 50);
	m->setMeleeStats(50, SPEED_SLOW);
	m->setMoveStats(SPEED_SLOW);
	m->makeProfane();
	m->addSpellKnown(spellSharedPtr(new spell("Chain Whip", TCODColor::grey, 3, 25)));
	m->setSpellCastChance(50);
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_BloatedSlaveDriver()
{
	monsterSharedPtr m(new monster("Bloated Slave Driver", JAILOR_TILE, TCODColor::lightGrey,
		"A driver of slaves, fat with overindulgence, who leers and wields a wicked whip."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setMoveStats(SPEED_NORMAL);
	m->addSpellKnown(spellSharedPtr(new spell("Lash", TCODColor::red, 3, 5, APPLY_BLEED_DAMAGE, 15)));
	m->addSpellKnown(ability_FeedingFrenzy());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_SlaveringHound()
{
	monsterSharedPtr m(new monster("Slavering Hound", HOUND_TILE, TCODColor::red,
		"A starving dog, its jaw stained with blood, its eyes betraying a ravenous hunger."));
	m->setHealth(50);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(15, SPEED_FAST, EFFECT_BLEED, 10)));
	m->setFragmentsDropped(25);
	return m;
}

monsterSharedPtr monster_Emissary()
{
	monsterSharedPtr m(new monster("Emissary", EMISSARY_TILE, TCODColor::flame,
		"A gaunt figure in a silver robe. He holds up a charred holy symbol and utters words of blessed fire."));
	m->setHealth(150);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->addSpellKnown(spellSharedPtr(new spell("Holy Fire", TCODColor::flame, 5, 0, APPLY_FIRE_DAMAGE, 25)));
	m->addSpellKnown(prayer_DivineRestoration());
	m->setSpellCastChance(75);
	m->keepsDistance = true;
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_BartonTheProwlingMinister()
{
	monsterSharedPtr m(new monster("Barton the Prowling Minister", EMISSARY_TILE, TCODColor::purple,
		"The grinning minister sways above his supplicant congregation, clutching a holy symbol and whispering \
prayers through emaciated lips."));
	m->setHealth(800);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setDefence(DAMAGE_ELECTRIC, 25);
	m->setMeleeStats(10, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->addSpellKnown(prayer_DivineRetribution());
	m->addSpellKnown(prayer_DivineJudgement());
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->isBoss = true;
	m->setFragmentsDropped(1000);
	m->addItemDrop(key_BartonsKey());
	return m;
}

monsterSharedPtr monster_SlaveMasterGasgut()
{
	monsterSharedPtr m(new monster("Slavemaster Gasgut", JAILOR_TILE, TCODColor::red,
		"This bloated lord of the slave-drivers wields a crooked whip and bellows with impotent rage. \
Alone, there is little he can do."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setBleedResist(80);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_FAST, EFFECT_BLEED, 15)));
	m->addSpellKnown(attack_Lash());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(500);
	m->addItemDrop(key_HightowerKey());
	m->addItemDrop(weapon_SlaveMastersWhip());
	m->isBoss = true;
	m->showBossHealthBar = false;
	return m;
}

monsterSharedPtr monster_WhiteFogHerald()
{
	monsterSharedPtr m(new monster("White Fog Herald", HERALD_TILE, TCODColor::darkGrey,
		"A figure is visible in the fog: no less than ten feet tall, with stretched skin of pure white and eyes that \
glow crystal-blue. Torn silver robes hang from its gaunt body, and it wields a horn and a slender staff."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_COLD, 100);
	m->addWeakness(DAMAGE_FIRE);
	m->setBleedResist(100);
	m->setPoisonResist(100);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(15, SPEED_SLOW, DAMAGE_COLD, 35)));
	m->emitsFog = 3;
	m->addSpellKnown(spellSharedPtr(new spell("Fellfrost", TCODColor::cyan, 5, 0, APPLY_COLD_DAMAGE, 25)));
	m->addSpellKnown(spell_VoidJaunt());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(2000);
	m->isBoss = true;
	m->addItemDrop(horn_HeraldsWhiteHorn());
	return m;
}

monsterSharedPtr monster_HightowerAristocrat()
{
	monsterSharedPtr m(new monster("Hightower Aristocrat", ARISTOCRAT_TILE, TCODColor::lightBlue,
		"A grinning aristocrat of Hightower, already drunk on wine and black honey. Its face is a death-mask."));
	m->setHealth(50);
	m->addWeakness(DAMAGE_FIRE);
	m->addWeakness(DAMAGE_ELECTRIC);
	m->addWeakness(DAMAGE_ACID);
	m->addWeakness(DAMAGE_COLD);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_BLEED, 10)));
	m->setFragmentsDropped(10);
	return m;
}

monsterSharedPtr monster_SilverKnight()
{
	monsterSharedPtr m(new monster("Silver Knight", KNIGHT_TILE, TCODColor::silver,
		"A knight in silver armour, set to stand guard over the guests. Their silence, \
and their heavy hoods, led some guests to suspect that the suits of armour were empty."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_PROFANE, 75);
	m->setDefence(DAMAGE_COLD, 25);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_SLOW, DAMAGE_ELECTRIC, 25)));
	m->addSpellKnown(prayer_BlessedRadiance());
	m->setSpellCastChance(20);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_LadyTvert()
{
	monsterSharedPtr m(new monster("Lady Tvert", ARISTOCRAT_TILE, TCODColor::lightestBlue,
		"Her gown and jewels are the portrait of elegance, but something about her aspect is unsettling. Perhaps her unnatural \
height, or her extraordinarily long and thin fingers, which end in hooked nails; or perhaps \
it's the sight of her sharpened teeth as she turns to grin at you, or the blankness of her pure red eyes."));
	m->setHealth(800);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_MAGIC, 50);
	m->makeProfane();
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_POISON, 20)));
	m->addSpellKnown(attack_RavenousHunger());
	m->addSpellKnown(spell_VoidJaunt());
	m->setSpellCastChance(20);
	m->keepsDistance = true;
	m->addSpawnableCreature("scion_of_tvert");
	m->setSpawnHealthThreshold(400);
	return m;
}

monsterSharedPtr monster_ScionOfTvert()
{
	monsterSharedPtr m(new monster("Scion of Tvert", ARISTOCRAT_TILE, TCODColor::lightPink,
		"Lady Tvert's bile forms the shape of this twisted creature - a mockery of the human form, \
his face riddled with scars, his arms gruesome chunks of flesh covered in vile worms."));
	m->setHealth(1200);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setBleedResist(80);
	m->setPoisonResist(80);
	m->makeProfane();
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_FAST, DAMAGE_PROFANE, 30)));
	m->addSpellKnown(ability_ShredSkin());
	m->setSpellCastChance(20);
	m->addItemDrop(key_LadyTvertsKey());
	m->addItemDrop(heart_LadyTvertsHeart());
	m->setFragmentsDropped(1500);
	m->isBoss = true;
	return m;
}

monsterSharedPtr monster_MouthOfPash()
{
	monsterSharedPtr m(new monster("Mouth of Pash", SPIDER_TILE, TCODColor::lightestRed,
		"This thing looks like nothing more than an enormous mouth, crawling on hundreds of tiny insect's legs. \
Its thousands of teeth drip poison, and a mass of eyestalks protrude from its head."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->makeProfane();
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(15, SPEED_NORMAL, EFFECT_POISON, 15)));
	m->addSpellKnown(ability_Gnash());
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->setFragmentsDropped(2000);
	m->addItemDrop(key_MawtoothFragment());
	return m;
}



/*
	Dead Sparrow's Tower
*/



monsterSharedPtr monster_SparrowKnight()
{
	monsterSharedPtr m(new monster("Sparrow Knight", KNIGHT_TILE, TCODColor::magenta,
		"A knight in half-visible armour, wielding an ethereal blade. Two pinprick-light eyes glare from under \
the glistening helm."));
	m->setHealth(150);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_MAGIC, 80);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_NORMAL, DAMAGE_MAGIC, 50)));
	m->addSpellKnown(spellSharedPtr(new spell("Ethereal Jaunt", TCODColor::magenta, ATTACK_BUFF_SELF,
		0, TELEPORT, 3)));
	m->setSpellCastChance(10);
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_EtherealMage()
{
	monsterSharedPtr m(new monster("Ethereal Mage", MAGE_TILE, TCODColor::magenta,
		"At first this creature is hardly visible. It appears only as a floating white robe clutching \
a wand, while two pale eyes regard you with something like disinterest."));
	m->setHealth(50);
	m->setDefence(DAMAGE_MAGIC, 90);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->addSpellKnown(spell_LightningStrike());
	m->addSpellKnown(spellSharedPtr(new spell("Ethereal Curse", TCODColor::magenta, ATTACK_RANGE, 4,
		TELEPORT, 3)));
	m->setSpellCastChance(75);
	m->keepsDistance = true;
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_DecomposingKnight()
{
	monsterSharedPtr m(new monster("Decomposing Knight", KNIGHT_TILE, TCODColor::green,
		"This rotten knight slouches towards you, flesh disintegrating, dragging a filth-crusted sword."));
	m->setHealth(200);
	m->setMoveStats(SPEED_SLOW);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ACID, 25);
	m->setPoisonResist(60);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_NORMAL, EFFECT_POISON, 20)));
	m->addSpellKnown(spellSharedPtr(new spell("Flesh Chunk", TCODColor::darkLime, 5, 0, APPLY_POISON_DAMAGE, 10)));
	m->setSpellCastChance(20);
	m->setFragmentsDropped(250);
	return m;
}

monsterSharedPtr monster_LifedrinkerWraith()
{
	monsterSharedPtr m(new monster("Lifedrinker Wraith", GHOST_TILE, TCODColor::magenta,
		"The half-shape of a creature starved for living flesh, which drifts through the aether."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_COLD, 75);
	m->setPoisonResist(1000);
	m->setBleedResist(1000);
	m->addWeakness(DAMAGE_MAGIC);
	m->addWeakness(DAMAGE_ELECTRIC);
	m->setMoveStats(SPEED_FAST);
	m->setFragmentsDropped(200);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_FAST, DAMAGE_MAGIC, 25)));
	return m;
}

monsterSharedPtr monster_TheDeadSparrow()
{
	monsterSharedPtr m(new monster("The Dead Sparrow", GHOST_TILE, TCODColor::lightestFuchsia,
		"The First Sparrow has died a hundred times, but his body endures, shrouded in arcane mist."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_MAGIC, 90);
	m->setMoveStats(SPEED_FAST);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->keepsDistance = true;
	m->setSpellPower(150);
	m->addSpellKnown(spell_MagicMissile());
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->addSpawnableCreature("mirror_image");
	m->setSpawnChance(75);
	m->setFragmentsDropped(3000);
	m->addItemDrop(key_MoshkasKey());
	m->addItemDrop(heart_DeadSparrowsHeart());
	return m;
}

monsterSharedPtr monster_MirrorImage()
{
	monsterSharedPtr m(new monster("Mirror Image", GHOST_TILE, TCODColor::desaturatedFuchsia,
		"A faded image of the Dead Sparrow, half-lost in the aether."));
	m->setHealth(1);
	m->setDefence(DAMAGE_MAGIC, 90);
	m->setMoveStats(SPEED_FAST);
	m->setSpellPower(75);
	m->addSpellKnown(spell_MagicMissile());
	m->addSpellKnown(spell_ArcLightning());
	m->setSpellCastChance(80);
	return m;
}


/*
	Moshka's Observatory
*/

monsterSharedPtr monster_Starspawned()
{
	monsterSharedPtr m(new monster("Starspawned", STARSPAWN_TILE, TCODColor::lighterPurple,
		"This shrieking mass of eyes floats on tattered wings."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setDefence(DAMAGE_MAGIC, 25);
	m->addWeakness(DAMAGE_ELECTRIC);
	m->addSpellKnown(ability_DevouringGaze());
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(15, SPEED_FAST, EFFECT_POISON, 15)));
	m->setSpellCastChance(20);
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_Ooze()
{
	monsterSharedPtr m(new monster("Ooze", OOZE_TILE, TCODColor::lightSepia,
		"This pathetic ooze dribbles along the ground. A few bones and rags protrude from its midst."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setPoisonResist(1000);
	m->setBleedResist(1000);
	m->addWeakness(DAMAGE_FIRE);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_NORMAL, EFFECT_POISON, 15)));
	m->addSpellKnown(spellSharedPtr(new spell("Spit", TCODColor::lime, 5, 0, APPLY_POISON_DAMAGE, 10)));
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->setFragmentsDropped(5000);
	m->addItemDrop(wand_MoshkasSingingStaff());
	return m;
}

/*
	Mausoleum of Blood
*/

monsterSharedPtr monster_BleedingMaw()
{
	monsterSharedPtr m(new monster("Bleeding Maw", TOAD_TILE, TCODColor::crimson,
		"A wreckage of flesh, encrusted with gibbering mouths and drenched in blood. Its own, or another's?"));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_SLOW, EFFECT_BLEED, 20)));
	m->addSpellKnown(spellSharedPtr(new spell("Engulf", TCODColor::crimson, 1, 10, APPLY_ENTANGLING, 3)));
	m->setSpellCastChance(100);
	return m;
}

monsterSharedPtr monster_CrawlingWretch()
{
	monsterSharedPtr m(new monster("Crawling Wretch", WRETCH_TILE, TCODColor::crimson,
		"A wretched ruin, perhaps once human, that crawls around the ground. As it grows near, \
its mouth suddenly distends, exposing myriad jagged teeth."));
	m->setHealth(150);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_SLOW, EFFECT_BLEED, 20)));
	m->addSpellKnown(spellSharedPtr(new spell("Rush", TCODColor::orange, ATTACK_BUFF_SELF, 1, GAIN_FREE_MOVES, 3)));
	m->setSpellCastChance(50);
	return m;
}

monsterSharedPtr monster_CorenTheBloody()
{
	monsterSharedPtr m(new monster("Coren the Bloody", PLAYER_TILE, TCODColor::crimson,
		"Wounds cover the naked warrior's body, gushing blood as though from a hundred gaping mouths. \
He hefts his greataxe and roars incoherently."));
	m->setHealth(900);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(50, SPEED_SLOW, EFFECT_BLEED, 30)));
	m->addSpellKnown(spellSharedPtr(new spell("Frenzy", TCODColor::darkCrimson, ATTACK_BUFF_SELF, 1, SCALE_NEXT_ATTACK, 50)));
	m->setSpellCastChance(30);
	m->setFragmentsDropped(1500);
	m->addItemDrop(heart_HeartOfCoren());
	m->isBoss = true;
	return m;
}

/*
	Mausoleum of the Lost
*/

monsterSharedPtr monster_SkeletonKnight()
{
	monsterSharedPtr m(new monster("Skeleton Knight", SKELETON_TILE, TCODColor::lightestGrey,
		"A clattering pile of bones, wielding the shattered hilt of a sword."));
	m->setHealth(400);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_COLD, 50);
	m->setDefence(DAMAGE_FIRE, 50);
	m->setBleedResist(1000);
	m->setPoisonResist(1000);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, DAMAGE_ELECTRIC, 25)));
	return m;
}

monsterSharedPtr monster_SkeletonArcher()
{
	monsterSharedPtr m(new monster("Skeleton Archer", SKELETON_TILE, TCODColor::lighterGrey,
		"A clattering pile of bones holding a battered crossbow."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_COLD, 50);
	m->setDefence(DAMAGE_FIRE, 50);
	m->setBleedResist(1000);
	m->setPoisonResist(1000);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_BLEED, 10)));
	m->keepsDistance = true;
	m->addSpellKnown(spellSharedPtr(new spell("Bone Arrow", TCODColor::white, 8, 10, APPLY_ELECTRIC_DAMAGE, 15)));
	m->setSpellCastChance(75);
	return m;
}

monsterSharedPtr monster_KhalleTheLost()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Khalle the Lost", PLAYER_TILE, TCODColor::darkYellow,
		"Her skull is visible through her thin flesh; bones jut from her arms. She clutches her holy chime and her \
crooked scythe, and whispers unanswered prayers."));
	m->setHealth(800);
	m->setDefence(DAMAGE_ELECTRIC, 50);
	m->setDefence(DAMAGE_FIRE, 50);
	m->setDefence(DAMAGE_BLESSED, 100);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_NORMAL, DAMAGE_ELECTRIC, 25)));
	m->addSpellKnown(prayer_TouchOfDoom());
	m->addSpellKnown(attack_HolySummons());
	m->setDivinePower(120);
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->setFragmentsDropped(1500);
	m->addItemDrop(heart_KhallesWretchedHeart());
	return m;
}

monsterSharedPtr monster_VoidwalkerMarduk()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Voidwalker Marduk", PLAYER_TILE, TCODColor::lightPurple,
		"This voidwalker wears armour spiked with void crystals, and wields a broken deadwood wand."));
	m->setHealth(400);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_PROFANE, 50);
	m->setDefence(DAMAGE_BLESSED, 50);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(25, SPEED_NORMAL);
	m->addSpellKnown(spell_StormLash());
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(500);
	m->addItemDrop(spell_StormLash());
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_VoidwalkerIcarus()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Voidwalker Icarus", PLAYER_TILE, TCODColor::purple,
		"This voidwalker wears a crown that glows with the light of the void, and wields a\
 pair of knives imbued with dark flame."));
	m->setHealth(400);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 50);
	m->setDefence(DAMAGE_FIRE, 50);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weapon_VoidfireKnife());
	m->setFragmentsDropped(500);
	m->addItemDrop(weapon_VoidfireKnife());
	m->addSpellKnown(attack_KnifeCast());
	m->setSpellCastChance(30);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_VoidwalkerPenelope()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Voidwalker Penelope", PLAYER_TILE, TCODColor::darkPurple,
		"This voidwalker wears a tattered purple cape, and wields an enormous hammer that crackles with void lightning."));
	m->setHealth(600);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 50);
	m->setDefence(DAMAGE_ACID, 50);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weapon_LightningWarhammer());
	m->addSpellKnown(attack_LightningSlam());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(500);
	m->addItemDrop(weapon_LightningWarhammer());
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_IetraEaterOfSilence()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Ietra, Eater of Silence", PLAYER_TILE, TCODColor::lighterPurple,
		"Her eyes are deep pools that reflect the dark of the void; her hair crackles with lightning and fire. In one hand \
she wields a crooked scythe imbued with flames; in the other, she clutches an amber pendant."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 30);
	m->setDefence(DAMAGE_FIRE, 30);
	m->setDefence(DAMAGE_PROFANE, 30);
	m->setMoveStats(SPEED_NORMAL);
	m->isBoss = true;
	m->setFragmentsDropped(3000);
	m->addItemDrop(weapon_IetrasFlameScythe());
	m->addSpellKnown(spell_Firestorm());
	m->setSpellCastChance(20);
	m->addItemDrop(heart_IetrasBurningHeart());
	m->addItemDrop(key_IetrasAmberPendant());
	m->makeImmuneToVoid();
	return m;
}




/*
	Bridge of Lords
*/



monsterSharedPtr monster_SentinelPikeman()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Sentinel Pikeman", KNIGHT_TILE, TCODColor::lightBlue,
		"A tall figure in monolithic armour, wielding a wicked-looking pike."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 75);
	m->setDefence(DAMAGE_MAGIC, 75);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_NORMAL, DAMAGE_ELECTRIC, 25)));
	m->addSpellKnown(attack_Penetrate());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(250);
	return m;
}

monsterSharedPtr monster_SentinelArcher()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Sentinel Archer", KNIGHT_TILE, TCODColor::lightestBlue,
		"A tall knight in monolithic armour, wielding an enormous bow."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 75);
	m->setDefence(DAMAGE_MAGIC, 75);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(10, SPEED_SLOW);
	m->addSpellKnown(spellSharedPtr(new spell("Greatbow Shot", TCODColor::lightBlue, 8, 25, APPLY_ELECTRIC_DAMAGE, 25)));
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(250);
	return m;
}

monsterSharedPtr monster_GhostlyWarrior()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Ghostly Warrior", KNIGHT_TILE, TCODColor::lightPink,
		"The pale shadow of a knight, dressed in ghostly armour, who drifts slightly above the ground. In one \
hand it clutches a huge, ethereal greatsword."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->addWeakness(DAMAGE_MAGIC);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_SLOW, DAMAGE_MAGIC, 50)));
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_EtherealWisp()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Ethereal Wisp", WISP_TILE, TCODColor::lightPink,
		"This thing is hardly visible - a faint cloud that drifts above the ground, exuding a pale mist."));
	m->setHealth(25);
	m->setLightEmitted(0.4);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_ACID, 100);
	m->addWeakness(DAMAGE_MAGIC);
	m->setMoveStats(SPEED_NORMAL);
	m->keepsDistance = true;
	m->addSpellKnown(spellSharedPtr(new spell("Acidic Cloud", TCODColor::green, ATTACK_AOE, 3, APPLY_ACID_DAMAGE, 25)));
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_WeepingWanderer()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Weeping Wanderer", WISP_TILE, TCODColor::darkPink,
		"One could almost mistake this for a human shape, were it not for the ethereal glow exuding from its torn \
white robes. Its tears stain the floor of the valley."));
	m->setHealth(500);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_ACID, 100);
	m->addWeakness(DAMAGE_MAGIC);
	m->setMoveStats(SPEED_NORMAL);
	m->keepsDistance = true;
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->setFragmentsDropped(500);
	m->addSpellKnown(spellSharedPtr(new spell("Weep", TCODColor::green, ATTACK_BUFF_SELF, 1, DROP_DRAINING_POOL, 1)));
	m->setSpellCastChance(25);
	return m;
}

monsterSharedPtr monster_ThePaleShade()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("The Pale Shade", GHOST_TILE, TCODColor::lightCyan,
		"This near-invisible creature floats in a pale cloud, and its frost-drenched eyes suggest a sense of profound calm. \
Acid drips from one twisted hand, while the other sings with electricity."));
	m->setHealth(1200);
	m->setLightEmitted(0.4);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_COLD, 75);
	m->setDefence(DAMAGE_ELECTRIC, 75);
	m->setDefence(DAMAGE_COLD, 75);
	m->addWeakness(DAMAGE_FIRE);
	m->addWeakness(DAMAGE_MAGIC);
	m->setPoisonResist(1000);
	m->setBleedResist(100);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_NORMAL, DAMAGE_MAGIC, 50)));
	m->addSpellKnown(spellSharedPtr(new spell("Electric Touch", TCODColor::lightPurple, 1, 0, APPLY_ELECTRIC_DAMAGE, 25)));
	m->addSpellKnown(spellSharedPtr(new spell("Frigid Glance", TCODColor::cyan, 5, 0, APPLY_COLD_DAMAGE, 25)));
	m->addSpellKnown(spellSharedPtr(new spell("Weep", TCODColor::green, ATTACK_BUFF_SELF, 1, DROP_DRAINING_POOL, 1)));
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->setFragmentsDropped(4000);
	m->addItemDrop(key_MoonpaleKey());
	m->addItemDrop(heart_PaleHeart());
	return m;
}

monsterSharedPtr monster_SoulEater()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Soul Eater", WISP_TILE, TCODColor::cyan,
		"An ethereal skeleton, its massive jaw unhinged, roams the halls in long, loping strides."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_MAGIC, 100);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, DAMAGE_MAGIC, 40)));
	m->addSpellKnown(spellSharedPtr(new spell("Drain Soul", TCODColor::cyan, 1, 0, DRAIN_VIGOUR, 1)));
	m->setSpellCastChance(25);
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_TheKingsRemnants()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("The King's Remnants", KNIGHT_TILE, TCODColor::lightCyan,
		"This withered husk wears the ruins of a king's robe, and on its head is perched a golden crown. It floats above \
the ground in a cloud of pale mist, which nearly forms the shape of a giant hand."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setDefence(DAMAGE_MAGIC, 75);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(10, SPEED_SLOW);
	m->addSpellKnown(spellSharedPtr(new spell("Drain Soul", TCODColor::cyan, 1, 0, DRAIN_VIGOUR, 2)));
	m->setFragmentsDropped(1);
	m->isBoss = true;
	m->addItemDrop(key_WitheredFinger());
	m->addItemDrop(headgear_MoonPaleCrown());
	return m;
}



/*
	Ancestral Tombs of Farin
*/


monsterSharedPtr monster_DeathwatchStatue()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Deathwatch Statue", STATUE_TILE, TCODColor::lightGreen,
		"An immobile stone statue in the shape of a gravekeeper. Its face is eyeless, but you feel its stare nonetheless."));
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 80);
	m->setDefence(DAMAGE_FIRE, 100);
	m->setDefence(DAMAGE_MAGIC, 100);
	m->setDefence(DAMAGE_ELECTRIC, 50);
	m->immobile = true;
	m->setFragmentsDropped(5);
	m->setMeleeStats(0, SPEED_SLOW);
	m->addSpellKnown(ability_Deathwatch());
	m->setSpellCastChance(100);
	return m;
}

monsterSharedPtr monster_Gravekeeper()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Gravekeeper", GRAVEKEEPER_TILE, TCODColor::sepia,
		"A loping figure in putrid rags stalks among the graves, clutching a rotted spear."));
	m->setHealth(200);
	m->setPlagueResist(1000);
	m->setPoisonResist(100);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_ACID, 50);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_NORMAL, EFFECT_PLAGUE, 10)));
	m->addSpellKnown(spellSharedPtr(new spell("Corrosion Spear", TCODColor::lime, 4, 15, APPLY_ACID_DAMAGE, 20)));
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_PlagueRottedZombie()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Plague-Rotted Zombie", ZOMBIE_TILE, TCODColor::lightSepia,
		"Shambling remnant of the vile dead, putrid with plague."));
	m->setHealth(400);
	m->addDamageResist(DAMAGE_ACID, 25);
	m->setPlagueResist(1000);
	m->setPoisonResist(1000);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_SLOW, EFFECT_PLAGUE, 15)));
	m->addSpellKnown(spellSharedPtr(new spell("Plaguebomb", TCODColor::sepia, 4, 0, APPLY_PLAGUE_DAMAGE, 5)));
	m->setSpellCastChance(15);
	m->setFragmentsDropped(50);
	return m;
}

monsterSharedPtr monster_GravekeeperSelvix()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Gravekeeper Selvix", GRAVEKEEPER_TILE, TCODColor::lightAmber,
		"There is little to distinguish him from the other gravekeepers, except for the hooked scimitar he wields alongside \
his spear, and the furious yellow light in his eyes."));
	m->setHealth(2000);
	m->setPlagueResist(1000);
	m->setPoisonResist(1000);
	m->setBleedResist(120);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(20, SPEED_FAST, EFFECT_PLAGUE, 20)));
	m->addSpellKnown(attack_Quickstep());
	m->addSpellKnown(spellSharedPtr(new spell("Plaguebomb", TCODColor::amber, 4, 10, APPLY_PLAGUE_DAMAGE, 20)));
	m->setSpellCastChance(40);
	m->setFragmentsDropped(4000);
	m->keepsDistance = true;
	m->addItemDrop(heart_PlagueRiddenHeart());
	m->addItemDrop(key_FarinsKey());
	m->isBoss = true;
	return m;
}

monsterSharedPtr monster_StarvingRat()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Starving Rat", RAT_TILE, TCODColor::sepia,
		"A dog-sized rat, slavering for sustenance, its eyes hollow and gleaming."));
	m->setHealth(30);
	m->setPlagueResist(1000);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_PLAGUE, 20)));
	m->setFragmentsDropped(5);
	return m;
}

monsterSharedPtr monster_Tentacle()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Tentacle", TENTACLE_TILE, TCODColor::green,
		"A slimy tentacle, covered in tiny suckers, gropes from the muck."));
	m->setHealth(100);
	m->setPoisonResist(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_NORMAL);
	m->addSpellKnown(spellSharedPtr(new spell("Grasp", TCODColor::green, 1, 5, APPLY_ENTANGLING, 1)));
	m->setSpellCastChance(50);
	m->setFragmentsDropped(100);
	return m;
}

monsterSharedPtr monster_TombHorror()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Tomb Horror", SPIDER_TILE, TCODColor::lightAmber,
		"A gaping maw rises from the muck, ringed with millions of tiny teeth, surrounded by squirming throngs of tentacles."));
	m->setHealth(4000);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(50, SPEED_SLOW, DAMAGE_ACID, 25)));
	m->addSpellKnown(spellSharedPtr(new spell("Rotblast", TCODColor::amber, 3, 50, APPLY_PLAGUE_DAMAGE, 30)));
	m->addSpawnableCreature("tentacle");
	m->setSpawnChance(75);
	m->keepsDistance = true;
	m->setFragmentsDropped(5000);
	m->addItemDrop(heart_HeartOfFarin());
	m->addItemDrop(key_AtalundraRunekey());
	m->isBoss = true;
	return m;
}




/*
	Sunken Atalundra
*/



monsterSharedPtr monster_LanternBearer()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Lantern Bearer", LANTERN_BEARER_TILE, TCODColor::lightCyan,
		"A withered figure, still clutching an enchanted lantern, watches you as you pass by."));
	m->setHealth(100);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(5, SPEED_SLOW);
	m->setDefence(DAMAGE_MAGIC, 90);
	m->immobile = true;
	m->setLightEmitted(0.4);
	m->addSpellKnown(spellSharedPtr(new spell("Atalundra Lightspear", TCODColor::cyan, 8, 0, APPLY_MAGIC_DAMAGE, 50)));
	m->setSpellCastChance(5);
	m->setFragmentsDropped(5);
	return m;
}

monsterSharedPtr monster_ShadeOfAtalundra()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Shade of Atalundra", GHOST_TILE, TCODColor::lightOrange,
		"Ghostly figure of a scholar, clutching a tome and raising a spectral wand."));
	m->setHealth(300);
	m->setMeleeStats(10, SPEED_SLOW);
	m->setDefence(DAMAGE_MAGIC, 100);
	m->setDefence(DAMAGE_ACID, 50);
	m->addSpellKnown(spellSharedPtr(new spell("Magic Missile", TCODColor::magenta, 5, 0, APPLY_MAGIC_DAMAGE, 50)));
	m->addSpellKnown(spellSharedPtr(new spell("Acid Shot", TCODColor::green, 3, 0, APPLY_ACID_DAMAGE, 75)));
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(200);
	return m;
}

monsterSharedPtr monster_DistortedHorror()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Distorted Horror", SPIDER_TILE, TCODColor::darkPurple,
		"A ruined mass of limbs, heads, and eyes, moaning in perpetual agony - or perhaps incomprehensible delight."));
	m->setHealth(500);
	m->setDefence(DAMAGE_PHYSICAL, 15);
	m->setDefence(DAMAGE_FIRE, 25);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(100, SPEED_SLOW);
	m->setFragmentsDropped(300);
	return m;
}

monsterSharedPtr monster_HeartOfAtalundra()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Heart of Atalundra", HEART_TILE, TCODColor::red,
		"A singular beating heart rests in the muck, exuding magical surges."));
	m->setHealth(3000);
	m->immobile = true;
	m->makeProfane();
	m->setMeleeStats(1, SPEED_SLOW);
	m->addSpellKnown(spellSharedPtr(new spell("Surge of Power", TCODColor::magenta, ATTACK_AOE, 20,
		APPLY_MAGIC_DAMAGE, 100)));
	m->addSpellKnown(spellSharedPtr(new spell("Push", TCODColor::white, ATTACK_AOE, 5, KNOCKBACK_TARGET, 5)));
	m->setSpellCastChance(25);
	m->setFragmentsDropped(5000);
	m->isBoss = true;
	m->addItemDrop(key_BeatingHeart());
	return m;
}

monsterSharedPtr monster_RatKing()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Rat King", RAT_TILE, TCODColor::darkSepia,
		"The lord of rats writhes at the centre of his shrieking host, wearing a crown of agony."));
	m->setHealth(50);
	m->makeProfane();
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(10, SPEED_SLOW);
	m->keepsDistance = true;
	m->setDefence(DAMAGE_PROFANE, 100);
	m->setDefence(DAMAGE_MAGIC, 100);
	m->setFragmentsDropped(4000);
	m->addSpawnableCreature("rat_spawn");
	m->setSpawnChance(50);
	m->isBoss = true;
	return m;
}

monsterSharedPtr monster_RatSpawn()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Ratspawn", RAT_TILE, TCODColor::lightSepia,
		"Loathsome spawn of the lord of rats, writhing at his feet in holy anguish."));
	m->setHealth(25);
	m->makeProfane();
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_NORMAL, EFFECT_POISON, 5)));
	m->setMoveStats(SPEED_FAST);
	m->setFragmentsDropped(0);
	return m;
}

monsterSharedPtr monster_SirPercivel()
{
	monsterSharedPtr m = monsterSharedPtr(new monster("Sir Percivel the Enslaved", KNIGHT_TILE, TCODColor::green,
		"Ruined and tattered, Sir Percivel stands before you, wielding a broken sword."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setDefence(DAMAGE_ELECTRIC, 20);
	m->setPoisonResist(45);
	m->setBleedResist(60);
	m->equipItem(weapon_SirPercivelsSword());
	m->setSpellCastChance(20);
	m->setMoveStats(SPEED_NORMAL);
	m->isBoss = true;
	m->makeProfane();
	m->setFragmentsDropped(1000);
	m->addItemDrop(charm_VoidwalkersRing());
	return m;
}


/*
	The Void
*/


monsterSharedPtr monster_VoidTouched()
{
	monsterSharedPtr m(new monster("Void-Touched", GHOST_TILE, TCODColor::lighterPurple,
		"These figures seem to shift in and out of existence, their faces contorted in agony."));
	m->setHealth(25);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(25, SPEED_NORMAL);
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_AstralJailorFireAcid()
{
	monsterSharedPtr m(new monster("Lost Blackwatch Knight", JAILOR_TILE, TCODColor::flame,
		"A knight shrouded in a halo of flame and acid."));
	m->setHealth(100);
	m->setDefence(DAMAGE_FIRE, 90);
	m->setDefence(DAMAGE_ACID, 90);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setFragmentsDropped(100);
	m->addSpellKnown(spellSharedPtr(new spell("Astral Beam", TCODColor::flame, ATTACK_RANGE, 5,
		APPLY_FIRE_DAMAGE, 15, APPLY_ACID_DAMAGE, 15)));
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_AstralJailorElectricMagic()
{
	monsterSharedPtr m(new monster("Lost Blackwatch Knight", JAILOR_TILE, TCODColor::lightPurple,
		"A knight shrouded in an arcane field, crackling with lightning."));
	m->setHealth(100);
	m->setDefence(DAMAGE_MAGIC, 90);
	m->setDefence(DAMAGE_ELECTRIC, 90);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setFragmentsDropped(100);
	m->addSpellKnown(spellSharedPtr(new spell("Astral Beam", TCODColor::flame, ATTACK_RANGE, 5,
		APPLY_ACID_DAMAGE, 15, APPLY_ELECTRIC_DAMAGE, 15)));
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_AstralJailorColdPhys()
{
	monsterSharedPtr m(new monster("Lost Blackwatch Knight", JAILOR_TILE, TCODColor::cyan,
		"A knight shrouded in frost."));
	m->setHealth(100);
	m->setDefence(DAMAGE_COLD, 90);
	m->setDefence(DAMAGE_PHYSICAL, 90);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setFragmentsDropped(100);
	m->addSpellKnown(spellSharedPtr(new spell("Astral Beam", TCODColor::flame, ATTACK_RANGE, 5,
		APPLY_COLD_DAMAGE, 15, APPLY_PHYSICAL_DAMAGE, 15)));
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_AstralJailorPoisBleed()
{
	monsterSharedPtr m(new monster("Lost Blackwatch Knight", JAILOR_TILE, TCODColor::lime,
		"A knight drenched in blood and bleeding venom."));
	m->setHealth(100);
	m->setBleedResist(100);
	m->setPoisonResist(100);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(5, SPEED_NORMAL, EFFECT_BLEED, 10)));
	m->setFragmentsDropped(100);
	m->addSpellKnown(spellSharedPtr(new spell("Poison Ray", TCODColor::lime, 5, 0, APPLY_POISON_DAMAGE, 10)));
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_AstralJailorPlague()
{
	monsterSharedPtr m(new monster("Lost Blackwatch Knight", JAILOR_TILE, TCODColor::sepia,
		"A knight rotten with plague."));
	m->setHealth(100);
	m->setPlagueResist(100);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_NORMAL, EFFECT_PLAGUE, 10)));
	m->setFragmentsDropped(100);
	m->addSpellKnown(ability_ShadowWalk());
	m->setSpellCastChance(50);
	m->makeImmuneToVoid();
	return m;
}

monsterSharedPtr monster_LordOfFallowFields()
{
	monsterSharedPtr m(new monster("The Lord of Fallow Fields", FINAL_BOSS_TILE, TCODColor::lightestPurple,
		"The Lord of famine and death strides across the astral precipice, wielding a blade infused with the profane \
energy of death itself."));
	m->setHealth(5000);
	m->setDefence(DAMAGE_PHYSICAL, 50);
	m->setBleedResist(100);
	m->setPoisonResist(100);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setDefence(DAMAGE_COLD, 25);
	m->setDefence(DAMAGE_ACID, 25);
	m->setDefence(DAMAGE_ELECTRIC, 25);
	m->equipItem(weapon_GreatswordOfTheAncientLords());
	m->setMoveStats(SPEED_NORMAL);
	m->addSpellKnown(prayer_ProfoundStill());
	m->addSpellKnown(prayer_ProfaneRay());
	m->setSpellCastChance(50);
	m->setFragmentsDropped(10000);
	m->makeImmuneToVoid();
	m->isBoss = true;
	return m;
}

monsterSharedPtr monster_TheDeathlessKnight()
{
	monsterSharedPtr m(new monster("The Deathless Knight", KNIGHT_TILE, TCODColor::pink,
		"A silent suit of armour regards you from across the room, glowing sword in hand."));
	m->setHealth(200);
	m->setDefence(DAMAGE_PHYSICAL, 100);
	m->setDefence(DAMAGE_MAGIC, 100);
	m->setDefence(DAMAGE_ACID, 100);
	m->setDefence(DAMAGE_COLD, 100);
	m->setDefence(DAMAGE_ELECTRIC, 100);
	m->setDefence(DAMAGE_FIRE, 100);
	m->setDefence(DAMAGE_BLESSED, 100);
	m->setDefence(DAMAGE_PROFANE, 100);
	m->setBleedResist(1000);
	m->setPoisonResist(1000);
	m->setPlagueResist(1000);
	m->isBoss = true;
	m->setFragmentsDropped(5000);
	m->equipItem(weapon_DeathlessBlade());
	m->setSpellCastChance(25);
	m->addItemDrop(weapon_DeathlessBlade());
	return m;
}



/*
	SOME RANDOM UNIQUE ENEMIES
*/


monsterSharedPtr monster_SlaveOfPash()
{
	monsterSharedPtr m(new monster("Slave of Pash", PRIEST_TILE, TCODColor::lightRed,
		"Agonized devotee of the great god Pash, bound in chains and shrieking in blessed agony. As the scriptures of Pash state, \
the blood drains from the body, but the spirit will never perish."));
	m->setHealth(1000);
	m->setDefence(DAMAGE_PHYSICAL, 25);
	m->setDefence(DAMAGE_FIRE, 50);
	m->addWeakness(DAMAGE_COLD);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(20, SPEED_FAST, DAMAGE_COLD, 40)));
	m->addSpellKnown(spellSharedPtr(new spell("Agony", TCODColor::crimson, ATTACK_BUFF_SELF, 0, APPLY_BLEED_DAMAGE, 20)));
	m->addSpellKnown(spellSharedPtr(new spell("Ecstasy", TCODColor::yellow, ATTACK_BUFF_SELF, 0, RESTORE_HEALTH, 100)));
	m->setSpellCastChance(25);
	m->isBoss = true;
	m->showBossHealthBar = false;
	m->addItemDrop(misc_BrandOfTheEmissary());
	m->setFragmentsDropped(1000);
	return m;
}


/*
	GODS
*/


monsterSharedPtr monster_GlurgTheImbecile()
{
	monsterSharedPtr m(new monster("Glurg the Imbecile", GLURG_TILE, TCODColor::darkGreen,
		"The earthly avatar of Glurg manifests as a drooling toad, shuddering in the deep. Perhaps when the imbecile god \
devoured its forebears, the revelation of the truth beyond the stars proved unbearable."));
	m->setHealth(3000);
	m->healthTrickle = 3000;
	m->setMoveStats(SPEED_NORMAL);
	m->keepsDistance = true;
	m->setPoisonResist(1000);
	m->setDefence(DAMAGE_PHYSICAL, 80);
	m->addWeakness(DAMAGE_FIRE);
	m->isBoss = true;
	m->setFragmentsDropped(5000);
	m->addSpellKnown(spellSharedPtr(new spell("Hop", TCODColor::yellow, ATTACK_BUFF_SELF, 1, GAIN_FREE_MOVES, 4)));
	m->addSpellKnown(spellSharedPtr(new spell("Idiot's Prayer", TCODColor::lightYellow, ATTACK_BUFF_SELF, 1,
		RESTORE_HEALTH, 100)));
	m->setSpellCastChance(20);
	m->addSpawnableCreature("adherent_of_glug");
	m->setSpawnChance(60);
	m->addItemDrop(heart_HeartOfTheOldGods());
	return m;
}

monsterSharedPtr monster_PashTheRavenous()
{
	monsterSharedPtr m(new monster("Pash the Ravenous", PASH_TILE, TCODColor::lightestCrimson,
		"The earthly avatar of Pash manifests as a creature composed entirely of mouths, all agape and bristling with teeth. Pale \
eyes peer from the darkness within Pash's myriad maws, and saliva drips upon the ground."));
	m->setHealth(5000);
	m->setMoveStats(SPEED_NORMAL);
	m->keepsDistance = true;
	m->makeProfane();
	m->setBleedResist(1000);
	m->setDefence(DAMAGE_PHYSICAL, 40);
	m->setDefence(DAMAGE_FIRE, 30);
	m->addWeakness(DAMAGE_ELECTRIC);
	m->isBoss = true;
	m->setFragmentsDropped(5000);
	m->addSpellKnown(spellSharedPtr(new spell("Planar Shift", TCODColor::purple, 8, 0, TELEPORT, 5)));
	m->addSpellKnown(spellSharedPtr(new spell("Corrosion", TCODColor::red, ATTACK_BUFF_SELF, 1, DROP_CORRODING_BLOOD, 1)));
	m->setSpellCastChance(50);
	m->equipItem(weaponSharedPtr(new weapon(25, SPEED_NORMAL, DAMAGE_FIRE, 15)));
	m->addSpawnableCreature("bleeding_maw");
	m->setSpawnChance(5);
	return m;
}

monsterSharedPtr monster_TheBlindKing()
{
	monsterSharedPtr m(new monster("The Blind King", BLIND_KING_TILE, TCODColor::lightAmber,
		"The master of the blind worms that gibber at the base of the universe. This living worm shrieks endlessly, exuding \
putrid toxins from every pore."));
	m->setHealth(8000);
	m->setDefence(DAMAGE_FIRE, 60);
	m->setDefence(DAMAGE_ACID, 60);
	m->setPoisonResist(1000);
	m->setPlagueResist(100);
	m->setMoveStats(SPEED_SLOW);
	m->equipItem(weaponSharedPtr(new weapon(30, SPEED_NORMAL, EFFECT_POISON, 40)));
	m->addSpellKnown(spellSharedPtr(new spell("The King's Eyes", TCODColor::blue, 4, 0, APPLY_BLINDING, 5)));
	m->addSpellKnown(spellSharedPtr(new spell("Dragging Stare", TCODColor::white, 8, 10, PULL_CLOSER, 5)));
	m->addSpellKnown(spellSharedPtr(new spell("The King's Scream", TCODColor::lightYellow, ATTACK_BUFF_SELF, 0,
		APPLY_DAMAGE_PENALTY, -10)));
	m->setSpellCastChance(60);
	m->isBoss = true;
	m->setFragmentsDropped(5000);
	return m;
}

monsterSharedPtr monster_RuinsOfTheOldGods()
{
	monsterSharedPtr m(new monster("Ruins of the Old Gods", WRETCH_TILE, TCODColor::white,
		"Sordid wreckage of an old god, withered beyond recognition."));
	m->setHealth(1000);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_SLOW);
	m->keepsDistance = true;
	m->addItemDrop(gem_GodlessGemstone());
	return m;
}


/*
TRUE FINAL BAWS
*/
monsterSharedPtr monster_QomTheGibberingSpider()
{
	monsterSharedPtr m(new monster("Qom, the Gibbering Spider", SPIDER_TILE, TCODColor::lightRed,
		"The devourer of the ancients gibbers at the heart of the void, weeping the ichor of obliteration from its \
myriad eyes."));
	m->setHealth(8000);
	m->setDefence(DAMAGE_PHYSICAL, 80);
	m->setDefence(DAMAGE_ACID, 25);
	m->setDefence(DAMAGE_COLD, 25);
	m->setDefence(DAMAGE_FIRE, 25);
	m->setDefence(DAMAGE_ELECTRIC, 25);
	m->setDefence(DAMAGE_MAGIC, 25);
	m->makeImmuneToVoid();
	m->keepsDistance = true;
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(50, SPEED_SLOW, EFFECT_PLAGUE, 20)));
	m->addSpawnableCreature("astral_spider");
	m->setSpawnChance(50);
	m->isBoss = true;
	return m;
}

//TRUE FINAL BAWS ASSISTANT
monsterSharedPtr monster_AstralSpider()
{
	monsterSharedPtr m(new monster("Astral Spider", SPIDER_TILE, TCODColor::lightestPurple,
		"One of Qom's endless spawn, gibbering with incoherent rage."));
	m->setHealth(50);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(20, SPEED_FAST, DAMAGE_PROFANE, 20)));
	m->makeImmuneToVoid();
	return m;
}


/*
	FRIENDLIES
*/


monsterSharedPtr npc_Gorem()
{
	monsterSharedPtr m(new monster("Gorem, Ancient of Wyrd", GOREM_TILE, TCODColor::lighterGrey,
		"At first it looks like a hulking statue, overgrown with flowers and vines, but a light appears in its eyes as \
you approach."));
	m->setHealth(800);
	m->isShopkeeper = true;
	m->isHostile = false;
	m->setShopTag("gorem_shop");
	//Chatting
	m->loadDialogue("dialogue/gorem_chat.txt");
	return m;
}

monsterSharedPtr npc_UtricTheRat()
{
	monsterSharedPtr m(new monster("Utric the Rat", PLAYER_TILE, TCODColor::lighterSepia,
		"A scrawny man in ragged clothes, with a curiously rat-like visage. He watches you eagerly."));
	m->setHealth(100);
	m->isHostile = false;
	m->isShopkeeper = true;
	m->setShopTag("utric_shop");
	m->addShopUnlock("waterlogged_writings");
	m->addShopUnlock("minas_profaned_writings");
	m->addShopUnlock("frostbitten_writings");
	m->addShopUnlock("singed_writings");
	//Dialogue
	m->loadDialogue("dialogue/utric_chat.txt");
	m->addSpecialDialogue("waterlogged_writings",
		"Aha! So Gottric's journeys brought him to the lowlands! I suspect he was on his way to the Winter Court. But who knows \
what he found there?");
	m->addSpecialDialogue("minas_profaned_writings",
		"It must have troubled Gottric to record these awful words! He was always a man of the most blessed devotion, never \
turning away from the glorious light of the old gods, even during the age of despair.");
	m->addSpecialDialogue("frostbitten_writings",
		"I suppose I thought that Gottric's journeys would end at the Winter Court, but I suppose I was wrong! Where could he \
have gone next?");
	m->addSpecialDialogue("singed_writings",
		"Yes, the Sparrows knew certain magicks that were meant to have been forgotten... Nonetheless, I wonder what profane \
inspiration drove Gottric to such depths of insanity!");
	return m;
}

monsterSharedPtr npc_MuiraClericOfTheRose()
{
	monsterSharedPtr m(new monster("Muira, Cleric of the Rose", PLAYER_TILE, TCODColor::darkPink,
		"A narrow-faced woman dressed in stained red robes."));
	m->setHealth(120);
	m->isHostile = false;
	m->isShopkeeper = true;
	m->setShopTag("muira_shop");
	m->addShopUnlock("orsyls_tome_of_prayer");
	m->addShopUnlock("divine_moonspark_tome");
	m->addShopUnlock("divine_tome_of_the_emissary");
	m->addShopUnlock("divine_nightmare_tome");
	m->addShopUnlock("tome_of_the_dead");
	//Dialogue
	m->loadDialogue("dialogue/muira_chat.txt");
	m->addSpecialDialogue("orsyls_tome_of_prayer",
		"Have an interest in the profane, do you? Hee hee... Careful, that could lead to the grip of an unknowable depravity, \
of the kind that the old gods forsook!");
	m->addSpecialDialogue("divine_moonspark_tome",
		"The Murdermire was once a beautiful place, you know, and it had quite a different name! But those were Yutria's times, \
when the gods still answered prayers...");
	m->addSpecialDialogue("divine_tome_of_the_emissary",
		"Be careful now! Some say that Lady Tvert's emissaries are no longer agents of purity and light, but that she's dragged \
them into the same decay that consumed her and her accursed scion...");
	m->addSpecialDialogue("divine_nightmare_tome",
		"Learning about Ietra, are we? Let me give you some advice... If you take the road of the old saints, you'll end up as \
they did - we all know how that went!");
	m->addSpecialDialogue("tome_of_the_dead",
		"So you seek the secrets of the dead, just as they did in Sunken Atalundra! You know their city was drowned, surely, and \
that their ancient teachings were lose forever? Do you desire the same fate?");
	return m;
}

monsterSharedPtr npc_ElenaThePilgrim()
{
	monsterSharedPtr m(new monster("Elena the Pilgrim", PLAYER_TILE, TCODColor::lightCyan,
		"A woman in travel-worn robes, dragging a cart piled with goods."));
	m->setHealth(80);
	m->isHostile = false;
	m->isShopkeeper = true;
	m->setShopTag("elena_shop");
	m->addShopUnlock("piece_of_jade");
	m->addShopUnlock("piece_of_bone");
	//Dialogue
	m->loadDialogue("dialogue/elena_chat.txt");
	m->addSpecialDialogue("piece_of_jade",
		"What a lovely piece of jade! It reminds me of home...");
	m->addSpecialDialogue("piece_of_bone",
		"Oh - say, a scrap of bone! It brings back strange memories...");
	return m;
}

monsterSharedPtr npc_VoidwalkerDaedelus()
{
	monsterSharedPtr m(new monster("Voidwalker Daedelus", PLAYER_TILE, TCODColor::purple,
		"A mournful-looking man in purple robes. He seems to be listening intently."));
	m->setHealth(200);
	m->isHostile = false;
	//Dialogue
	m->loadDialogue("dialogue/daedelus_chat.txt");
	return m;
}

monsterSharedPtr npc_Ydella()
{
	monsterSharedPtr m(new monster("Ydella, Ancient of Wyrd", GOREM_TILE, TCODColor::lightestGreen,
		"This figure of a woman seems to be made of stone, until a sudden light shines in her eyes."));
	m->setHealth(500);
	m->isHostile = false;
	m->isShopkeeper = true;
	//Items are all acquired via HEART-TRADING
	m->setShopTag("ydella_shop");
	m->addShopUnlock("wretched_heart");
	m->addShopUnlock("old_crows_heart");
	m->addShopUnlock("fishborn_heart");
	m->addShopUnlock("orsyls_heart");
	m->addShopUnlock("vortens_heart");
	m->addShopUnlock("venomous_spider_heart");
	m->addShopUnlock("lady_tverts_heart");
	m->addShopUnlock("dead_sparrows_heart");
	m->addShopUnlock("corens_heart");
	m->addShopUnlock("khalles_heart");
	m->addShopUnlock("ietras_heart");
	m->addShopUnlock("pale_heart");
	m->addShopUnlock("plague_ridden_heart");
	m->addShopUnlock("heart_of_farin");
	m->addShopUnlock("rat_kings_heart");
	//Dialogue
	m->loadDialogue("dialogue/ydella_chat.txt");
	m->addSpecialDialogue("wretched_heart",
		"Alas, the poor wretches! I saw them fleeing the ruin of the city, but there was nothing I could do...");
	m->addSpecialDialogue("old_crows_heart",
		"Shame! The Old Crow and I were once friends, and we spoke about the old days, before the stars left the sky...");
	m->addSpecialDialogue("fishborn_heart",
		"Pshaw! Creatures of the deep, one and all, deserve none of our pity - they devoured everything that mattered...");
	m->addSpecialDialogue("orsyls_heart",
		"Orsyl dead too? All the old clerics dying, one after the other, in the sordid light of the nightmare!");
	m->addSpecialDialogue("vortens_heart",
		"A thousand curses be heaped upon Vorten! His betrayal was our downfall, the cause of the accursed gaze of Pash!");
	m->addSpecialDialogue("venomous_spiders_heart",
		"My, this heart has a particular toxicity! Surely it came from a creature most disgustingly vile!");
	m->addSpecialDialogue("dead_sparrows_heart",
		"Be careful, little pilgrim! I've heard that it's not wise for certain creatures to die...");
	m->addSpecialDialogue("corens_heart",
		"Coren, scourge of the lowlands, laid low at last!");
	m->addSpecialDialogue("khalles_heart",
		"Khalle, madman whom the gods ignored, laid low at last!");
	m->addSpecialDialogue("ietras_heart",
		"Ietra, queen of the abyss, captain of the abandoned, laid low at last!");
	m->addSpecialDialogue("pale_heart",
		"This heart is singularly cold! Is it the heart of the old king, or just his echo?");
	m->addSpecialDialogue("heart_of_farin",
		"Alas, Farin! He was fascinated by the truth that lay beyond death, I heard, but perhaps he didn't find it to his liking...");
	m->addSpecialDialogue("rat_kings_heart",
		"Now here's a revelation best left alone...");
	return m;
}

monsterSharedPtr npc_TheEyelessQueen()
{
	monsterSharedPtr m(new monster("The Eyeless Queen", PLAYER_TILE, TCODColor::darkCyan,
		"Her regal finery is encrusted with ice, her eye sockets are empty, and her lips are curled in the faintest \
of smiles."));
	m->setHealth(100);
	m->isHostile = false;
	m->loadDialogue("dialogue/eyeless_queen_chat.txt");
	return m;
}

monsterSharedPtr npc_ToadPriestGhulluk()
{
	monsterSharedPtr m(new monster("Toad Priest Ghulluk", TOAD_TILE, TCODColor::lighterSea,
		"This tiny, toadlike man clutches a gnarled wooden staff, and leers at your from under his mud-stained hood."));
	m->setHealth(500);
	m->isHostile = false;
	//Shop
	m->setShopTag("ghulluk_shop");
	m->addShopUnlock("watchful_eyestalk");
	m->addShopUnlock("mawtooth_fragment");
	m->addShopUnlock("withered_finger");
	m->addShopUnlock("heart_of_the_old_gods");
	//Dialogue
	m->loadDialogue("dialogue/ghulluk_chat.txt");
	m->addSpecialDialogue("watchful_eyestalk",
		"Haha, excellent! The eyestalk of Vorten, delver into the watchful deeps, will serve our purpose!");
	m->addSpecialDialogue("mawtooth_fragment",
		"Good, good! The mawtooth of the ravenous Pash, hungering for everlasting flesh, will serve our purpose!");
	m->addSpecialDialogue("withered_finger",
		"Remarkable! This finger, pointing to gibbering oblivion, speaks to a forgotten truth!");
	return m;
}

monsterSharedPtr npc_SiltraTheRedEyed()
{
	monsterSharedPtr m(new monster("Siltra the Red-Eyed", PLAYER_TILE, TCODColor::red,
		"A forlorn woman sits among the ruins. Her eyes shine bright red."));
	m->setHealth(1000);
	m->isHostile = false;
	m->loadDialogue("dialogue/siltra_chat.txt");
	return m;
}

monsterSharedPtr npc_CastalaTheExile()
{
	monsterSharedPtr m(new monster("Castala the Exile", PLAYER_TILE, TCODColor::lightBlue,
		"A woman in a regal dress, dripping golden jewelry, regards you haughtily. What is she doing in this strange place?"));
	m->setHealth(100);
	m->isHostile = false;
	m->loadDialogue("dialogue/castala_chat.txt");
	return m;
}

monsterSharedPtr npc_BlackwatchKnightShamash()
{
	monsterSharedPtr m(new monster("Blackwatch Knight Shamash", PLAYER_TILE, TCODColor::lightPurple,
		"A figure in jet-black armour, sorting through a pile of rubble."));
	m->setHealth(1000);
	m->isHostile = false;
	m->setShopTag("shamash_shop");
	m->loadDialogue("dialogue/shamash_chat.txt");
	return m;
}

monsterSharedPtr npc_BlackwatchCaptainAnshar()
{
	monsterSharedPtr m(new monster("Blackwatch Captain Anshar", PLAYER_TILE, TCODColor::lightestPurple,
		"A figure in jet-black armour, radiating the pure energy of the Void."));
	m->setHealth(1000);
	m->isHostile = false;
	m->loadDialogue("dialogue/anshar_chat.txt");
	return m;
}

monsterSharedPtr npc_ShadeOfCastala()
{
	monsterSharedPtr m(new monster("Shade of Castala, the Exile", PLAYER_TILE, TCODColor::lightBlue,
		"Shadow of a woman in a regal dress, dripping golden jewelry."));
	m->setHealth(100);
	m->isHostile = false;
	m->loadDialogue("dialogue/castala_shade_chat.txt");
	return m;
}

monsterSharedPtr npc_ShadeOfGottric()
{
	monsterSharedPtr m(new monster("Shade of Gottric", PLAYER_TILE, TCODColor::lightPurple,
		"Shadow of a man in tattered mage's robes."));
	m->setHealth(100);
	m->isHostile = false;
	m->loadDialogue("dialogue/gottric_shade_chat.txt");
	return m;
}

monsterSharedPtr npc_TrainingDummy()
{
	monsterSharedPtr m(new monster("Training Dummy", PLAYER_TILE, TCODColor::lighterYellow,
		"An immobile training dummy."));
	m->setHealth(5000);
	m->immobile = true;
	return m;
}

/*
This giant nightmare is how monsters are defined in map files.
*/
monsterSharedPtr getMonsterByHandle(std::string handle)
{

	//Stardrift Wreckage
	if (handle == "drowned_crew")
		return monster_DrownedCrew();

	//Coruscating Beach
	else if (handle == "thin_wretch")
		return monster_ThinWretch();
	else if (handle == "timid_wretch")
		return monster_TimidWretch();
	else if (handle == "bloated_wretch")
		return monster_BloatedWretch();
	else if (handle == "the_wretched_mass")
		return boss_TheWretchedMass();

	//Pilgrim's Road
	else if (handle == "crow_knight")
		return monster_CrowKnight();
	else if (handle == "crow_thief")
		return monster_CrowThief();
	else if (handle == "crow_archer")
		return monster_CrowArcher();
	else if (handle == "forlorn_crow_knight")
		return monster_ForlornCrowKnight();
	else if (handle == "the_old_crow")
		return monster_TheOldCrow();

	//Flooded Lowlands
	else if (handle == "starving_fishman")
		return monster_StarvingFishman();
	else if (handle == "fishman_doom_preacher")
		return monster_FishmanDoomPreacher();
	else if (handle == "fishman_spearfisher")
		return monster_FishmanSpearfisher();
	else if (handle == "guardian_golem")
		return monster_GuardianGolem();
	else if (handle == "degenerate_fishman")
		return monster_DegenerateFishman();
	else if (handle == "skinless_knight")
		return monster_SkinlessKnight();
	else if (handle == "fishborn_goddess")
		return monster_FishbornGoddess();

	//Darkwater Forest
	else if (handle == "giant_rat")
		return monster_GiantRat();
	else if (handle == "profane_rat_priest")
		return monster_ProfaneRatPriest();
	else if (handle == "rat_prince")
		return monster_RatPrince();

	//Crumbling City
	else if (handle == "crawling_zombie")
		return monster_CrawlingZombie();
	else if (handle == "firespitter_zombie")
		return monster_FirespitterZombie();
	else if (handle == "oozing_zombie")
		return monster_OozingZombie();
	else if (handle == "devotee_of_pash")
		return monster_DevoteeOfPash();
	else if (handle == "voidwalker_minas")
		return monster_VoidwalkerMinas();
	else if (handle == "bloodthirsty_hound")
		return monster_BloodthirstyHound();
	else if (handle == "bloodstarved_zombie")
		return  monster_BloodstarvedZombie();
	else if (handle == "orsyl_the_profaned")
		return monster_HighClericOrsylTheProfaned();
	else if (handle == "cursed_knight")
		return monster_CursedKnight();
	else if (handle == "cursed_archer")
		return monster_CursedArcher();
	else if (handle == "enkidu")
		return monster_Enkidu();
	else if (handle == "duke_vorten")
		return monster_DukeVortenTheWatchful();

	//Winter Court
	else if (handle == "winter_shade")
		return monster_WinterShade();
	else if (handle == "hollowfaced_knight")
		return monster_HollowfacedKnight();

	//Murdermire
	else if (handle == "wisp")
		return monster_Wisp();
	else if (handle == "drowned_dead")
		return monster_DrownedDead();
	else if (handle == "swamp_spider")
		return monster_SwampSpider();
	else if (handle == "adherent_of_glug")
		return monster_AdherentOfGlug();
	else if (handle == "ghorth")
		return monster_GhorthTheBloatedSpider();

	//Tear-Stained Valley
	else if (handle == "undead_prisoner")
		return monster_UndeadPrisoner();
	else if (handle == "bloated_slave_driver")
		return monster_BloatedSlaveDriver();
	else if (handle == "slavering_hound")
		return monster_SlaveringHound();
	else if (handle == "emissary")
		return monster_Emissary();
	else if (handle == "barton")
		return monster_BartonTheProwlingMinister();

	//Desolate Village
	else if (handle == "slavemaster_gasgut")
		return monster_SlaveMasterGasgut();
	else if (handle == "white_fog_herald")
		return monster_WhiteFogHerald();

	//Hightower Banquet
	else if (handle == "hightower_lord")
		return monster_HightowerAristocrat();
	else if (handle == "silver_knight")
		return monster_SilverKnight();
	else if (handle == "lady_tvert")
		return monster_LadyTvert();
	else if (handle == "scion_of_tvert")
		return monster_ScionOfTvert();
	else if (handle == "mouth_of_pash")
		return monster_MouthOfPash();

	//Dead Sparrow's Tower
	else if (handle == "sparrow_knight")
		return monster_SparrowKnight();
	else if (handle == "ethereal_mage")
		return monster_EtherealMage();
	else if (handle == "decomposing_knight")
		return monster_DecomposingKnight();
	else if (handle == "lifedrinker_wraith")
		return monster_LifedrinkerWraith();
	else if (handle == "dead_sparrow")
		return monster_TheDeadSparrow();
	else if (handle == "mirror_image")
		return monster_MirrorImage();

	//Moshka's Observatory
	else if (handle == "starspawned")
		return monster_Starspawned();
	else if (handle == "ooze")
		return monster_Ooze();

	//Mausoleum of Blood
	else if (handle == "bleeding_maw")
		return monster_BleedingMaw();
	else if (handle == "crawling_wretch")
		return monster_CrawlingWretch();
	else if (handle == "coren_the_bloody")
		return monster_CorenTheBloody();

	//Mausoleum of the Lost
	else if (handle == "skeleton_knight")
		return monster_SkeletonKnight();
	else if (handle == "skeleton_archer")
		return monster_SkeletonArcher();
	else if (handle == "khalle_the_lost")
		return monster_KhalleTheLost();

	//Mausoleum of the Void
	else if (handle == "voidwalker_enkidu")
		return monster_VoidwalkerMarduk();
	else if (handle == "voidwalker_icarus")
		return monster_VoidwalkerIcarus();
	else if (handle == "voidwalker_penelope")
		return monster_VoidwalkerPenelope();
	else if (handle == "ietra")
		return monster_IetraEaterOfSilence();

	//Bridge of Lords
	else if (handle == "sentinel_pikeman")
		return monster_SentinelPikeman();
	else if (handle == "sentinel_archer")
		return monster_SentinelArcher();

	//Moonpale Wastes
	else if (handle == "ghostly_warrior")
		return monster_GhostlyWarrior();
	else if (handle == "ethereal_wisp")
		return monster_EtherealWisp();
	else if (handle == "weeping_wanderer")
		return monster_WeepingWanderer();
	else if (handle == "the_pale_shade")
		return monster_ThePaleShade();
	else if (handle == "soul_eater")
		return monster_SoulEater();
	else if (handle == "the_kings_remnants")
		return monster_TheKingsRemnants();

	//Ancestral Tombs of Farin
	else if (handle == "deathwatch_statue")
		return monster_DeathwatchStatue();
	else if (handle == "gravekeeper")
		return monster_Gravekeeper();
	else if (handle == "plague_rotted_zombie")
		return monster_PlagueRottedZombie();
	else if (handle == "selvix")
		return monster_GravekeeperSelvix();
	else if (handle == "starving_rat")
		return monster_StarvingRat();
	else if (handle == "tentacle")
		return monster_Tentacle();
	else if (handle == "tomb_horror")
		return monster_TombHorror();

	//Sunken Atalundra
	else if (handle == "lantern_bearer")
		return monster_LanternBearer();
	else if (handle == "shade_of_atalundra")
		return monster_ShadeOfAtalundra();
	else if (handle == "distorted_horror")
		return monster_DistortedHorror();
	else if (handle == "heart_of_atalundra")
		return monster_HeartOfAtalundra();

	//Rat King's Cathedral
	else if (handle == "rat_king")
		return monster_RatKing();
	else if (handle == "rat_spawn")
		return monster_RatSpawn();
	else if (handle == "sir_percivel")
		return monster_SirPercivel();

	//The Void
	else if (handle == "void_touched")
		return monster_VoidTouched();
	else if (handle == "astral_jailor_fire_acid")
		return monster_AstralJailorFireAcid();
	else if (handle == "astral_jailor_cold_phys")
		return monster_AstralJailorColdPhys();
	else if (handle == "astral_jailor_electric_magic")
		return monster_AstralJailorElectricMagic();
	else if (handle == "astral_jailor_pois_bleed")
		return monster_AstralJailorPoisBleed();
	else if (handle == "astral_jailor_plague")
		return monster_AstralJailorPlague();
	else if (handle == "lord_of_fallow_fields")
		return monster_LordOfFallowFields();
	else if (handle == "the_deathless_knight")
		return monster_TheDeathlessKnight();

	//The Gods
	else if (handle == "glurg_the_imbecile")
		return monster_GlurgTheImbecile();
	else if (handle == "pash_the_ravenous")
		return monster_PashTheRavenous();
	else if (handle == "the_blind_king")
		return monster_TheBlindKing();
	else if (handle == "ruins_of_the_old_gods")
		return monster_RuinsOfTheOldGods();
	else if (handle == "qom")
		return monster_QomTheGibberingSpider();
	else if (handle == "astral_spider")
		return monster_AstralSpider();

	//Misc special enemies
	else if (handle == "slave_of_pash")
		return monster_SlaveOfPash();

	//Friendly NPCs
	else if (handle == "gorem")
		return npc_Gorem();
	else if (handle == "utric")
		return npc_UtricTheRat();
	else if (handle == "muira")
		return npc_MuiraClericOfTheRose();
	else if (handle == "elena")
		return npc_ElenaThePilgrim();
	else if (handle == "daedelus")
		return npc_VoidwalkerDaedelus();
	else if (handle == "ydella")
		return npc_Ydella();
	else if (handle == "eyeless_queen")
		return npc_TheEyelessQueen();
	else if (handle == "ghulluk")
		return npc_ToadPriestGhulluk();
	else if (handle == "siltra")
		return npc_SiltraTheRedEyed();
	else if (handle == "castala")
		return npc_CastalaTheExile();
	else if (handle == "blackwatch_captain_anshar")
		return npc_BlackwatchCaptainAnshar();
	else if (handle == "blackwatch_knight_shamash")
		return npc_BlackwatchKnightShamash();
	else if (handle == "shade_of_castala")
		return npc_ShadeOfCastala();
	else if (handle == "shade_of_gottric")
		return npc_ShadeOfGottric();

	else if (handle == "dummy")
		return npc_TrainingDummy();

	//LET'S HOPE WE NEVER GET HERE!
	return nullptr;
}
