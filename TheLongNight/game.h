
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

#include "map.h"
#include "person.h"
#include "monster.h"


//Draw data - useful little chunk of data that defines what to draw at a point on the map
struct drawData {
	drawData() {}
	drawData(int tileCode, TCODColor color, TCODColor bgcolor): 
		tileCode(tileCode), color(color), bgcolor(bgcolor) {}
	int tileCode;
	TCODColor color;
	TCODColor bgcolor;
};

//Messages!
struct message {
	message(std::string txt, TCODColor color) :
		txt(txt), color(color) {}
	std::string txt;
	TCODColor color;
};

typedef std::vector<message> messageVector;
typedef std::vector<coord> pathVector;


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
	pathVector getAllAdjacentWalkable(person* ai);
	bool aiIsValidMove(person* ai, int xnew, int ynew);
	void aiMoveToTarget(person* ai);
	void aiFindTarget(person* ai);
	void doMonsterTurn(person* ai);

	//Drawing functions
	void drawScreen();
	void drawMap(int atx, int aty);
	drawData getDrawData(int x, int y);
	void drawInterface(int atx, int aty);
	void drawInventory(int atx, int aty);

	//Input processing
	void processCommand();
	void useConsumable();

	//Effects
	void applyEffectToPerson(person* target, effect e, int potency);

	//Movement
	void processMove(TCOD_key_t kp);
	bool isMovementKey(TCOD_key_t kp);
	void movePerson(person* p, int xnew, int ynew);
	void standOnTile(person* victim);

	//Combat
	void meleeAttack(person* attacker, person* target);

	//Inventory management
	void createInventoryMenu();

	//Spellcasting
	void castSpell(spell* sp);
	void dischargeSpellOnTarget(spell* sp, person* caster, person* target);

	//Keeping the world up to date
	void tick();
	void clearDeadCreatures();
	
	//Draw location constants
	const static int MAP_DRAW_X = 3;
	const static int MAP_DRAW_Y = 3;
};

#endif
