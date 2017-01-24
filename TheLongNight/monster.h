
#ifndef MONSTER_H
#define MONSTER_H

#include <iostream>
#include <fstream>
#include "person.h"
#include "utility.h"

class monster: public person
{
public:

	//Constructors
	monster();
	monster(std::string name, int tileCode, TCODColor color);
	monster(const monster& rhs) {} //Copy constructor
	~monster() {}

	//Copying
	monster* clone() const { return new monster(*this); }

	//Setting
	void setMeleeStats(int damage, int speed);
	void setMoveStats(int speed);
	void setDefence(damageType dtype, int defence);
	void setBleedResist(int br) { bleedBuildup.setMaxValue(br); }
	void setHealth(int hp);
	void setSpellPower(int power) { baseSpellPower = power; }

	//Setting/getting behaviours
	bool keepsDistance = false;
	void setSpellCastChance(int c) { spellCastChance = c; }
	int getSpellCastChance() { return spellCastChance; }

	//Spawning stuff
	bool canSpawnCreatures() { return spawnList.size() > 0; }
	bool wantsToSpawn();
	monster * getRandomSpawn();
	void addSpawnableCreature(std::string handle) { spawnList.push_back(handle); }
	void setSpawnChance(int c) { spawnChance = c; }

	//Item drops when we DIE
	itemVector getItemDrops() { return drops; }
	void addItemDrop(item* it) { drops.push_back(it); }
	int getFragmentsDropped() { return dropsFragments; }
	void setFragmentsDropped(int f) { dropsFragments = f; }

	//Shopkeeping
	void addItemToStock(item* it, int price);
	void removeItemFromStock(item* it);
	itemVector getStock() { return stock; }

	//Chatting with ur buddies
	void loadDialogue(std::string filename);
	void addDialogue(std::string line) { dialogue.push_back(line); }
	std::string getNextDialogueLine();
	std::string getCurrentDialogueLine() { return dialogue.at(atChatLine); }
	void backUpDialogue() { atChatLine--; }
	bool hasDialogue() { return dialogue.size() > 0; }

	//Flags
	bool isBoss = false;
	bool respawns = true;

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

	//Chatting
	std::vector<std::string> dialogue;
	int atChatLine = 0;

};

typedef std::vector<monster*> monsterVector;



//		Monster definitions



//Stardrift Wreckage
monster* monster_DrownedDead();

//Coruscating Beach
monster* monster_ThinWretch();
monster* monster_TimidWretch();
monster* monster_BloatedWretch();
monster* boss_TheWretchedMass();

//Pilgrim's Road
monster* monster_CrowKnight();
monster* monster_CrowThief();
monster* monster_CrowArcher();
monster* monster_ForlornCrowKnight();
monster* monster_TheOldCrow();

//The Flooded Lowlands
monster* monster_StarvingFishman();
monster* monster_FishmanDoomPreacher();
monster* monster_FishmanSpearfisher();
monster* monster_GuardianGolem();

//Friendly NPCs
monster* npc_Gorem();
monster* npc_UtricTheRat();


//For map files
monster* getMonsterByHandle(std::string handle);

#endif
