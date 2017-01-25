#include "monster.h"

monster::monster() : person()
{
}

monster::monster(std::string name, int tileCode, TCODColor color) : 
	person(name, tileCode, color)
{
	//We're not the player! That's kind of our defining feature
	isPlayer = false;
	//Default to 100% spell power
	setSpellPower(100);
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
monster * monster::getRandomSpawn()
{
	int i = randrange(spawnList.size());
	std::string handle = spawnList.at(i);
	return getMonsterByHandle(handle);
}


/*
For shopkeeper NPCs.
*/
void monster::addItemToStock(item * it, int price)
{
	it->isGlittery = false;
	it->setPrice(price);
	stock.push_back(it);
}

/*
These items are unlocked if the player has a specific key.
*/
void monster::addStockUnlock(item * it, int price, std::string unlockCode)
{
	it->isGlittery = false;
	it->setPrice(price);
	stockUnlocks.push_back(new unlockableStock(it,unlockCode));
}

/*
Cause we bought something!
*/
void monster::removeItemFromStock(item * it)
{
	auto iter = std::find(stock.begin(), stock.end(), it);
	if (iter != stock.end())
		stock.erase(iter);
}


/*
See if the given person has a key that will unlock additional stock items.
*/
void monster::checkForStockUnlocks(person * unlocker)
{
	//Keep track of stuff to remove, if any
	stockUnlockVector toRemove;
	//Have a look
	for (auto unlock : stockUnlocks) {
		if (unlocker->hasKey(unlock->unlockCode)) {
			//Yup, here's one!
			stock.push_back(unlock->it);
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




monster * monster_DrownedDead()
{
	monster * m = new monster("Drowned Dead", DROWNED_DEAD_TILE, TCODColor::lightGreen);
	m->setMeleeStats(10, SPEED_SLOW);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(10);
	m->setFragmentsDropped(1);
	return m;
}

monster * monster_ThinWretch()
{
	monster * m = new monster("Thin Wretch", WRETCH_TILE, TCODColor::green);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->setMoveStats(SPEED_NORMAL);
	m->setHealth(50);
	m->setFragmentsDropped(5);
	return m;
}

monster * monster_TimidWretch()
{
	monster * m = new monster("Timid Wretch", WRETCH_TILE, TCODColor::lightestGreen);
	m->setMeleeStats(15, SPEED_FAST);
	m->setMoveStats(SPEED_SLOW);
	m->setHealth(50);
	m->addSpellKnown(ability_Charge());
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(10);
	return m;
}

monster * monster_BloatedWretch()
{
	monster * m = new monster("Bloated Wretch", BLOATED_WRETCH_TILE, TCODColor::lime);
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
monster * boss_TheWretchedMass()
{
	monster* m = new monster("The Wretched Mass", WRETCHED_MASS_TILE, TCODColor::lightGreen);
	m->setHealth(400);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(25, SPEED_SLOW);
	m->addSpawnableCreature("thin_wretch");
	m->setSpawnChance(25);
	m->addItemDrop(key_WretchsKey());
	m->isBoss = true;
	m->setFragmentsDropped(200);
	return m;
}

/*
Pilgrim's Road
*/

monster * monster_CrowKnight()
{
	monster* m = new monster("Crow Knight", CROW_KNIGHT_TILE, TCODColor::lightGrey);
	m->setHealth(100);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(20, SPEED_NORMAL);
	m->equipItem(weapon_CrowKnightSword());
	m->setFragmentsDropped(25);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	return m;
}

monster * monster_CrowThief()
{
	monster* m = new monster("Crow Thief", CROW_THIEF_TILE, TCODColor::lightGreen);
	m->setHealth(50);
	m->setMoveStats(SPEED_FAST);
	m->setMeleeStats(10, SPEED_FAST);
	m->equipItem(weapon_CrowKnife());
	m->setFragmentsDropped(25);
	return m;
}

monster * monster_CrowArcher()
{
	monster* m = new monster("Crow Archer", CROW_KNIGHT_TILE, TCODColor::lightBlue);
	m->setHealth(100);
	m->setMoveStats(SPEED_NORMAL);
	m->setMeleeStats(10, SPEED_NORMAL);
	m->addSpellKnown(ability_CrowArrow());
	m->setSpellCastChance(50);
	m->keepsDistance = true;
	m->setFragmentsDropped(25);
	return m;
}

monster * monster_ForlornCrowKnight()
{
	monster* m = new monster("Forlorn Crow Knight", CROW_KNIGHT_TILE, TCODColor::crimson);
	m->setHealth(200);
	m->setMoveStats(SPEED_NORMAL);
	m->equipItem(weapon_CrowHalfsword());
	m->addSpellKnown(attack_Quickstep());
	m->setSpellCastChance(20);
	m->setFragmentsDropped(100);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->respawns = false;
	return m;
}

monster * monster_TheOldCrow()
{
	monster* m = new monster("The Old Crow", THE_OLD_CROW_TILE, TCODColor::lightCrimson);
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
	return m;
}

//Flooded Lowlands

monster * monster_StarvingFishman()
{
	monster* m = new monster("Starving Fishman", FISHMAN_TILE, TCODColor::lightGreen);
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

monster * monster_FishmanDoomPreacher()
{
	monster* m = new monster("Fishman Doom-Preacher", FISHMAN_TILE, TCODColor::lime);
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

monster * monster_FishmanSpearfisher()
{
	monster* m = new monster("Fishman Spearfisher", FISHMAN_TILE, TCODColor::lightLime);
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

monster * monster_GuardianGolem()
{
	monster* m = new monster("Guardian Golem", GOREM_TILE, TCODColor::lightLime);
	m->setHealth(300);
	m->setDefence(DAMAGE_PHYSICAL, 10);
	m->setBleedResist(1000);
	m->setMoveStats(SPEED_SLOW);
	m->setMeleeStats(100, SPEED_SLOW);
	m->setFragmentsDropped(200);
	m->addItemDrop(key_RuinedTownshipKey());
	return m;
}


/*
	FRIENDLIES
*/


monster * npc_Gorem()
{
	monster* m = new monster("Gorem, Ancient of Wyrd", GOREM_TILE, TCODColor::lighterGrey);
	m->setHealth(800);
	m->isShopkeeper = true;
	m->isHostile = false;
	m->addItemToStock(shield_WoodenWyrdShield(), 25);
	m->addItemToStock(chime_WyrdBellbranch(), 50);
	m->addItemToStock(prayer_WyrdChantOfStrength(), 25);
	m->addItemToStock(charm_BloodstainedCharm(), 100);
	//Chatting
	m->loadDialogue("dialogue/gorem_chat.txt");
	return m;
}

monster * npc_UtricTheRat()
{
	monster* m = new monster("Utric the Rat", GOREM_TILE, TCODColor::lighterSepia);
	m->setHealth(100);
	m->isHostile = false;
	m->isShopkeeper = true;
	//Starting stock
	m->addItemToStock(spell_ArcaneBlade(), 50);
	m->addItemToStock(spell_GottricsArcaneProtection(), 40);
	m->addItemToStock(spell_Frostbolt(), 25);
	//Unlock: Waterlogged Writings
	m->addStockUnlock(spell_AcidSpit(), 50, "waterlogged_writings");
	m->addStockUnlock(spell_AcidBurst(), 75, "waterlogged_writings");
	m->addStockUnlock(spell_AcidBlade(), 100, "waterlogged_writings");
	//Dialogue
	m->loadDialogue("dialogue/utric_chat.txt");
	return m;
}

/*
This giant nightmare is how monsters are defined in map files.
*/
monster * getMonsterByHandle(std::string handle)
{

	//Stardrift Wreckage
	if (handle == "drowned_dead")
		return monster_DrownedDead();

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

	//Friendly NPCs
	else if (handle == "gorem")
		return npc_Gorem();
	else if (handle == "utric")
		return npc_UtricTheRat();

	//LET'S HOPE WE NEVER GET HERE!
	return nullptr;
}
