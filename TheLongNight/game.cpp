#include "game.h"



game::game()
{
	//We start by creating an empty map, just for now
	map* newmap = new map();
	setCurrentMap(newmap);
	//Add some random walls and shit
	currentMap->setTile(wall(), 5, 5);
	currentMap->setTile(wall(), 6, 5);
	currentMap->setTile(wall(), 8, 8);
	currentMap->setTile(wall(), 10, 8);
	currentMap->setTile(acid(), 13, 13);
	currentMap->setTile(acid(), 13, 14);
	currentMap->setTile(acid(), 14, 13);
	//Character create
	player = new person();
	player->equipItem(weapon_SplinteredSword());
	player->isPlayer = true;
	currentMap->addPerson(player, 3, 3);
	currentMap->updateFOV(player->getx(), player->gety());
	//Monsters
	monster* m = drownedDead();
	currentMap->addPerson(m, 12, 12);
	//Add everything to the turn timer
	//turns.addEntity(player, 0);
	turns.addEntity(m, 1);
}


game::~game()
{
}

/*
	MAIN GAME LOOP
*/


void game::mainGameLoop()
{
	//Draw
	while (!isGameOver) {
		drawScreen();
		//Has the player's turn ended?
		if (playerTurnDelay > 0) {
			endPlayerTurn();
		}
		//Process player action
		processCommand();
	}
}

/*
	MAP STUFF
*/

/*
Sets the current map
*/
void game::setCurrentMap(map * newMapP)
{
	currentMap = newMapP;
}

/*
	PASSAGE OF TIME
*/


void game::endPlayerTurn()
{
	turns.addEntity(player, playerTurnDelay);
	playerTurnDelay = 0;
	person* nextTurn;
	//Cycle through all turns
	do {
		nextTurn = turns.getNext();
		if (nextTurn != player) {
			doMonsterTurn(nextTurn);
		}
	} while (nextTurn != player);
}


/*
	AI ACTIONS
*/

/*
Returns whether the given ai will move to the given point.
*/
bool game::aiIsValidMove(person * ai, int xnew, int ynew)
{
	if (!currentMap->inBounds(xnew, ynew))
		return false;
	else if (!currentMap->isWalkable(xnew, ynew))
		return false;
	return true;
}

/*
AI moves directly towards its target, melee-attacking if possible.
*/
void game::aiMoveToTarget(person * ai)
{
	int xnew = ai->getx();
	int ynew = ai->gety();
	person* target = ai->getTarget();
	//Movement vectors
	xnew += get1dVector(xnew, target->getx());
	ynew += get1dVector(ynew, target->gety());
	//Try to move it
	if (aiIsValidMove(ai, xnew, ynew)) {
		movePerson(ai, xnew, ynew);
	}
	//Time passes
	turns.addEntity(ai, ai->getMoveDelay());
}

/*
AI tries to find something to kill.
*/
void game::aiFindTarget(person * ai)
{
	//If the player can see us, we can see them. ONE SIMPLE RULE.
	if (currentMap->isPointInFOV(ai->getx(), ai->gety())) {
		ai->setTarget(player);
	}
}

/*
Monster does its turn and then is placed back into the turn tracker.
*/
void game::doMonsterTurn(person * ai)
{
	//If we're dead, we probably shouldn't do anything
	if (ai->isDead)
		return;
	//Do we have a target?
	person* target = ai->getTarget();
	if (target == nullptr)
		aiFindTarget(ai);
	//If we have one, just move towards it
	if (target != nullptr)
		aiMoveToTarget(ai);
	else
		//If not, just wait
		turns.addEntity(ai, 1);
}

/*
	DRAWING STUFF
*/

/*
Clears, draws the entire screen, then refreshes.
*/
void game::drawScreen() 
{
	win.clear();
	drawMap(MAP_DRAW_X, MAP_DRAW_Y);
	drawInterface(MAP_DRAW_X + 40, MAP_DRAW_Y);
	//win.drawFont();
	win.refresh();
}

/*
Just draws the current map.
Input: Coordinates to start drawing at (the top left corner of the map)
*/
void game::drawMap(int leftx, int topy)
{
	//Draw entire map, left to right & top to bottom
	for (int x = 0; x < currentMap->getXSize(); x++) {
		for (int y = 0; y < currentMap->getYSize(); y++) {
			drawData toDraw = getDrawData(x, y);
			win.writec(leftx + x, topy + y, toDraw.tileCode, toDraw.color, toDraw.bgcolor);
		}
	}
}

/*
Returns what to draw at the given point.
*/
drawData game::getDrawData(int x, int y)
{
	//Basic data comes from the map tile
	maptile* m = currentMap->getTile(x, y);
	drawData toDraw(m->getTileCode(), m->getColor(), m->getBgColor());
	//If this point isn't visible, don't draw it!
	if (!currentMap->isPointInFOV(x, y)) {
		//Darken color if out of FOV
		toDraw.color = win.mixColors(toDraw.color, TCODColor::black, 0.9);
		toDraw.bgcolor = win.mixColors(toDraw.bgcolor, TCODColor::black, 0.9);
	}
	else {
		//Is there a player here?
		person* p = currentMap->getPerson(x, y);
		if (p != nullptr) {
			toDraw.tileCode = p->getTileCode();
			toDraw.color = p->getColor();
		}
		//Darken tiles that are further away
		int distance = hypot(x - player->getx(), y - player->gety());
		float modifier = distance * 0.07;
		if (modifier > 0.85)
			modifier = 0.85;
		else if (modifier < 0.1)
			modifier = 0.1;
		toDraw.color = win.mixColors(toDraw.color, TCODColor::black, modifier);
		toDraw.bgcolor = win.mixColors(toDraw.bgcolor, TCODColor::black, modifier);
	}
	//Done! Return it all
	return toDraw;
}

/*
Draws everything that's not the map!
*/
void game::drawInterface(int leftx, int topy)
{
	int atx = leftx;
	int aty = topy;
	//Health
	win.drawCounter(player->getHealth(), "LIFE", atx, aty, TCODColor::darkRed, TCODColor::darkGrey, 20);
	win.write(atx + 4, ++aty, player->getHealth().getAsString(), TCODColor::darkRed);
	//Equipment
	weapon* wp = player->getWeapon();
	if (wp != nullptr) {
		win.writec(atx, ++aty, wp->getTileCode(), wp->getColor());
		win.write(atx + 2, aty, wp->getName(), wp->getColor());
	}
	//Target info
	person* target = player->getTarget();
	if (target != nullptr) {
		aty++;
		win.write(atx, ++aty, target->getName(), target->getColor());
		win.write(atx, ++aty, "HP:" + target->getHealth().getAsString(), TCODColor::darkRed);
	}
}

/*
	COMMAND PROCESSING
*/

void game::processCommand()
{
	TCOD_key_t kp = win.getkey();
	if (kp.c == 'Q')
		isGameOver = true;
	//Movement
	else if (isMovementKey(kp))
		processMove(kp);
	//Debug
	else if (kp.c == 'w')
		player->takeDamage(5);
}

/*
Movement commands
*/
void game::processMove(TCOD_key_t kp)
{
	//Determine new position
	int xnew = player->getx();
	int ynew = player->gety();
	if (kp.vk == KEY_NORTH)
		ynew--;
	else if (kp.vk == KEY_SOUTH)
		ynew++;
	else if (kp.vk == KEY_EAST)
		xnew++;
	else if (kp.vk == KEY_WEST)
		xnew--;
	//And then move there!
	movePerson(player, xnew, ynew);
}

/*
Returns whether the given key is a movement command
*/
bool game::isMovementKey(TCOD_key_t kp)
{
	return kp.vk == KEY_NORTH || kp.vk == KEY_EAST || kp.vk == KEY_SOUTH || kp.vk == KEY_WEST;
}

/*
Change player character's position, if the move is valid.
*/
void game::movePerson(person* p, int xnew, int ynew)
{
	//In bounds?
	if (currentMap->inBounds(xnew, ynew)) {
		//Possible to walk on?
		if (currentMap->isWalkable(xnew, ynew)) {
			//Is someone already here?
			person* here = currentMap->getPerson(xnew, ynew);
			if (here != nullptr) {
				//We attack
				meleeAttack(p, here);
				//Player delay, if this is the player
				if (p->isPlayer)
					playerTurnDelay = p->getAttackDelay();
			}
			else {
				//Adjust position and deal with the consequences
				p->setPosition(xnew, ynew);
				standOnTile(p);
				//If this is the player, update the FOV
				if (p->isPlayer) {
					currentMap->updateFOV(player->getx(), player->gety());
					//Time passes
					playerTurnDelay = p->getMoveDelay();
				}
			}
		}
	}
}

/*
Apply effects of standing on a tile to the given person.
Usually ticks once per round, and once when the player first moves to this tile.
*/
void game::standOnTile(person * victim)
{
	maptile* mt = currentMap->getTile(victim->getx(), victim->gety());
	for (auto te : mt->getTouchEffects()) {
		if (te == ACID_TOUCH) {
			//Take damage! OUCH
			victim->takeDamage(5);
		}
	}
}

/*
	COMBAT
*/

/*
One creature attacks another in melee.
*/
void game::meleeAttack(person * attacker, person * target)
{
	target->takeDamage(10);
	//Update targeting
	if (target->isDead)
		attacker->clearTarget();
	else
		attacker->setTarget(target);
	//Make sure dead things are removed from the map
	clearDeadCreatures();
}

/*
	KEEPING THE WORLD UPDATED
*/

/*
Removes all dead creatures from the map.
*/
void game::clearDeadCreatures()
{
	personVector toClear;
	//Find everyone who's dead
	for (auto p : currentMap->getAllPeople()) {
		if (p->isDead)
			toClear.push_back(p);
	}
	//And remove them!
	for (auto p : toClear) {
		currentMap->removePerson(p);
	}
}
