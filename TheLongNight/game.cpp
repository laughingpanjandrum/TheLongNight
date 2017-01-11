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
	player->isPlayer = true;
	currentMap->addPerson(player, 3, 3);
	currentMap->updateFOV(player->getx(), player->gety());
	//Starting items
	currentMap->addItem(armour_RuinedUniform(), 6, 6);
	currentMap->addItem(weapon_SplinteredSword(), 2, 3);
	currentMap->addItem(consumable_StarwaterDraught(), 7, 5);
	currentMap->addItem(consumable_InvigoratingTea(), 7, 6);
	//Monsters
	for (int i = 0; i < 4; i++) {
		monster* m = monster_ThinWretch();
		currentMap->addPerson(m, 10 + i, 12 + i / 2);
		turns.addEntity(m, 1);
	}
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
Change the game state.
*/
void game::setState(gameState st)
{
	state = st;
}

void game::addMessage(std::string txt, TCODColor color)
{
	messages.push_back(message(txt, color));
	if (messages.size() > 5)
		messages.pop_back();
}

/*
	PASSAGE OF TIME
*/

void game::endPlayerTurn()
{
	//Timer effects happen
	tick();
	player->checkTargetValidity();
	//Add player to turn tracker
	turns.addEntity(player, playerTurnDelay);
	playerTurnDelay = 0;
	//Cycle through all turns
	person* nextTurn = turns.getNext();
	while (nextTurn != player) {
		doMonsterTurn(nextTurn);
		nextTurn = turns.getNext();
	}
}


/*
	AI ACTIONS
*/

/*
Returns whether the given ai will move to the given point.
*/
bool game::aiIsValidMove(person * ai, int xnew, int ynew)
{
	//Make sure it's in bounds
	if (!currentMap->inBounds(xnew, ynew))
		return false;
	else if (!currentMap->isWalkable(xnew, ynew))
		return false;
	//Make sure we won't attack a friendly
	person* here = currentMap->getPerson(xnew, ynew);
	if (here != nullptr && !here->isPlayer)
		return false;
	//Otherwise, we're good to go!
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
	//Figure out what to draw
	if (state == STATE_VIEW_MAP) {
		drawMap(MAP_DRAW_X, MAP_DRAW_Y);
	}
	else if (state == STATE_VIEW_INVENTORY) {
		drawInventory(MAP_DRAW_X, MAP_DRAW_Y);
	}
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
		else {
			//Is there an item here?
			item* it = currentMap->getItem(x, y);
			if (it != nullptr) {
				toDraw.tileCode = it->getTileCode();
				toDraw.color = it->getColor();
			}
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
	//Vigour
	win.drawCounter(player->getVigour(), "VIG ", atx, ++aty, TCODColor::darkGreen, TCODColor::darkGrey, 20);
	win.write(atx + 4, ++aty, player->getVigour().getAsString(), TCODColor::darkGreen);
	//	Equipment
	//Weapons
	weapon* wp = player->getWeapon();
	if (wp != nullptr) {
		//Weapon
		win.writec(atx, ++aty, wp->getTileCode(), wp->getColor());
		win.write(atx + 2, aty, wp->getMenuName(), wp->getColor());
		//Weapon special attack
		spell* atk = wp->getSpecialAttack();
		if (atk != nullptr) {
			win.writec(atx + 3, ++aty, VIGOUR_GLYPH, TCODColor::darkGreen);
			win.write(atx + 4, aty, std::to_string(atk->getVigourCost()), TCODColor::green);
			win.write(atx + 6, aty, atk->getName(), atk->getColor());
		}
	}
	else
		win.write(atx + 2, ++aty, "no weapon", TCODColor::darkGrey);
	//Armour
	armour* ar = player->getArmour();
	if (ar != nullptr) {
		win.writec(atx, ++aty, ar->getTileCode(), ar->getColor());
		win.write(atx + 2, aty, ar->getMenuName(), ar->getColor());
	}
	else
		win.write(atx + 2, ++aty, "no armour", TCODColor::darkGrey);
	//Consumable selected
	consumable* c = player->getSelectedConsumable();
	if (c != nullptr) {
		if (c != nullptr) {
			win.writec(atx, ++aty, c->getTileCode(), c->getColor());
			win.write(atx + 2, aty, c->getMenuName(), c->getColor());
		}
		else
			win.write(atx + 2, ++aty, "no consumable", TCODColor::darkGrey);
	}
	//Target info
	person* target = player->getTarget();
	if (target != nullptr) {
		aty++;
		//Health
		win.write(atx, ++aty, target->getName(), target->getColor());
		win.write(atx, ++aty, "HP:" + target->getHealth().getAsString(), TCODColor::darkRed);
		//Bleed
		int bleeding = target->getBleedDuration();
		if (bleeding > 0) {
			//Currently bleeding
			win.write(atx + 1, ++aty, "Bleeding (" + std::to_string(bleeding) + ")", TCODColor::crimson);
		}
		else {
			//Bleed building up but not proc'd
			counter* bleed = target->getSpecialEffectBuildup(EFFECT_BLEED);
			if (bleed->getValue() > 0)
				win.write(atx + 1, ++aty, "BLEED:" + bleed->getAsString(), TCODColor::crimson);
		}
	}
	//Messages
	atx = MAP_DRAW_X;
	aty = MAP_DRAW_Y + 40;
	for (auto m : messages) {
		win.write(atx, aty++, m.txt, m.color);
	}
}

/*
List inventory
*/
void game::drawInventory(int atx, int aty)
{
	//List all categories
	for (unsigned int i = 0; i < ALL_ITEM_TYPES.size(); i++) {
		//Category
		itemTypes cat = ALL_ITEM_TYPES[i];
		std::string title = getItemCategoryName(cat);
		win.write(atx, ++aty, title, TCODColor::lightGrey);
		//Items of this type that we have
		itemVector carriedItems = player->getItemsOfType(cat);
		//And draw all of THEM
		for (auto item : carriedItems) {
			win.write(atx + 1, ++aty, item->getName(), item->getColor());
		}
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
	//Menus
	else if (kp.vk == KEY_BACK_OUT)
		setState(STATE_VIEW_MAP);
	else if (kp.c == 'i')
		setState(STATE_VIEW_INVENTORY);
	//Using stuff
	else if (kp.c == 'c')
		player->cycleConsumable();
	else if (kp.c == 'q')
		useConsumable();
	else if (kp.c == 's')
		castSpell(player->getWeapon()->getSpecialAttack());
	//Movement
	else if (isMovementKey(kp))
		processMove(kp);
	//Debug
	else if (kp.c == 'w')
		player->takeDamage(5);
}

/*
We use up our selected consumable.
*/
void game::useConsumable()
{
	consumable* toUse = player->getSelectedConsumable();
	if (toUse != nullptr) {
		//Make sure we have enough
		if (toUse->getAmountLeft() > 0) {
			//Use it!
			toUse->lose();
			//And perform the proper effect
			int potency = toUse->getPotency();
			for (auto eff : toUse->getEffects()) {
				applyEffectToPerson(player, eff, potency);
			}
		}
	}
}

/*
	EFFECTS
*/

/*
Apply any effect to any person.
Its strength is the given potency.
*/
void game::applyEffectToPerson(person * target, effect eff, int potency)
{
	//Restoratives
	if (eff == RESTORE_HEALTH)
		target->addHealth(potency);
	else if (eff == RESTORE_VIGOUR)
		target->addVigour(potency);
	//Damage effects
	else if (eff == APPLY_BLEED_DAMAGE)
		target->takeStatusEffectDamage(EFFECT_BLEED, potency);
}

/*
	MOVEMENT
*/

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
	//If we're the PLAYER, we pick up any items that are here
	if (victim->isPlayer) {
		item* itemHere = currentMap->getItem(victim->getx(), victim->gety());
		if (itemHere != nullptr) {
			//Get item
			victim->addItem(itemHere);
			victim->equipItem(itemHere);
			//...and remove it from the floor
			currentMap->removeItem(itemHere);
		}
	}
	//Tile effects
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
	addMessage(attacker->getName() + " strikes " + target->getName(), attacker->getColor());
	//First: NORMAL DAMAGE
	weapon* wp = attacker->getWeapon();
	int damage = attacker->getBaseMeleeDamage();
	if (wp != nullptr) {
		damage = wp->getDamage();
	}
	//Reduce damage via armour
	int def = target->getDefence();
	int damageReduction = ((float)def / 100.0) * damage;
	damage -= damageReduction;
	//Minimum damage is 1
	if (damage < 1)
		damage = 1;
	//Deal the damage
	target->takeDamage(damage);
	//Next: SPELL DISCHARGES
	if (attacker->buffNextMelee != nullptr) {
		dischargeSpellOnTarget(attacker->buffNextMelee, attacker, target);
		attacker->buffNextMelee = nullptr;
	}
	//Next: STATUS EFFECTS
	if (wp != nullptr) {
		for (int idx = 0; idx < wp->getStatusEffectCount(); idx++) {
			target->takeStatusEffectDamage(wp->getStatusEffectType(idx), wp->getStatusEffectDamage(idx));
		}
	}
	//Update targeting
	if (target->isDead)
		attacker->clearTarget();
	else
		attacker->setTarget(target);
	//Make sure dead things are removed from the map
	clearDeadCreatures();
}


/*
	SPELL MAGIC
*/

/*
Player starts the casting of a spell.
*/
void game::castSpell(spell * sp)
{
	int vcost = sp->getVigourCost();
	if (player->getVigour().getValue() >= vcost) {
		//We have enough vig to cast. PROCEED.
		attackType aType = sp->getAttackType();
		if (aType == ATTACK_MELEE) {
			//Our next melee attack will have this buff.
			player->buffNextMelee = sp;
			addMessage("Select melee target", sp->getColor());
		}
	}
}

/*
Spell actually affects the target
*/
void game::dischargeSpellOnTarget(spell * sp, person * caster, person * target)
{
	addMessage(caster->getName() + " hits " + target->getName() + " with " + sp->getName(), sp->getColor());
	//Iterate through all effects
	for (int idx = 0; idx < sp->getEffectsCount(); idx++) {
		applyEffectToPerson(target, sp->getEffectType(idx), sp->getEffectPotency(idx));
	}
	//This is when the caster expends their vigour
	caster->loseVigour(sp->getVigourCost());
}

/*
	KEEPING THE WORLD UPDATED
*/

/*
Happens right after the player's turn.
This is when status effects proc, & everything else that involves a timer happens.
*/
void game::tick()
{
	//Creature ticks
	for (auto p : currentMap->getAllPeople()) {
		p->tick();
	}
	//Clear out any lingering death
	clearDeadCreatures();
}

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
