
/*
This class manages the whole game.
It takes care of input handling, time passing, and calling everything else that needs to happen.
It also draws all the stuff that needs drawing.
*/


#ifndef GAME_H
#define GAME_H

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


//Messages!
struct message {
	message(std::string txt, TCODColor color) :
		txt(txt), color(color) {}
	std::string txt;
	TCODColor color;
};

//Save point data - a coordinate and a map pointer
struct savePoint {
	map* saveMap;
	coord savePt;
};

typedef std::vector<message> messageVector;


class game
{
public:
	
	game();
	~game();
	
	//Map stuff
	void setCurrentMap(map* newMapP);
	
	//Game running
	void mainGameLoop();

private:
	
	window win;

	//Character/state/etc
	map* currentMap;
	person* player;
	gameState state = STATE_VIEW_MAP;
	bool isGameOver = false;

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

	//Targeting
	bool targetModeOn = false;
	coord targetPt;
	void toggleTargetMode();
	void setCursorPosition(int xnew, int ynew);
	void setCursorPosition(coord xy);

	//Pathfinding
	TCODPath* getPathToCoord(coord startxy, coord endxy);
	pathVector getLine(coord startxy, coord endxy);
	person* getTargetOnPath(pathVector path);

	//Turn tracking
	turnTracker turns;
	int playerTurnDelay = 0; //When it increases, that means the player's turn is over!
	void endPlayerTurn();

	//Monster actions
	pathVector getAllAdjacentWalkable(monster* ai);
	bool aiIsValidMove(monster* ai, int xnew, int ynew);
	bool aiMoveToTarget(monster* ai);
	bool aiTryUseSpell(monster* ai);
	void aiSpawnCreature(monster* ai);
	void aiDoCombatAction(monster* ai);
	void aiFindTarget(monster* ai);
	void doMonsterTurn(person* ai);

	//Menu management/drawing
	menu* currentMenu;
	void drawMenu(menu* m, int atx, int aty);
	void navigateMenu(TCOD_key_t kp);
	void acceptCurrentMenuIndex();
	void menuBackOut();

	//Drawing functions
	void drawScreen();
	void drawMap(int atx, int aty);
	drawData getDrawData(int x, int y);
	void drawInterface(int atx, int aty);
	void drawInventory(int atx, int aty);

	//Animations
	animVector playingAnimations;
	void addAnimations(animations* a) { playingAnimations.push_back(a); }
	drawData getAnimationDataOverride(drawData* baseData, int x, int y);
	void updateAnimations();

	//Drawing item information
	void drawItemInfo(item* it, int atx, int aty);
	void drawWeaponInfo(weapon* it, int atx, int aty);
	void drawArmourInfo(armour* it, int atx, int aty);

	//Input processing
	void processCommand();
	void useConsumable();

	//Effects
	void doAOE(spell* sp, person* caster);
	void applyEffectToPerson(person* target, effect e, int potency);

	//Movement
	void processMove(TCOD_key_t kp);
	bool isMovementKey(TCOD_key_t kp);
	void playerMoveLogic(int xnew, int ynew);
	void movePerson(person* p, int xnew, int ynew);
	void standOnTile(person* victim);
	void unlockAdjacentTiles(int x, int y);
	void tryUnlockDoor(int x, int y);

	//Inter-map movement
	mapLoader makemap;
	void tryMapChange(int xnew, int ynew);
	void loadMapFromHandle(std::string handle, connectionPoint connect, int oldx, int oldy);
	void loadNewMap(map* newMap, connectionPoint connect, int oldx, int oldy);

	//Combat
	void meleeAttack(person* attacker, person* target);

	//Boss fights
	monster* currentBoss;
	void setBoss(monster* m);
	void bossKillMessage();

	//Inventory management
	void itemPickupMessage(item* it);
	void pickUpItem(item* it);
	void createInventoryMenu();
	void selectInventoryCategory(itemTypes cat);
	void getDeathDrops(monster* m);

	//Spellcasting
	void castSpell(spell* sp);
	void dischargeSpellOnTarget(spell* sp, person* caster, person* target);

	//Keeping the world up to date
	void tick();
	void clearDeadCreatures();

	//Save points
	savePoint ourSavePt;
	void setSavePoint();
	void restoreFromSavePoint();

	//Debugging options
	void debugMenu();
	
	//Draw location constants
	const static int MAP_DRAW_X = 3;
	const static int MAP_DRAW_Y = 3;


};

#endif
