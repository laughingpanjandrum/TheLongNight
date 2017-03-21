
/*
This class manages the whole game.
It takes care of input handling, time passing, and calling everything else that needs to happen.
It also draws all the stuff that needs drawing.
*/


/*
TODO
	Fix items respawning on maps with warp destinations when loading saves
*/


#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <memory>

#include "libtcod.hpp"
#include "window.h"
#include "keycodes.h"
#include "turnTracker.h"
#include "utility.h"
#include "menu.h"
#include "animations.h"

#include "mapLoader.h"
#include "map.h"
#include "person.h"
#include "monster.h"

#include "savegame.h"
#include "worldmap.h"

const std::string PROFILES_FILE = "_profiles.txt";


//Messages!
struct message {
	message(std::string txt, TCODColor color) :
		txt(txt), color(color) {}
	std::string txt;
	TCODColor color;
};


//A little chunk of flavour text to display when an area is first loaded.
struct areaText {
	areaText() {}
	areaText(std::string mapTag, std::string title, std::string text) :
		mapTag(mapTag), title(title), text(text) {}
	std::string title; //Title displayed
	std::string text; //Text displayed
	std::string mapTag; //Name of map we are tied to
};


typedef std::vector<message> messageVector;
typedef std::vector<std::string> stringVector;
typedef std::vector<areaText*> areaTextVector;

typedef std::shared_ptr<TCODPath> pathSharedPtr;



class game
{
public:
	
	game() : game("unnamed") {}
	game(std::string saveFileName, bool isNewGame);
	game(std::string fname) : SAVE_FILE_NAME(fname) {
		loadSaveGame(fname);
	} //Constructs a game using a save file
	~game();
	
	//Map stuff
	void setCurrentMap(map* newMapP);
	
	//Game running
	void mainGameLoop();

private:
	
	window win;

	//Name of the save file we use.
	const std::string SAVE_FILE_NAME;

	//Constants
	const static std::string VOID_EDGE_MAP;
	const static std::string VOID_RETURN_MAP;
	const static std::string BLACKWATCH_FORTRESS;

	//Character/state/etc
	map* currentMap;
	personSharedPtr player;
	gameState state = STATE_VIEW_MAP;
	bool isGameOver = false;
	bool canAccessInventory = false;

	//State manipulation
	void setState(gameState st);

	//Map management
	mapVector allMaps;
	std::vector<std::string> allMapHandles;
	void addKnownMap(map* m, std::string handle);
	map* getKnownMap(std::string handle);

	//Messages
	messageVector messages;
	void addMessage(std::string txt, TCODColor color);

	//Worldmap
	worldmap ourWorldMap;
	void drawWorldMap();
	void setupWorldMap();

	//Targeting
	bool targetModeOn = false;
	coord targetPt;
	void toggleTargetMode();
	void setCursorPosition(int xnew, int ynew);
	void setCursorPosition(coord xy);

	//Pathfinding
	pathSharedPtr getPathToCoord(coord startxy, coord endxy);
	coordVectorSharedPtr getLine(coord startxy, coord endxy);
	personSharedPtr getTargetOnPath(coordVectorSharedPtr path);

	//Turn tracking
	turnTracker turns;
	int playerTurnDelay = 0; //When it increases, that means the player's turn is over!
	void endPlayerTurn();

	//Monster actions
	pathVector getAllAdjacentWalkable(monsterSharedPtr ai);
	bool aiIsValidMove(monsterSharedPtr ai, int xnew, int ynew);
	bool aiMoveToTarget(monsterSharedPtr ai);
	bool aiShouldCastSpell(monsterSharedPtr ai, spellSharedPtr sp);
	bool aiTryUseSpell(monsterSharedPtr ai);
	void aiSpawnCreature(monsterSharedPtr ai);
	void aiDoCombatAction(monsterSharedPtr ai);
	void aiFindTarget(monsterSharedPtr ai);
	void doMonsterTurn(personSharedPtr ai);

	//Menu management/drawing
	menu* currentMenu;
	void drawMenu(menu* m, int atx, int aty);
	void navigateMenu(TCOD_key_t kp);
	void acceptCurrentMenuIndex();
	void menuBackOut();

	//General drawing functions
	void drawScreen(bool doRefresh = true);
	void drawMap(int atx, int aty);
	float getLightEmitters(int x, int y);
	bool canPlayerSeePoint(int x, int y);
	drawData getDrawData(int x, int y);
	void drawInterface(int atx, int aty);
	void drawInventory(int atx, int aty);
	void drawPlayerInfo(int atx, int aty);
	void drawMonsterInfo(monsterSharedPtr m, int atx, int aty);
	void drawMouseover(int atx, int aty);
	void drawTargetInfo(personSharedPtr target, int atx, int aty);
	int listStatusEffects(personSharedPtr target, int atx, int aty);

	//Animations
	animVector playingAnimations;
	void addAnimations(animations* a) { playingAnimations.push_back(a); }
	drawDataSharedPtr getAnimationDataOverride(drawDataSharedPtr baseData, int x, int y);
	void updateAnimations();
	void spellTitleAnimation(personSharedPtr caster, spellSharedPtr sp);

	//Drawing item information
	void drawItemInfo(itemSharedPtr it, int atx, int aty, bool drawImage = true);
	void drawWeaponInfo(weaponSharedPtr it, int atx, int aty);
	void drawArmourInfo(armourSharedPtr it, int atx, int aty);
	void drawSpellInfo(spellSharedPtr it, int atx, int aty);
	void drawConsumableInfo(consumableSharedPtr it, int atx, int aty);
	void drawCharmInfo(charmSharedPtr it, int atx, int aty);
	void drawMiscItemInfo(miscItemSharedPtr it, int atx, int aty);
	void drawItemImage(itemSharedPtr it, int atx, int aty, bool drawOtherArmourPiece = true);

	//Input processing
	void processCommand();
	void processMouseClick();
	bool isMouseOver(int x, int xwidth, int y);

	//Auto-walking for the PC
	bool isAutoWalking = false;
	void startAutoWalk();
	void playerAutoWalk();
	pathSharedPtr playerAutoPath;

	//Consumables
	void useConsumable();
	void openConsumableMenu();
	void selectConsumableFromMenu();

	//Effects
	void doAOE(spellSharedPtr sp, personSharedPtr caster);
	void applyEffectToPerson(personSharedPtr target, effect e, int potency, personSharedPtr caster = nullptr);

	//Movement
	void processMove(TCOD_key_t kp);
	bool isMovementKey(TCOD_key_t kp);
	bool isNumberKey(TCOD_key_t kp);
	int getNumberByKeycode(TCOD_key_t kp);
	void playerMoveLogic(int xnew, int ynew);
	void movePerson(personSharedPtr p, int xnew, int ynew);
	void standOnTile(personSharedPtr victim);
	void unlockAdjacentTiles(int x, int y);
	void tryUnlockDoor(int x, int y);

	//Inter-map movement
	mapLoader makemap;
	savePointVector warpPoints;
	void takeStairs();
	void tryMapChange(int xnew, int ynew);
	void loadMapFromHandle(std::string handle, connectionPoint connect, int oldx, int oldy);
	void loadNewMap(map* newMap, connectionPoint connect, int oldx, int oldy);
	void setupWarpMenu();
	void addWarpPoint(savePoint pt);
	void doWarp(std::string warpPointName);

	//Combat
	void meleeAttack(personSharedPtr attacker, personSharedPtr target);

	//Special effects
	void knockbackTarget(personSharedPtr knocker, personSharedPtr target, int distance);
	void pullTarget(personSharedPtr puller, personSharedPtr target, int distance);
	bool waterWarp(personSharedPtr target, int distance);
	void teleport(personSharedPtr target, int distance);

	//Player-only SPECIAL MAP JUMPS
	void teleportToVoid();
	void teleportOutOfVoid();
	void operateMoonGate();
	void makeRitualOffering();

	//Boss fights
	monsterSharedPtr currentBoss;
	void setBoss(monsterSharedPtr m);
	void bossKillMessage();

	//Inventory management
	bool itemPickupMessage(itemSharedPtr it);
	void pickUpItem(itemSharedPtr it);
	void createInventoryMenu();
	void selectInventoryCategory(itemTypes cat);
	void getDeathDrops(monsterSharedPtr m);
	void equipItem(itemSharedPtr it);

	//Spellcasting
	void useAbilityByHotkey(TCOD_key_t kp);
	void doRangedSpell(spellSharedPtr sp);
	void castSpell(spellSharedPtr sp);
	void dischargeSpellOnTarget(spellSharedPtr sp, personSharedPtr caster, personSharedPtr target);
	void dischargeSpellOnWeapon(spellSharedPtr sp, personSharedPtr caster, weaponSharedPtr target);
	void openSpellMenu();
	void selectSpellFromMenu();

	// $$ money money $$ and leveling up
	int fragments = 0;
	void setupLevelUpMenu();
	void doLevelUp();
	void drawLevelUpMenu(int atx, int aty);

	//Shopping/chatting
	void initializeShops();
	shopSharedPtr getShopByTag(std::string tag);
	shopVector allShops;
	shopVector allUnlockableShops;
	monsterSharedPtr currentShopkeeper;
	void talkToNPC();
	void drawDialogueLine(personSharedPtr target, std::string line);
	void doDialogue(monsterSharedPtr target);
	bool checkForDialogueEvent(std::string line, monsterSharedPtr target);
	void drawShopMenu(int atx, int aty);
	void checkForStockUnlocks(monsterSharedPtr shopkeeper);
	void setupShopMenu(personSharedPtr shopkeeper);
	void buyItemFromShop();
	void destroyShopItemsWithKey(shopSharedPtr currentShop, std::string keyTag);

	//Story event tracking!
	std::vector<std::string> storyFlags;
	void addStoryFlag(std::string f);
	bool hasStoryFlag(std::string f);
	void loadStoryEvents(std::string filename);
	storyEventVector storyEventsReady;
	storyEventVector storyEventsWaiting;
	void saveStoryEvent(storyEvent e) { storyEventsWaiting.push_back(e); }
	void queueStoryEvent(storyEvent e) { storyEventsReady.push_back(e); }

	//Text dumps displayed when certain maps load.
	void loadTextDumps(std::string filename);
	areaTextVector areaTextList;
	void checkForAreaText(std::string mapTag);
	areaText* findAreaText(std::string mapTag);

	//Keeping the world up to date
	void tick();
	void clearDeadCreatures();

	//Save points
	savePoint ourSavePt;
	void setSavePoint();
	void restoreFromSavePoint(savePoint* warpTo = nullptr);
	void deletePlayerBuffs();

	//Coordinate conversions
	coord screenToMapCoords(coord pt);
	TCOD_mouse_t mouse;
	TCOD_key_t key;

	//Debugging options
	void debugMenu();
	
	//Draw location constants
	const static int MAP_DRAW_X = 3;
	const static int MAP_DRAW_Y = 3;
	const static int ITEM_DRAW_X = 3;
	const static int ITEM_DRAW_Y = 50;

	//Special story progression
	int gotSirPercivelsSet = 0;
	int offeringsMade = 0;

	//Image used to represent the PC when armour is being drawn
	imageSharedPtr PLAYER_IMAGE = imageSharedPtr(loadImage("character"));

	//Saving, loading!
	void saveGame();
	void loadSaveGame(std::string fname);

	void setupGeneralGame();


};

#endif
