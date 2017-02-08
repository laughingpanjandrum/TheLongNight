
#ifndef MONSTER_H
#define MONSTER_H

#include <iostream>
#include <fstream>
#include <memory>

#include "person.h"
#include "utility.h"


class monster: public person
{
public:

	typedef std::shared_ptr<monster> monsterSharedPtr;

	//Constructors
	monster();
	monster(std::string name, int tileCode, TCODColor color, std::string description = "");
	monster(const monster& rhs) {} //Copy constructor
	~monster() {}

	//Setting
	void setMeleeStats(int damage, int speed);
	void setMoveStats(int speed);
	void setDefence(damageType dtype, int defence);
	void setBleedResist(int br) { bleedBuildup.setMaxValue(br); }
	void setPoisonResist(int pois) { poisonBuildup.setMaxValue(pois); }
	void setHealth(int hp);
	void setSpellPower(int power) { baseSpellPower = power; }
	void setDivinePower(int power) { baseDivinePower = power; }
	void makeProfane() { profane = true; }

	//Setting/getting behaviours
	bool keepsDistance = false;
	void setSpellCastChance(int c) { spellCastChance = c; }
	int getSpellCastChance() { return spellCastChance; }

	//Spawning stuff
	bool canSpawnCreatures() { return spawnList.size() > 0; }
	bool wantsToSpawn();
	monsterSharedPtr getRandomSpawn();
	void addSpawnableCreature(std::string handle) { spawnList.push_back(handle); }
	void setSpawnChance(int c) { spawnChance = c; }

	//Item drops when we DIE
	itemVector getItemDrops() { return drops; }
	void addItemDrop(itemSharedPtr it) { drops.push_back(it); }
	int getFragmentsDropped() { return dropsFragments; }
	void setFragmentsDropped(int f) { dropsFragments = f; }

	//Shopkeeping
	void addItemToStock(itemSharedPtr it, int price);
	void addStockUnlock(itemSharedPtr it, int price, std::string unlockCode);
	void removeItemFromStock(itemSharedPtr it);
	itemVector getStock() { return stock; }
	void checkForStockUnlocks(personSharedPtr unlocker);

	//Chatting with ur buddies
	void loadDialogue(std::string filename);
	void addDialogue(std::string line) { dialogue.push_back(line); }
	std::string getNextDialogueLine();
	std::string getCurrentDialogueLine() { return dialogue.at(atChatLine); }
	void backUpDialogue() { atChatLine--; }
	bool hasDialogue() { return dialogue.size() > 0; }

	//Fog
	int emitsFog = 0;

	//Flags
	bool isBoss = false;
	bool showBossHealthBar = true; //only if we're actually a boss, of course!

protected:

	//Behaviours
	int spellCastChance = 0;

	//Spawning powers
	std::vector<std::string> spawnList;
	int spawnChance = 0;

	//Item drops
	itemVector drops;
	int dropsFragments = 0;

	//Shopkeeping
	itemVector stock;
	stockUnlockVector stockUnlocks;

	//Chatting
	std::vector<std::string> dialogue;
	int atChatLine = 0;

};


typedef std::shared_ptr<monster> monsterSharedPtr;
typedef std::vector<monsterSharedPtr> monsterVector;



//		Monster definitions



//Stardrift Wreckage
monsterSharedPtr monster_DrownedCrew();

//Coruscating Beach
monsterSharedPtr monster_ThinWretch();
monsterSharedPtr monster_TimidWretch();
monsterSharedPtr monster_BloatedWretch();
monsterSharedPtr boss_TheWretchedMass();

//Pilgrim's Road
monsterSharedPtr monster_CrowKnight();
monsterSharedPtr monster_CrowThief();
monsterSharedPtr monster_CrowArcher();
monsterSharedPtr monster_ForlornCrowKnight();
monsterSharedPtr monster_TheOldCrow();

//The Flooded Lowlands
monsterSharedPtr monster_StarvingFishman();
monsterSharedPtr monster_FishmanDoomPreacher();
monsterSharedPtr monster_FishmanSpearfisher();
monsterSharedPtr monster_GuardianGolem();
monsterSharedPtr monster_DegenerateFishman();
monsterSharedPtr monster_SkinlessKnight();
monsterSharedPtr monster_FishbornGoddess();

//Darkwater Forest
monsterSharedPtr monster_GiantRat();
monsterSharedPtr monster_ProfaneRatPriest();

//Crumbling City
monsterSharedPtr monster_CrawlingZombie();
monsterSharedPtr monster_FirespitterZombie();
monsterSharedPtr monster_OozingZombie();
monsterSharedPtr monster_DevoteeOfPash();
monsterSharedPtr monster_VoidwalkerMinas();
monsterSharedPtr monster_BloodthirstyHound();
monsterSharedPtr monster_BloodstarvedZombie();
monsterSharedPtr monster_HighClericOrsylTheProfaned();
monsterSharedPtr monster_CursedKnight();
monsterSharedPtr monster_CursedArcher();
monsterSharedPtr monster_Enkidu();
monsterSharedPtr monster_DukeVortenTheWatchful();

//The Winter Court
monsterSharedPtr monster_WinterShade();
monsterSharedPtr monster_HollowfacedKnight();

//The Murdermire
monsterSharedPtr monster_Wisp();
monsterSharedPtr monster_DrownedDead();
monsterSharedPtr monster_SwampSpider();
monsterSharedPtr monster_AdherentOfGlug();
monsterSharedPtr monster_GhorthTheBloatedSpider();

//Tear-Stained Valley
monsterSharedPtr monster_UndeadPrisoner();
monsterSharedPtr monster_BloatedSlaveDriver();
monsterSharedPtr monster_SlaveringHound();
monsterSharedPtr monster_Emissary();
monsterSharedPtr monster_BartonTheProwlingMinister();

//Desolate Village
monsterSharedPtr monster_SlaveMasterGasgut();
monsterSharedPtr monster_WhiteFogHerald();

//The Void
monsterSharedPtr monster_VoidTouched();

//Friendly NPCs
monsterSharedPtr npc_Gorem();
monsterSharedPtr npc_UtricTheRat();
monsterSharedPtr npc_MuiraClericOfTheRose();
monsterSharedPtr npc_ElenaThePilgrim();
monsterSharedPtr npc_VoidwalkerDaedelus();
monsterSharedPtr npc_Ydella();
monsterSharedPtr npc_TheEyelessQueen();
monsterSharedPtr npc_ToadPriestGhulluk();


//For map files
monsterSharedPtr getMonsterByHandle(std::string handle);

#endif
