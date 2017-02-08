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


/*
For shopkeeper NPCs.
*/
void monster::addItemToStock(itemSharedPtr it, int price)
{
	it->isGlittery = false;
	it->setPrice(price);
	stock.push_back(it);
}

/*
These items are unlocked if the player has a specific key.
*/
void monster::addStockUnlock(itemSharedPtr it, int price, std::string unlockCode)
{
	it->isGlittery = false;
	it->setPrice(price);
	stockUnlocks.push_back(new unlockableStock(it,unlockCode));
}

/*
Cause we bought something!
*/
void monster::removeItemFromStock(itemSharedPtr it)
{
	auto iter = std::find(stock.begin(), stock.end(), it);
	if (iter != stock.end())
		stock.erase(iter);
}


/*
See if the given person has a key that will unlock additional stock items.
*/
void monster::checkForStockUnlocks(personSharedPtr unlocker)
{
	//Keep track of stuff to remove, if any
	stockUnlockVector toRemove;
	//Have a look
	for (auto unlock : stockUnlocks) {
		if (unlocker->hasKey(unlock->unlockCode)) {
			//Yup, here's one!
			stock.push_back(unlock->it);
			//And make sure we only add it once!
			toRemove.push_back(unlock);
		}
	}
	//Remove anything that we unlocked from the list to ensure it can't be unlocked twice
	for (auto rem : toRemove) {
		auto iter = std::find(stockUnlocks.begin(), stockUnlocks.end(), rem);
		stockUnlocks.erase(iter);
	}
}


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
	m->addSpellKnown(ability_CrowArrow());
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
	m->setHealth(400);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weapon_CrowKnife());
	m->addSpellKnown(attack_Quickstep());
	m->addSpellKnown(attack_Splintering());
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
	monsterSharedPtr m(new monster("Skinless Knight", CROW_KNIGHT_TILE, TCODColor::lightPink,
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
		"His armour glows with profane radiance, and his jet-black sword pulses with dark radiance."));
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
	monsterSharedPtr m(new monster("Adherent of Glug", TOAD_TILE, TCODColor::sea,
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
	m->setFragmentsDropped(500);
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
	m->setHealth(25);
	m->setMoveStats(SPEED_FAST);
	m->equipItem(weaponSharedPtr(new weapon(10, SPEED_FAST, EFFECT_BLEED, 10)));
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
	m->setSpellCastChance(50);
	m->isBoss = true;
	m->addSpawnableCreature("scion_of_tvert");
	m->setSpawnChance(25);
	m->addItemDrop(key_LadyTvertsKey());
	m->addItemDrop(heart_LadyTvertsHeart());
	m->setFragmentsDropped(1500);
	return m;
}

monsterSharedPtr monster_ScionOfTvert()
{
	monsterSharedPtr m(new monster("Scion of Tvert", ARISTOCRAT_TILE, TCODColor::lightPink,
		"Lady Tvert's bile forms the shape of this twisted creature - a mockery of the human form, \
his face riddled with scars, his arms gruesome chunks of flesh covered in vile worms."));
	m->setHealth(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setBleedResist(60);
	m->setPoisonResist(60);
	m->makeProfane();
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weaponSharedPtr(new weapon(0, SPEED_NORMAL, DAMAGE_PROFANE, 30)));
	m->addSpellKnown(ability_ShredSkin());
	m->setSpellCastChance(20);
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
	m->addItemToStock(shield_WoodenWyrdShield(), 25);
	m->addItemToStock(chime_WyrdBellbranch(), 50);
	m->addItemToStock(prayer_WyrdChantOfStrength(), 25);
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
	//Starting stock
	m->addItemToStock(charm_ArcanaDrenchedCharm(), 100);
	m->addItemToStock(spell_ArcaneBlade(), 50);
	m->addItemToStock(spell_GottricsArcaneProtection(), 40);
	m->addItemToStock(spell_Frostbolt(), 25);
	//Unlock: Waterlogged Writings
	m->addStockUnlock(spell_AcidSpit(), 50, "waterlogged_writings");
	m->addStockUnlock(spell_AcidBurst(), 75, "waterlogged_writings");
	m->addStockUnlock(spell_AcidBlade(), 100, "waterlogged_writings");
	//Unlock: Minas's Profaned Writings
	m->addStockUnlock(spell_ProfanedBlade(), 100, "minas_profaned_writings");
	m->addStockUnlock(spell_VoidJaunt(), 100, "minas_profaned_writings");
	m->addStockUnlock(spell_DevouringVoidCloud(), 150, "minas_profaned_writings");
	//Unlock: Frostbitten Writings
	m->addStockUnlock(spell_FrostBlast(), 100, "frostbitten_writings");
	m->addStockUnlock(spell_FrozenBlade(), 100, "frostbitten_writings");
	m->addStockUnlock(spell_Chillbite(), 100, "frostbitten_writings");
	//Dialogue
	m->loadDialogue("dialogue/utric_chat.txt");
	return m;
}

monsterSharedPtr npc_MuiraClericOfTheRose()
{
	monsterSharedPtr m(new monster("Muira, Cleric of the Rose", PLAYER_TILE, TCODColor::darkPink,
		"A narrow-faced woman dressed in stained red robes."));
	m->setHealth(120);
	m->isHostile = false;
	m->isShopkeeper = true;
	//Starting stock
	m->addItemToStock(charm_ClericsHolyPendant(), 100);
	m->addItemToStock(prayer_RayOfLight(), 25);
	m->addItemToStock(prayer_BlessedRadiance(), 50);
	m->addItemToStock(prayer_RemovePoison(), 25);
	//With Orsyl's Tome
	m->addStockUnlock(prayer_ProfaneRadiance(), 50, "orsyls_tome_of_prayer");
	m->addStockUnlock(prayer_DrawOutTheBlood(), 75, "orsyls_tome_of_prayer");
	//With Divine Moonspark Tome
	m->addStockUnlock(prayer_DivineRetribution(), 100, "divine_moonspark_tome");
	m->addStockUnlock(prayer_YutriasDivineSpark(), 100, "divine_moonspark_tome");
	//With Divine Tome of the Emissary
	m->addStockUnlock(prayer_DivineRestoration(), 100, "divine_tome_of_the_emissary");
	m->addStockUnlock(prayer_DivineJudgement(), 100, "divine_tome_of_the_emissary");
	//Dialogue
	m->loadDialogue("dialogue/muira_chat.txt");
	return m;
}

monsterSharedPtr npc_ElenaThePilgrim()
{
	monsterSharedPtr m(new monster("Elena the Pilgrim", PLAYER_TILE, TCODColor::lightCyan,
		"A woman in travel-worn robes, dragging a cart piled with goods."));
	m->setHealth(80);
	m->isHostile = false;
	m->isShopkeeper = true;
	//Starting stock
	m->addItemToStock(ranged_ThrowingKnives(), 50);
	m->addItemToStock(ranged_LaceratingKnives(), 75);
	m->addItemToStock(ranged_WitchsJar(), 75);
	m->addItemToStock(ranged_PyromancersFlask(), 75);
	m->addItemToStock(consumable_TinyGreenFlower(), 25);
	m->addItemToStock(consumable_InvigoratingTea(), 150);
	//With Piece of Jade
	m->addStockUnlock(consumable_TinyRedFlower(), 50, "piece_of_jade");
	m->addStockUnlock(consumable_TinyGreenFlower(), 50, "piece_of_jade");
	m->addStockUnlock(ranged_CorrodingJar(), 50, "piece_of_jade");
	m->addStockUnlock(ranged_HeavyJavelin(), 50, "piece_of_jade");
	m->addStockUnlock(ranged_LightingJavelin(), 75, "piece_of_jade");
	m->addStockUnlock(ranged_FrostKnives(), 75, "piece_of_jade");
	//Dialogue
	m->loadDialogue("dialogue/elena_chat.txt");
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
	m->loadDialogue("dialogue/ydella_chat.txt");
	//Items are all acquired via HEART-TRADING
	m->addStockUnlock(charm_WretchedFleshBand(), 200, "wretched_heart");
	m->addStockUnlock(charm_WretchedFleshmask(), 200, "wretched_heart");
	m->addStockUnlock(prayer_SinkBeneath(), 200, "fishborn_heart");
	m->addStockUnlock(weapon_OldCrowsLongKnife(), 200, "old_crows_heart");
	m->addStockUnlock(chime_OrsylsProfaneChime(), 200, "orsyls_heart");
	m->addStockUnlock(weapon_ArmOfTheDuke(), 300, "vortens_heart");
	m->addStockUnlock(weapon_SpiderboneShard(), 200, "venomous_spider_heart");
	m->addStockUnlock(prayer_SpidersPrayer(), 100, "venomous_spider_heart");
	m->addStockUnlock(weapon_LadyTvertsClaws(), 150, "lady_tverts_heart");
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
	m->loadDialogue("dialogue/ghulluk_chat.txt");
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

	//The Void
	else if (handle == "void_touched")
		return monster_VoidTouched();

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

	//LET'S HOPE WE NEVER GET HERE!
	return nullptr;
}
