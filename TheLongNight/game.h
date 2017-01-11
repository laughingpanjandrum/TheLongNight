
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

	//Turn tracking
	turnTracker turns;
	int playerTurnDelay = 0; //When it increases, that means the player's turn is over!
	void endPlayerTurn();

	//Monster actions
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

	//Spellcasting
	void castSpell(spell* sp);

	//Keeping the world up to date
	void tick();
	void clearDeadCreatures();
	
	//Draw location constants
	const static int MAP_DRAW_X = 3;
	const static int MAP_DRAW_Y = 3;
};

#endif
