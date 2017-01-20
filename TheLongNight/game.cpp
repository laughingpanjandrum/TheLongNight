#include "game.h"



game::game()
{
	//We start by creating an empty map, just for now
	map* newmap = new map();
	newmap = makemap.loadMapFromFile(makemap.getStartMapHandle());
	setCurrentMap(newmap);
	newmap->respawnAllMonsters();
	//Remember the first map
	addKnownMap(newmap, makemap.getStartMapHandle());
	//Character create
	player = new person();
	player->isPlayer = true;
	player->stats = new statline(1, 1, 1, 1, 1, 1, 1);
	//Find starting position
	coord startPt = currentMap->getStartPoint();
	currentMap->addPerson(player, startPt.first, startPt.second);
	currentMap->updateFOV(player->getx(), player->gety());
	//Make our starting position a save point
	setSavePoint();
	//Add monsters to clock
	for (auto m : currentMap->getAllPeople()) {
		if (!m->isPlayer)
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
		updateAnimations();
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



/*
	MAP MANAGEMENT
*/


/*
Add a new map.
The handle is the name of the map file it was loaded from.
It's how maps are differentiated, and how they keep track of interconnections.
*/
void game::addKnownMap(map * m, std::string handle)
{
	allMaps.push_back(m);
	allMapHandles.push_back(handle);
}

/*
Get a map we have memorized.
If we don't find the handle, we return nullptr.
*/
map * game::getKnownMap(std::string handle)
{
	for (int i = 0; i < allMapHandles.size(); i++) {
		if (allMapHandles.at(i) == handle)
			return allMaps.at(i);
	}
	return nullptr;
}




/*
	MESSAGES
*/


void game::addMessage(std::string txt, TCODColor color)
{
	messages.push_back(message(txt, color));
	if (messages.size() > 5) {
		//Delete first message
		messages.erase(messages.begin());
	}
}


/*
	TARGETING
*/


void game::toggleTargetMode()
{
	targetModeOn = !targetModeOn;
	if (targetModeOn) {
		//Starting position
		setCursorPosition(player->getPosition());
	}
}

/*
Set position using two intss
*/
void game::setCursorPosition(int xnew, int ynew)
{
	//Move cursor position
	targetPt.first = xnew;
	targetPt.second = ynew;
	//If the cursor is over something, make it our target
	person* target = currentMap->getPerson(xnew, ynew);
	if (target != nullptr)
		player->setTarget(target);
}

/*
Set position using another coordinate
*/
void game::setCursorPosition(coord xy)
{
	targetPt.first = xy.first;
	targetPt.second = xy.second;
}


/*
	PATHFINDING
*/

/*
Returns TCOD-format path to the given coord
*/
TCODPath* game::getPathToCoord(coord startxy, coord endxy)
{
	TCODPath *path = currentMap->getPath();
	path->compute(startxy.first, startxy.second, endxy.first, endxy.second);
	return path;
}


/*
Builds a vector of x,y coordinate pairs in a straight line from the start point to the endpoint.
*/
pathVector game::getLine(coord startxy, coord endxy)
{
	pathVector path;
	coord atxy = startxy; //Current position
	//Vectors to final point
	int xvec = get1dVector(startxy.first, endxy.first);
	int yvec = get1dVector(startxy.second, endxy.second);
	//Adjust one point at a time until we get there
	while (atxy != endxy) {
		//Move the current point
		if (atxy.first != endxy.first)
			atxy.first += xvec;
		if (atxy.second != endxy.second)
			atxy.second += yvec;
		//Append to path
		coord thisPt = atxy;
		path.push_back(thisPt);
	}
	//Done
	return path;
}

/*
Returns the first person we find walking along the given path vector.
*/
person * game::getTargetOnPath(pathVector path)
{
	for (auto pt : path) {
		person* target = currentMap->getPerson(pt.first, pt.second);
		if (target != nullptr)
			return target;
	}
	return nullptr;
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
	//Make sure dead things are removed from the map
	clearDeadCreatures();
}


/*
	AI ACTIONS
*/

/*
Returns a list of all adjacent coords that the given ai will willingly walk upon.
*/
pathVector game::getAllAdjacentWalkable(monster * ai)
{
	int x = ai->getx();
	int y = ai->gety();
	pathVector pts = { coord(x - 1, y), coord(x + 1, y), coord(x, y - 1), coord(x, y + 1) };
	pathVector goodPts;
	for (auto pt : pts) {
		if (aiIsValidMove(ai, pt.first, pt.second))
			goodPts.push_back(pt);
	}
	return goodPts;
}

/*
Returns whether the given ai will move to the given point.
*/
bool game::aiIsValidMove(monster * ai, int xnew, int ynew)
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
Returns whether we're done moving.
We might not be done if we get multiple moves in one turn!
*/
bool game::aiMoveToTarget(monster * ai)
{

	//If our target died at some point prior to this, QUIT MOVING
	if (ai->getTarget() == nullptr)
		return true;

	//Rudimentary pathing. First we list all walkable points adjacent to us
	pathVector pts = getAllAdjacentWalkable(ai);
	//If there are no walkable points, GIVE UP
	if (pts.size() == 0) {
		turns.addEntity(ai, ai->getMoveDelay());
		return true;
	}

	//If there are, find the one that's nearest our target
	person* t = ai->getTarget();
	coord bestPt = pts.at(0);
	int bestDist = hypot(bestPt.first - t->getx(), bestPt.second - t->gety());
	//Look for the shortest distance (or the greatest distance, if we're the timid type)
	for (int i = 1; i < pts.size(); i++) {
		coord pt = pts.at(i);
		int newDist = hypot(pt.first - t->getx(), pt.second - t->gety());
		//Figure out what we want - to get closer or further away
		bool cond;
		if (ai->keepsDistance && !ai->hasFreeMoves())
			cond = newDist > bestDist;
		else
			cond = newDist < bestDist;
		//Now see if the new point is better according to our condition
		if (cond) {
			bestDist = newDist;
			bestPt = pt;
		}
	}

	//Now move to this point
	movePerson(ai, bestPt.first, bestPt.second);
	//Time passes (UNLESS WE HAVE FREE MOVES!)
	if (!ai->hasFreeMoves()) {
		turns.addEntity(ai, ai->getMoveDelay());
		return true;
	}
	else {
		ai->useFreeMove();
		return false;
	}

}

/*
See if we want to cast any of our spells.
*/
bool game::aiTryUseSpell(monster * ai)
{
	//Random chance to not cast a spell
	int r = randint(1, 100);
	if (r > ai->getSpellCastChance())
		return false;
	//Sort through all our spells and see if any of them look likely
	person* target = ai->getTarget();
	for (auto sp : ai->getSpellsKnown()) {
		attackType aType = sp->getAttackType();
		if (aType == ATTACK_RANGE) {
			//Ranged spell - do we have the reach for it?
			pathVector path = getLine(ai->getPosition(), target->getPosition());
			person* willHit = getTargetOnPath(path);
			if (willHit == target) {
				//Check spell range
				int dist = hypot(ai->getx() - target->getx(), ai->gety() - target->gety());
				if (dist <= sp->getAttackRange()) {
					//animation
					addAnimations(new bulletPath(path, BULLET_TILE, sp->getColor()));
					//We hit!
					dischargeSpellOnTarget(sp, ai, target);
					//Time passes
					turns.addEntity(ai, ai->getAttackDelay());
					return true;
				}
			}
		}
		else if (aType == ATTACK_AOE) {
			//See if AOE spell will hit target
			int dist = hypot(ai->getx() - target->getx(), ai->gety() - target->gety());
			if (dist <= sp->getAttackRange()) {
				//Do the AOE!
				doAOE(sp, ai);
				turns.addEntity(ai, ai->getAttackDelay());
				return true;
			}
		}
		else if (aType == ATTACK_BUFF_SELF) {
			//Cast spell on self - usually a good idea!
			dischargeSpellOnTarget(sp, ai, ai);
		}
	}
	//We didn't use any abilities
	return false;
}

/*
Try to spawn something nearby!
*/
void game::aiSpawnCreature(monster * ai)
{

	//First find a free point adjacent to us
	coordVector pts;
	for (int x = ai->getx() - 1; x <= ai->getx() + 1; x++) {
		for (int y = ai->gety() - 1; y <= ai->gety() + 1; y++) {
			//IS THIS POINT OKAY?
			if (aiIsValidMove(ai,x,y) && currentMap->getPerson(x, y) == nullptr) {
				//YES IT IS OKAY
				pts.push_back(coord(x, y));
			}
		}
	}

	//now PICK A RANDOM ONE
	if (pts.size()) {
		int p = randrange(pts.size());
		coord pt = pts.at(p);
		monster* m = ai->getRandomSpawn();
		//AND PUT IT DOWN
		currentMap->addPerson(m, pt.first, pt.second);
		//And add it to the clock
		turns.addEntity(m, 1);
		//And the current AI also uses up its turn
		turns.addEntity(ai, ai->getAttackDelay());
	}

}

/*
What we choose to do if we have a target.
*/
void game::aiDoCombatAction(monster * ai)
{
	//First see if we want to spawn something!
	if (ai->canSpawnCreatures() && ai->wantsToSpawn())
		aiSpawnCreature(ai);
	//If not, try casting a spell. If that fails, move towards our target.
	else if (!aiTryUseSpell(ai))
		//We keep moving until the move function says we're done.
		while (!aiMoveToTarget(ai)) {}
}

/*
AI tries to find something to kill.
*/
void game::aiFindTarget(monster * ai)
{
	//If the player can see us, we can see them. ONE SIMPLE RULE.
	if (currentMap->isPointInFOV(ai->getx(), ai->gety())) {
		ai->setTarget(player);
		//If we are a BOSS, it looks like a BOSS FIGHT just started
		if (ai->isBoss)
			setBoss(ai);
	}
}

/*
Monster does its turn and then is placed back into the turn tracker.
*/
void game::doMonsterTurn(person * p)
{
	monster* ai = static_cast<monster*>(p);
	//If we're dead, we probably shouldn't do anything
	if (ai->isDead)
		return;
	//Likewise, if we're non-hostile, we don't do anything
	if (!ai->isHostile)
		return;
	//Do we have a target?
	person* target = ai->getTarget();
	if (target == nullptr)
		aiFindTarget(ai);
	//If we have one, just move towards it
	if (target != nullptr)
		aiDoCombatAction(ai);
	else
		//If not, just wait
		turns.addEntity(ai, 1);
}



/*
	MENUS
*/


/*
Just displays a menu! It's pretty simple.
*/
void game::drawMenu(menu * m, int atx, int aty)
{
	//Title
	win.write(atx, aty, m->getTitle(), TCODColor::white);
	//Elements
	auto elements = m->getAllElements();
	for (auto it = elements.begin(); it != elements.end(); it++) {
		//SHOW NAME
		win.write(atx + 2, ++aty, (*it)->getName(), (*it)->getColor());
		//Indicate whether this is the highlighted element
		if (m->getSelectedItem() == (*it)) {
			win.writec(atx + 1, aty, '>', TCODColor::white);
		}
	}
}

/*
Change menu selection
*/
void game::navigateMenu(TCOD_key_t kp)
{
	if (kp.vk == KEY_NORTH) {
		currentMenu->scrollUp();
	}
	else {
		currentMenu->scrollDown();
	}
}

/*
Press ENTER in a menu
*/
void game::acceptCurrentMenuIndex()
{
	//What this does depends on which MENU we're in
	if (state == STATE_VIEW_INVENTORY) {
		//Selected menu object should be an ITEM
		item* sel = static_cast<item*>(currentMenu->getSelectedItem());
		selectInventoryCategory(sel->getCategory());
	}
	else if (state == STATE_VIEW_INVENTORY_CATEGORY) {
		//Equip the selected item
		item* sel = static_cast<item*>(currentMenu->getSelectedItem());
		if (sel != nullptr)
			equipItem(sel);
	}
	else if (state == STATE_LEVEL_UP_MENU)
		doLevelUp();
	else if (state == STATE_SELECT_SPELL)
		selectSpellFromMenu();
	else if (state == STATE_SELECT_CONSUMABLE)
		selectConsumableFromMenu();
	else if (state == STATE_SHOP_MENU)
		buyItemFromShop();
}

/*
Return to previous menu, if possible, or close menu altogether
*/
void game::menuBackOut()
{
	if (state == STATE_VIEW_INVENTORY_CATEGORY) {
		//Back up to standard inventory menu
		createInventoryMenu();
	}
	else if (state == STATE_LEVEL_UP_MENU)
		//Back to main inventory menu
		createInventoryMenu();
	else {
		//Return to map
		setState(STATE_VIEW_MAP);
	}
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
	if (state == STATE_VIEW_INVENTORY || state == STATE_VIEW_INVENTORY_CATEGORY) {
		drawInventory(MAP_DRAW_X, MAP_DRAW_Y);
	}
	else if (state == STATE_LEVEL_UP_MENU)
		drawLevelUpMenu(MAP_DRAW_X, MAP_DRAW_Y);
	else if (state == STATE_SHOP_MENU)
		drawShopMenu(MAP_DRAW_X, MAP_DRAW_Y);
	else
		drawMap(MAP_DRAW_X, MAP_DRAW_Y);
	//Always draw the interface
	drawInterface(MAP_DRAW_X + 43, MAP_DRAW_Y);
	//win.drawFont();
	win.refresh();
}

/*
Just draws the current map.
Input: Coordinates to start drawing at (the top left corner of the map)
*/
void game::drawMap(int leftx, int topy)
{
	//Draw map name just above
	win.write(leftx + 10, topy - 1, currentMap->getName(), TCODColor::white);
	//Draw entire map, left to right & top to bottom
	for (int x = 0; x < currentMap->getXSize(); x++) {
		for (int y = 0; y < currentMap->getYSize(); y++) {
			//Figure out what to draw here, then draw it
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
		//What to draw if out of FOV
		if (currentMap->inMemoryMap(x, y)) {
			toDraw.color = win.mixColors(toDraw.color, TCODColor::black, 0.9);
			toDraw.bgcolor = win.mixColors(toDraw.bgcolor, TCODColor::black, 0.9);
		}
		else {
			toDraw.tileCode = EMPTY_TILE;
			toDraw.color = TCODColor::black;
			toDraw.bgcolor = TCODColor::black;
		}
	}
	else {
		//If we can see it, add it to the memory map
		currentMap->addToMemoryMap(x, y);
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
		//Allow animations to adjust draw data
		toDraw = getAnimationDataOverride(&toDraw, x, y);
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
	//Highlight cursor position
	if (targetModeOn && x == targetPt.first && y == targetPt.second)
		toDraw.bgcolor = TCODColor::pink;
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
	}
	else
		win.write(atx + 2, ++aty, "no weapon", TCODColor::darkGrey);
	
	//Offhand item
	weapon* of = player->getOffhand();
	if (of != nullptr) {
		win.writec(atx, ++aty, of->getTileCode(), of->getColor());
		win.write(atx + 2, aty, of->getMenuName(), of->getColor());
	}
	
	//Helmet
	armour* helm = player->getHelmet();
	if (helm != nullptr) {
		win.writec(atx, ++aty, helm->getTileCode(), helm->getColor());
		win.write(atx + 2, aty, helm->getMenuName(), helm->getColor());
	}
	else
		win.write(atx + 2, ++aty, "no helmet", TCODColor::darkGrey);
	
	//Armour
	armour* ar = player->getArmour();
	if (ar != nullptr) {
		win.writec(atx, ++aty, ar->getTileCode(), ar->getColor());
		win.write(atx + 2, aty, ar->getMenuName(), ar->getColor());
	}
	else
		win.write(atx + 2, ++aty, "no armour", TCODColor::darkGrey);

	//Charms
	charm* ch = player->getCharm();
	if (ch != nullptr) {
		win.writec(atx, ++aty, ch->getTileCode(), ch->getColor());
		win.write(atx + 2, aty, ch->getMenuName(), ch->getColor());
	}
	else
		win.write(atx + 2, ++aty, "no charm", TCODColor::darkGrey);

	aty++;
	
	//Consumable selected, or menu if we have that open
	if (state == STATE_SELECT_CONSUMABLE) {
		drawMenu(currentMenu, atx, ++aty);
		aty += currentMenu->getAllElements().size() + 1;
	}
	else {
		consumable* c = player->getSelectedConsumable();
		if (c != nullptr) {
			if (c != nullptr) {
				win.writec(atx, ++aty, c->getTileCode(), c->getColor());
				win.write(atx + 2, aty, c->getMenuName(), c->getColor());
			}
			else
				win.write(atx + 2, ++aty, "no consumable", TCODColor::darkGrey);
		}
	}
	
	//Spell selected (or menu, if we're in that mode!)
	if (state == STATE_SELECT_SPELL) {
		drawMenu(currentMenu, atx, ++aty);
		aty += currentMenu->getAllElements().size() + 1;
	}
	else {
		spell* sp = player->getCurrentSpell();
		if (sp != nullptr) {
			win.writec(atx + 3, ++aty, VIGOUR_GLYPH, TCODColor::darkGreen);
			win.write(atx + 4, aty, std::to_string(sp->getVigourCost()), TCODColor::green);
			win.write(atx + 6, aty, sp->getName(), sp->getColor());
		}
	}

	//Money
	win.writec(atx, ++aty, FRAGMENT_GLYPH, TCODColor::amber);
	win.write(atx + 2, aty, std::to_string(fragments), TCODColor::lightAmber);
	
	//Buffs
	if (player->hasFreeMoves()) {
		win.write(atx, ++aty, "CHARGE!", TCODColor::yellow);
	}
	
	//Status effects
	if (player->getBleedDuration()) {
		win.write(atx, ++aty, "BLEEDING " + std::to_string(player->getBleedDuration()), TCODColor::crimson);
	}
	else {
		auto bl = player->getSpecialEffectBuildup(EFFECT_BLEED);
		if (bl->getValue() > 0)
			win.drawCounter(*bl, "BLEED", atx, ++aty, TCODColor::crimson, TCODColor::darkGrey, 10);
	}

	//Draws whatever we have HIGHLIGHTED
	aty += 2;
	drawMouseover(atx, aty);

	//List controls
	atx += 10;
	aty = MAP_DRAW_Y + 43;
	win.writec(atx, aty, 'a', TCODColor::green);
	win.write(atx + 2, aty, "Select spell", TCODColor::white);
	win.writec(atx, ++aty, 'c', TCODColor::green);
	win.write(atx + 2, aty, "Select consumable", TCODColor::white);
	win.writec(atx, ++aty, 't', TCODColor::green);
	win.write(atx + 2, aty, "Toggle targeting mode", TCODColor::white);
	win.writec(atx, ++aty, 'd', TCODColor::green);
	win.write(atx + 2, aty, "Switch to secondary weapon", TCODColor::white);
	win.writec(atx, ++aty, 's', TCODColor::green);
	win.write(atx + 2, aty, "Cast current spell", TCODColor::white);
	win.writec(atx, ++aty, 'q', TCODColor::green);
	win.write(atx + 2, aty, "Use current consumable", TCODColor::white);
	win.writec(atx, ++aty, 'T', TCODColor::green);
	win.write(atx + 2, aty, "Talk to friendly NPC", TCODColor::white);

	//BOSS HEALTH BAR, if we're fighting a boss
	atx = MAP_DRAW_X;
	aty = MAP_DRAW_Y + 42;
	if (currentBoss != nullptr) {
		win.write(atx, aty, currentBoss->getName(), currentBoss->getColor());
		win.drawCounter(currentBoss->getHealth(), "", atx, aty + 1, TCODColor::red, TCODColor::darkGrey, 40);
	}

	//Messages
	aty += 3;
	for (auto m : messages) {
		win.write(atx, aty++, m.txt, m.color);
	}
}

/*
List inventory.
When an item is selected, show its description.
*/

void game::drawInventory(int atx, int aty)
{
	//The menu
	drawMenu(currentMenu, MAP_DRAW_X, MAP_DRAW_Y);
	//Show selected item, if relevant
	if (state == STATE_VIEW_INVENTORY_CATEGORY) {
		if (currentMenu->getSelectedItem() != nullptr) {
			//Selected menu object should be an ITEM
			item* sel = static_cast<item*>(currentMenu->getSelectedItem());
			drawItemInfo(sel, atx, aty + 20);
		}
	}
	else {
		//We can go to the level-up menu from here
		win.write(atx, aty + 20, "Press [C] to LEVEL UP", TCODColor::white);
	}
}


/*
Just sums up all of our stats on one sexy screen!
*/
void game::drawPlayerInfo(int atx, int aty)
{
	TCODColor mainCol = TCODColor::lightGrey;
	int offset = 13;
	
	//Draw a big ole box
	win.clearRegion(atx, aty, 40, 40);
	win.drawBox(atx, aty, 40, 40, TCODColor::sepia);
	
	//Now FILL IN SOME DEETZ
	atx++;
	aty++;
	win.write(atx, aty, player->getName(), player->getColor());
	aty++;
	
	//Health and vigour
	win.write(atx + 1, ++aty, "Health", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getHealth().getMaxValue()), TCODColor::red);
	win.writec(atx, aty, HEALTH_GLYPH, TCODColor::red);
	win.write(atx + 1, ++aty, "Vigour", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getVigour().getMaxValue()), TCODColor::green);
	win.writec(atx, aty, VIGOUR_GLYPH, TCODColor::green);
	aty++;
	
	//Defence
	win.write(atx, ++aty, "DEFENCE", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getDefence()), TCODColor::lightBlue);
	//Damage resistances
	for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
		damageType dr = static_cast<damageType>(r);
		int res = player->getDamageResist(dr);
		win.write(atx + 1, ++aty, getDamageTypeName(dr), mainCol);
		win.write(atx + offset, aty, std::to_string(res), getDamageTypeColor(dr));
	}
	aty++;

	//Bleed resist
	win.write(atx, ++aty, "BLEED Resist", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getSpecialEffectBuildup(EFFECT_BLEED)->getMaxValue()), TCODColor::crimson);
	aty++;

	//Magic stuff
	win.write(atx, ++aty, "SPELL POWER", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getSpellPower()), TCODColor::magenta);
	win.write(atx, ++aty, "DIVINE POWER", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getDivinePower()), TCODColor::darkYellow);
	aty++;

	//List buffs
	for (auto b : player->getAllBuffs()) {
		win.write(atx, ++aty, b->name, b->color);
	}
	
	//Done! Wait for input
	win.refresh();
	while (win.getkey().vk != KEY_ACCEPT) {}
}


/*
Display info about whatever we've highlighted with the CURSOR.
*/
void game::drawMouseover(int atx, int aty)
{
	//Is the cursor even on?
	if (!targetModeOn) {
		//If not, just show whatever the player is targeting
		person* target = player->getTarget();
		if (target != nullptr)
			drawTargetInfo(target, atx, aty);
		return;
	}
	//What are we pointing at?
	coord mpt = targetPt;
	if (currentMap->inBounds(mpt.first, mpt.second) && currentMap->isPointInFOV(mpt.first, mpt.second)) {
		//Show highlighted object: person?
		person* target = currentMap->getPerson(mpt.first, mpt.second);
		if (target != nullptr)
			drawTargetInfo(target, atx, aty);
		else {
			//Item?
			item* it = currentMap->getItem(mpt.first, mpt.second);
			if (it != nullptr) {
				win.write(atx, aty, it->getName(), it->getColor());
			}
			else {
				//If none of the above, show the tile here
				maptile* t = currentMap->getTile(mpt.first, mpt.second);
				win.write(atx, aty, t->getName(), t->getColor());
			}
		}
	}
}


/*
Info about a person we have highlighted on the map.
*/
void game::drawTargetInfo(person * target, int atx, int aty)
{
	//Health
	win.write(atx, aty, target->getName(), target->getColor());
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


/*
Allows animations to adjust draw data when drawing the map.
*/
drawData game::getAnimationDataOverride(drawData * baseData, int x, int y)
{
	for (auto a : playingAnimations) {
		baseData = &a->getDrawData(baseData, x, y);
	}
	return *baseData;
}

/*
Progresses animations and checks to see if any are done and should be deleted.
*/
void game::updateAnimations()
{
	animVector toDelete;
	//Progress and check for deletions
	for (auto a : playingAnimations) {
		a->tick();
		if (a->isDone())
			toDelete.push_back(a);
	}
	//And delete anything that's expired
	for (auto a : toDelete) {
		auto it = std::find(playingAnimations.begin(), playingAnimations.end(), a);
		if (it != playingAnimations.end())
			playingAnimations.erase(it);
	}
}


/*
	ITEM INFO
*/

/*
This just figures out which particular info-drawing function to use.
*/
void game::drawItemInfo(item * it, int atx, int aty)
{
	//Tile and name
	win.writec(atx, aty, it->getTileCode(), it->getColor());
	win.write(atx + 2, aty, it->getName(), it->getColor());
	//Type of item
	win.write(atx, ++aty, getItemCategoryName(it->getCategory()), TCODColor::darkGrey);
	//Indicate whether equipped or not
	if (player->hasItemEquipped(it))
		win.write(atx + 12, aty, "EQUIPPED", TCODColor::grey);
	//Show price, if one is defined
	if (it->getPrice() > 0) {
		win.write(atx, ++aty, "PRICE: ", TCODColor::white);
		win.writec(atx + 6, aty, FRAGMENT_GLYPH, TCODColor::amber);
		win.write(atx + 8, aty, std::to_string(it->getPrice()), TCODColor::amber);

	}
	//Item description
	aty = win.writeWrapped(atx + 1, aty + 1, 40, it->description, TCODColor::lightGrey);
	//Rest of item info
	aty += 1;
	atx += 1;
	auto cat = it->getCategory();
	switch (cat) {
	case(ITEM_WEAPON): drawWeaponInfo(static_cast<weapon*>(it), atx, aty); break;
	case(ITEM_OFFHAND): drawWeaponInfo(static_cast<weapon*>(it), atx, aty); break;
	case(ITEM_BODY_ARMOUR): drawArmourInfo(static_cast<armour*>(it), atx, aty); break;
	case(ITEM_HELMET): drawArmourInfo(static_cast<armour*>(it), atx, aty); break;
	}
}

/*
Weapon descriptions.
*/
void game::drawWeaponInfo(weapon * it, int atx, int aty)
{
	int offset = 13;
	TCODColor maincol = TCODColor::white;
	//Damage
	win.write(atx, aty, "DAMAGE", TCODColor::darkRed);
	win.write(atx + offset, aty, std::to_string(it->getDamage()), maincol);
	//Rate
	win.write(atx, ++aty, "SPEED", TCODColor::yellow);
	win.write(atx + offset, aty, getAttackSpeedName(it->getAttackDelay()), maincol);
	//Defence, if any
	if (it->getDefence() > 0) {
		win.write(atx, ++aty, "DEFENCE", TCODColor::sepia);
		win.write(atx + offset, aty, std::to_string(it->getDefence()), maincol);
	}
	//Bleed resist
	if (it->getBleedResist() > 0) {
		win.write(atx, ++aty, "BLEED Resist", TCODColor::crimson);
		win.write(atx + offset, aty, std::to_string(it->getBleedResist()), maincol);
	}
	//Magic attributes, if any
	if (it->getSpellstoreSize()) {
		win.write(atx, ++aty, "SPELLSTORE", TCODColor::magenta);
		win.write(atx + offset, aty, std::to_string(it->getSpellstoreSize()), maincol);
	}
	if (it->getSpellPower()) {
		win.write(atx, ++aty, "SPELL POWER", TCODColor::lightBlue);
		win.write(atx + offset, aty, std::to_string(it->getSpellPower()), maincol);
	}
	if (it->getDivinePower()) {
		win.write(atx, ++aty, "DIVINE POWER", TCODColor::darkYellow);
		win.write(atx + offset, aty, std::to_string(it->getDivinePower()), maincol);
	}
	//Special attack, if any
	spell* atk = it->getSpecialAttack();
	if (atk != nullptr) {
		//Name of special attack
		win.writec(atx, ++aty, VIGOUR_GLYPH, TCODColor::green);
		win.write(atx + 1, aty, std::to_string(atk->getVigourCost()), TCODColor::green);
		win.write(atx + 4, aty, atk->getName(), atk->getColor());
		//Special attack description
		win.write(atx + 4, ++aty, '(' + atk->description + ')', TCODColor::lightGrey);
	}
}

/*
Armour descriptions.
*/
void game::drawArmourInfo(armour * it, int atx, int aty)
{
	int offset = 10;
	TCODColor maincol = TCODColor::white;
	//DEF
	win.write(atx, aty, "DEFENCE", TCODColor::sepia);
	win.write(atx + offset, aty, std::to_string(it->getDefence()), maincol);
	//Damage resistances
	for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
		damageType dr = static_cast<damageType>(r);
		int res = it->getDamageResist(dr);
		if (res > 0) {
			win.write(atx + 1, ++aty, getDamageTypeName(dr), getDamageTypeColor(dr));
			win.write(atx + offset, aty, std::to_string(res), maincol);
		}
	}
	//Bleed resist
	int br = it->getBleedResist();
	if (br) {
		win.write(atx, ++aty, "BLEED RES", TCODColor::crimson);
		win.write(atx + offset, aty, std::to_string(br), maincol);
	}
	//Move speed adjustment, if body armour
	if (it->getCategory() == ITEM_BODY_ARMOUR) {
		win.write(atx, ++aty, "SPEED", TCODColor::orange);
		win.write(atx + offset, aty, getAttackSpeedName(it->getMoveSpeed()), maincol);
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
		menuBackOut();
	else if (kp.c == 'i')
		createInventoryMenu();
	else if (kp.vk == KEY_ACCEPT)
		acceptCurrentMenuIndex();
	else if (kp.c == 'C') {
		if (state == STATE_VIEW_INVENTORY)
			setupLevelUpMenu();
		else
			drawPlayerInfo(MAP_DRAW_X, MAP_DRAW_Y);
	}

	//Using stuff
	else if (kp.c == 'c')
		openConsumableMenu();//player->cycleConsumable();
	else if (kp.c == 'a')
		openSpellMenu();//player->cycleSelectedSpell();
	else if (kp.c == 'q')
		useConsumable();
	else if (kp.c == 's')
		castSpell(player->getCurrentSpell());
	else if (kp.c == 'd') {
		player->swapWeapon();
		playerTurnDelay += SPEED_NORMAL;
	}

	//Movement
	else if (kp.c == 't')
		toggleTargetMode();
	else if (kp.c == 'z')
		playerTurnDelay = player->getMoveDelay();
	else if (isMovementKey(kp))
		if (state == STATE_VIEW_MAP)
			processMove(kp);
		else
			navigateMenu(kp);

	//Chitchat
	else if (kp.c == 'T')
		talkToShopkeeper();

	//Debug
	else if (kp.c == '~')
		debugMenu();
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
			//Is this a ranged-attack item?
			if (toUse->isRangedAttackItem()) {
				//We're basically casting a SPELL here
				doRangedSpell(toUse->getRangedAttack());
			}
			else {
				//All effects are applied to PERSON
				for (auto eff : toUse->getEffects()) {
					applyEffectToPerson(player, eff, potency);
				}
			}
			//TIME PASSAGE DUDE
			playerTurnDelay += SPEED_NORMAL;
		}
	}
}

/*
Menu where we select which consumable we want highlighted.
*/
void game::openConsumableMenu()
{
	menu* cmenu = new menu("SELECT ITEM");
	for (auto it : player->getConsumableList()) {
		if (it != nullptr)
			cmenu->addElement(it);
	}
	currentMenu = cmenu;
	setState(STATE_SELECT_CONSUMABLE);
}

/*
Actually pick a consumable to highlight
*/
void game::selectConsumableFromMenu()
{
	//Select item
	consumable* it = static_cast<consumable*>(currentMenu->getSelectedItem());
	player->setCurrentConsumable(it);
	//Close menu now!
	setState(STATE_VIEW_MAP);
}

/*
	EFFECTS
*/

/*
Discharge an AOE spell.
*/
void game::doAOE(spell * sp, person * caster)
{
	int r = sp->getAttackRange();
	for (int x = caster->getx() - r; x <= caster->getx() + r; x++) {
		for (int y = caster->gety() - r; y <= caster->gety() + r; y++) {
			//Check for target here
			person* target = currentMap->getPerson(x, y);
			//AOE doesn't affect caster
			if (target != nullptr && target != caster) {
				dischargeSpellOnTarget(sp, caster, target);
			}
		}
	}
	//Animation of the event; colour goes from spell colour to a lighter version on the outside
	TCODColor col1 = sp->getColor();
	TCODColor col2 = win.mixColors(col1, TCODColor::white, 0.5);
	addAnimations(new explosion(caster->getPosition(), r, col1, col2));
}

/*
Apply any effect to any person.
Its strength is the given potency.
*/
void game::applyEffectToPerson(person * target, effect eff, int potency)
{
	
	//Interface stuff for PCs only
	
	if (eff == ALLOW_INVENTORY_MANAGEMENT && target->isPlayer)
		//Can manage inventory when we step on this tile
		createInventoryMenu();
	else if (eff == SET_SAVE_POINT && target->isPlayer)
		//This is a save point; we'll respawn here on death
		setSavePoint();
	else if (eff == UNLOCK_ADJACENT_DOORS && target->isPlayer)
		//Auto-unlock adjacent doors
		unlockAdjacentTiles(target->getx(), target->gety());
	else if (eff == CHECK_FOR_UNLOCK && target->isPlayer)
		tryUnlockDoor(target->getx(), target->gety());

	//Restoratives

	else if (eff == FULL_RESTORE)
		target->fullRestore();
	else if (eff == RESTORE_HEALTH)
		target->addHealth(potency);
	else if (eff == RESTORE_VIGOUR)
		target->addVigour(potency);

	//Buffs

	else if (eff == GAIN_FREE_MOVES)
		target->gainFreeMoves(potency);
	else if (eff == SCALE_NEXT_ATTACK)
		target->scaleNextAttack = potency;
	else if (eff == SCALE_NEXT_SPELL)
		target->scaleNextSpell = potency;
	else if (eff == SCALE_NEXT_PRAYER)
		target->scaleNextPrayer = potency;
	else if (eff == ADD_HEALTH_TRICKLE)
		target->healthTrickle += potency;

	//Defensive buffs

	else if (eff == GAIN_DEFENCE)
		target->addDefence(potency);

	//Damage effects

	else if (eff == APPLY_PHYSICAL_DAMAGE)
		target->takeDamage(potency, DAMAGE_PHYSICAL);
	else if (eff == APPLY_MAGIC_DAMAGE)
		target->takeDamage(potency, DAMAGE_MAGIC);
	else if (eff == APPLY_BLEED_DAMAGE)
		target->takeStatusEffectDamage(EFFECT_BLEED, potency);

	//Special effects, other
	else if (eff == KNOCKBACK_TARGET)
		knockbackTarget(target, potency);

}

/*
	MOVEMENT
*/

/*
Movement commands
*/
void game::processMove(TCOD_key_t kp)
{
	//Cursor or player?
	int xnew;
	int ynew;
	if (targetModeOn) {
		xnew = targetPt.first;
		ynew = targetPt.second;
	}
	else {
		xnew = player->getx();
		ynew = player->gety();
	}
	//Find new position
	if (kp.vk == KEY_NORTH)
		ynew--;
	else if (kp.vk == KEY_SOUTH)
		ynew++;
	else if (kp.vk == KEY_EAST)
		xnew++;
	else if (kp.vk == KEY_WEST)
		xnew--;
	//Are we moving the cursor or the player?
	if (targetModeOn)
		setCursorPosition(xnew, ynew);
	else
		playerMoveLogic(xnew, ynew);
}

/*
Returns whether the given key is a movement command
*/
bool game::isMovementKey(TCOD_key_t kp)
{
	return kp.vk == KEY_NORTH || kp.vk == KEY_EAST || kp.vk == KEY_SOUTH || kp.vk == KEY_WEST;
}

/*
This handles a player move.
These differ from NPC movements, since the player can potentially move to a new map.
*/
void game::playerMoveLogic(int xnew, int ynew)
{
	//Are we moving off the map? If so, see if we can move to a new map
	if (!currentMap->inBounds(xnew, ynew)) {
		tryMapChange(xnew, ynew);
		return;
	}
	//The movement itself
	movePerson(player, xnew, ynew);
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
				//Player delay, if this is the player
				if (p->isPlayer)
					playerTurnDelay = p->getAttackDelay();
				//We attack
				meleeAttack(p, here);
			}
			else {
				//Adjust position and deal with the consequences
				p->setPosition(xnew, ynew);
				standOnTile(p);
				//If this is the player, update the FOV
				if (p->isPlayer) {
					currentMap->updateFOV(player->getx(), player->gety());
					//Time passes
					if (!player->hasFreeMoves())
						playerTurnDelay = p->getMoveDelay();
					else
						player->useFreeMove();
				}
			}
		}
		else {
			//If we try to step onto a non-walkable tile, try TOUCHING IT instead
			coord oldPos = p->getPosition();
			p->setPosition(xnew, ynew);
			standOnTile(p);
			p->setPosition(oldPos);
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
			pickUpItem(itemHere);
			//victim->addItem(itemHere);
			//...and remove it from the floor
			currentMap->removeItem(itemHere);
		}
	}
	//Tile effects
	maptile* mt = currentMap->getTile(victim->getx(), victim->gety());
	for (auto te : mt->getTouchEffects()) {
		applyEffectToPerson(victim, te, mt->getPotency());
	}
}

/*
Automatically unlocks ALL doors adjacent (within 1 space) of the given point
*/
void game::unlockAdjacentTiles(int cx, int cy)
{
	for (int x = cx - 1; x <= cx + 1; x++) {
		for (int y = cy - 1; y <= cy + 1; y++) {
			if (currentMap->inBounds(x, y)) {
				//Check tile here
				maptile* t = currentMap->getTile(x, y);
				if (t->isDoor && t->isDoorLocked()) {
					//Unlock door and update the map
					t->unlockDoor();
					currentMap->updateDatamapAtPoint(x, y);
					//Message about it
					addMessage("Door unlocked!", TCODColor::white);
				}
			}
		}
	}
}


/*
Check to see if we have the key for the given tile.
*/
void game::tryUnlockDoor(int x, int y)
{
	maptile* t = currentMap->getTile(x, y);
	if (t->isDoor && t->isDoorLocked()) {
		//Check for key
		if (player->hasKey(t->unlockCode)) {
			//Unlocked!
			t->unlockDoor();
			currentMap->updateDatamapAtPoint(x, y);
			addMessage("Unlocked " + t->getName() + ".", t->getColor());
		}
	}
}



/*
	INTER - MAP MOVEMENT
*/


/*
See if this move will take us to a new map
*/
void game::tryMapChange(int xnew, int ynew)
{
	//Figure out which direction we're stepping
	connectionPoint dr;
	if (xnew >= currentMap->getXSize())
		dr = CONNECT_EAST;
	else if (xnew < 0)
		dr = CONNECT_WEST;
	else if (ynew >= currentMap->getYSize())
		dr = CONNECT_SOUTH;
	else if (ynew < 0)
		dr = CONNECT_NORTH;
	else
		return;
	//Get handle of connecting map, should one exist
	std::string newHandle = currentMap->getConnection(dr);
	if (newHandle.size()) {
		loadMapFromHandle(newHandle, dr, xnew, ynew);
	}
}

/*
Try to load a map given its text handle.
*/
void game::loadMapFromHandle(std::string handle, connectionPoint connect, int oldx, int oldy)
{
	//Find corresponding map, should one exist
	map* newMap = getKnownMap(handle);
	if (newMap == nullptr)
		newMap = makemap.loadMapFromFile(handle);
	//Make sure we actually ended up with a map
	if (newMap != nullptr) {
		loadNewMap(newMap, connect, oldx, oldy);
		//Remember this map
		addKnownMap(newMap, handle);
	}
}

/*
Actually loads us onto the given new map.
The coordinates are our position on the old map, which define where we'll end up on the new one.
*/
void game::loadNewMap(map * newMap, connectionPoint connect, int oldx, int oldy)
{
	//Remove player from current map
	currentMap->removePerson(player);
	//Set new map
	setCurrentMap(newMap);
	//Respawn monsters on map
	newMap->respawnAllMonsters();
	//Figure our what our new coordinates will be, based on where we moved from
	int xnew = oldx;
	int ynew = oldy;
	if (connect == CONNECT_EAST)
		xnew = 0;
	else if (connect == CONNECT_WEST)
		xnew = newMap->getXSize() - 1;
	else if (connect == CONNECT_NORTH)
		ynew = newMap->getYSize() - 1;
	else if (connect == CONNECT_SOUTH)
		ynew = 0;
	else if (connect == CONNECT_WARP) {
		coord pt = newMap->getStartPoint();
		xnew = pt.first;
		ynew = pt.second;
	}
	//Add player to new map
	newMap->addPerson(player, xnew, ynew);
	//Set up new clock for the new map
	turns.clear();
	for (auto person : newMap->getAllPeople())
		if (!person->isPlayer)
			turns.addEntity(person, 1);
	//Update FOV for new map
	currentMap->updateFOV(player->getx(), player->gety());
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
		//Weapon base damage
		damage = wp->getDamage();
		//Damage scaling from stats, if any
		int scalingPercent = attacker->getScalingDamage(wp);
		damage += (float)scalingPercent / 100.0 * (float)damage;
	}

	//Damage buffs
	float dbuff = (float)attacker->scaleNextAttack / 100.0;
	damage += dbuff * (float)damage;
	if (attacker->scaleNextAttack)
		//This buff only lasts for 1 attack
		attacker->scaleNextAttack = 0;
	
	//Deal the damage
	target->takeDamage(damage, DAMAGE_PHYSICAL);
	
	//Next: SPELL DISCHARGES, if we have one readied
	if (attacker->buffNextMelee != nullptr) {
		dischargeSpellOnTarget(attacker->buffNextMelee, attacker, target);
		attacker->buffNextMelee = nullptr;
	}
	
	//Animation effect
	addAnimations(new flashCharacter(target, TCODColor::red));
	
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

	//If we're an AI, this expends all of our free moves
	if (!target->isPlayer && target->hasFreeMoves())
		target->clearFreeMoves();

}

/*
Target gets shoved away.
Currently ONLY AFFECTS NPCS and they are ALWAYS SHOVED AWAY FROM THE PLAYER
Yes, it's kinda clunky.
*/
void game::knockbackTarget(person * target, int distance)
{
	int xv = get1dVector(player->getx(), target->getx());
	int yv = get1dVector(player->gety(), target->gety());
	for (int i = 0; i < distance; i++) {
		movePerson(target, target->getx() + xv, target->gety() + yv);
	}
}


/*
	BOSS STUFF
*/



void game::setBoss(monster * m)
{
	currentBoss = m;
}


/*
Displays a special message when we kill a BOSS.
*/
void game::bossKillMessage()
{
	int atx = MAP_DRAW_X;
	int aty = MAP_DRAW_Y;
	win.clearRegion(atx - 1, aty, 42, 5);
	win.drawBox(atx - 1, aty, 42, 5, TCODColor::darkRed);
	aty++;
	win.write(atx, aty, centreText(currentBoss->getName(), 40), currentBoss->getColor());
	std::string txt2 = "has been destroyed. Well done, pilgrim!";
	win.write(atx, ++aty, centreText(txt2, 40), TCODColor::white);
	//Refresh and wait for input
	win.refresh();
	while (win.getkey().vk != KEY_ACCEPT) {}
}


/*
	INVENTORY MANAGEMENT
*/


/*
Display a special message when we pick up an item.
Returns whether we want to equip it or not!
*/
bool game::itemPickupMessage(item * it)
{
	int atx = MAP_DRAW_X;
	int aty = MAP_DRAW_Y + 10;
	win.clearRegion(atx - 1, aty, 42, 18);
	win.drawBox(atx - 1, aty, 42, 18, TCODColor::darkSepia);
	//What we can do
	std::string txt = "[ENTER] Equip  [ESC] Store";
	win.write(atx, ++aty, centreText(txt, 40), TCODColor::white);
	aty += 2;
	//Fill in with ITEM DEETS
	drawItemInfo(it, atx, aty + 1);
	//Wait for input: equip or no?
	win.refresh();
	TCOD_key_t kp = win.getkey();
	while (kp.vk != KEY_BACK_OUT) {
		if (kp.vk == KEY_ACCEPT)
			return true;
		kp = win.getkey();
	}
	return false;
}

/*
We get a new item - picked up off the floor, auto-dropped by a monster, or whatever
*/
void game::pickUpItem(item * it)
{
	//Get it
	bool stackedWithOther = player->addItem(it);
	//Message about it
	addMessage("Got " + it->getName() + "!", it->getColor());
	bool equipItem = itemPickupMessage(it);
	if (equipItem && !stackedWithOther)
		player->equipItem(it);
}

/*
Set up the MAIN MENU for inventory management, where you can select an inventory category.
*/
void game::createInventoryMenu()
{
	setState(STATE_VIEW_INVENTORY);
	//Pick an ITEM CATEGORY
	currentMenu = new menu("INVENTORY");
	for (auto cat : ALL_ITEM_TYPES) {
		item* e = new item(getItemCategoryName(cat), 0, TCODColor::lightGrey, cat);
		currentMenu->addElement(e);
	}
}

/*
New menu listing inventory items in this category
*/
void game::selectInventoryCategory(itemTypes cat)
{
	setState(STATE_VIEW_INVENTORY_CATEGORY);
	//Setup menu
	currentMenu = new menu(getItemCategoryName(cat));
	for (auto item : player->getItemsOfType(cat)) {
		currentMenu->addElement(item);
	}
}

/*
We get the items a dead monster dropped
*/
void game::getDeathDrops(monster * m)
{
	//Items
	for (auto it : m->getItemDrops()) {
		pickUpItem(it);
	}
	//Fragments
	fragments += m->getFragmentsDropped();
}


/*
Equip the selected item.
*/
void game::equipItem(item * it)
{
	player->equipItem(it);
}



/*
	SPELL MAGIC
*/


/*
Firing a ranged spell is... COMPLICATED
*/
void game::doRangedSpell(spell * sp)
{
	if (targetModeOn) {
		//Get a path to the target
		pathVector path = getLine(player->getPosition(), targetPt);
		//See if there's something to hit on the path
		person* target = getTargetOnPath(path);
		if (target != nullptr) {
			//Is it in range?
			int dist = hypot(player->getx() - target->getx(), player->gety() - target->gety());
			if (dist <= sp->getAttackRange()) {
				//We hit!
				dischargeSpellOnTarget(sp, player, target);
				//Bullet animation!
				addAnimations(new bulletPath(path, BULLET_TILE, sp->getColor()));
			}
			else {
				addMessage("Out of range!", TCODColor::white);
			}
		}
	}
	else {
		addMessage("First, turn on targeting mode!", TCODColor::white);
	}
}

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
		else if (aType == ATTACK_BUFF_SELF) {
			//Spell is applied to self
			dischargeSpellOnTarget(sp, player, player);
		}
		else if (aType == ATTACK_AOE) {
			//Hits everything within its radius
			doAOE(sp, player);
			//Time taken
			playerTurnDelay += player->getOffhand()->getAttackDelay();
		}
		else if (aType == ATTACK_RANGE) {
			//Hits target marked with the cursor
			doRangedSpell(sp);
			//Time taken is the attack delay of our OFFHAND item (which will usually be e.g. a wand)
			playerTurnDelay += player->getOffhand()->getAttackDelay();
		}
	}
}

/*
Spell actually affects the target
*/
void game::dischargeSpellOnTarget(spell * sp, person * caster, person * target)
{
	
	//No message if this is a self-buff
	if (caster != target)
		addMessage(caster->getName() + " hits " + target->getName() + " with " + sp->getName() + "!", sp->getColor());
	
	//Iterate through all effects
	for (int idx = 0; idx < sp->getEffectsCount(); idx++) {
		
		//Amount of damage (or whatever numeric value we require) the spell does
		int potency = sp->getEffectPotency(idx);
		
		if (sp->usesSpellPower) {
			//Sometimes scales with spell power
			int spellPower = caster->getSpellPower();
			//Check for temporary buff
			if (player->scaleNextSpell > 0) {
				spellPower += player->scaleNextSpell;
				player->scaleNextSpell = 0;
			}
			potency = (float)potency * ((float)spellPower / 100);
		}
		
		if (sp->usesDivinePower) {
			//Or with divine power
			int divPower = caster->getDivinePower();
			//Check for temporary buff
			if (player->scaleNextPrayer > 0) {
				divPower += player->scaleNextPrayer;
				player->scaleNextPrayer = 0;
			}
			potency = (float)potency * ((float)divPower / 100);
		}

		//Permanent buff or temporary?
		if (sp->addPermanentBuff) {
			//Permanent buffs need to be tracked
			if (target->addBuff(sp->getName(), sp->getColor(), sp->getEffectType(idx), potency)) {
				applyEffectToPerson(target, sp->getEffectType(idx), potency);
				//This is when the caster expends their vigour
				caster->loseVigour(sp->getVigourCost());
			}
			else {
				addMessage("You cannot stack this buff!", TCODColor::white);
			}
		}
		else {
			//Just a buff!
			applyEffectToPerson(target, sp->getEffectType(idx), potency);
			//This is when the caster expends their vigour
			caster->loseVigour(sp->getVigourCost());
		}
	
	}
}


/*
Pick which spell you want to select!
*/
void game::openSpellMenu()
{
	menu* sm = new menu("CHOOSE SPELL");
	for (auto sp : player->getSpellsKnown())
		sm->addElement(sp);
	currentMenu = sm;
	setState(STATE_SELECT_SPELL);
}

/*
Pick a spell off the menu, close the menu
*/
void game::selectSpellFromMenu()
{
	spell* sp = static_cast<spell*>(currentMenu->getSelectedItem());
	player->setCurrentSpell(sp);
	setState(STATE_VIEW_MAP);
}



/*
	LEVELING UP
*/


/*
Create the level-up menu.
*/
void game::setupLevelUpMenu()
{
	TCODColor statCol = TCODColor::lightGrey;
	menu* lmenu = new menu("LEVEL UP");
	lmenu->addElement("HEALTH", statCol);
	lmenu->addElement("VIGOUR", statCol);
	lmenu->addElement("STRENGTH", statCol);
	lmenu->addElement("DEXTERITY", statCol);
	lmenu->addElement("ARCANA", statCol);
	lmenu->addElement("DEVOTION", statCol);
	currentMenu = lmenu;
	setState(STATE_LEVEL_UP_MENU);
}


/*
Level up selected stat.
*/
void game::doLevelUp()
{
	element* e = currentMenu->getSelectedItem();
	std::string name = e->getName();
	//Can we afford it?
	if (fragments >= player->getNextLevelCost()) {
		fragments -= player->getNextLevelCost();
		//Buff selected stat
		if (name == "HEALTH")
			player->stats->health++;
		else if (name == "VIGOUR")
			player->stats->vigour++;
		else if (name == "STRENGTH")
			player->stats->strength++;
		else if (name == "DEXTERITY")
			player->stats->dexterity++;
		else if (name == "ARCANA")
			player->stats->arcana++;
		else if (name == "DEVOTION")
			player->stats->devotion++;
		//Increase overall level
		player->stats->level++;
		//Adjust stats that increase passively
		int maxHealth = 90 + 10 * player->stats->health;
		int maxVigour = 8 + 2 * player->stats->vigour;
		player->setMaxHealth(maxHealth);
		player->setMaxVigour(maxVigour);
	}
}

void game::drawLevelUpMenu(int atx, int aty)
{
	//Stats menu
	drawMenu(currentMenu, atx, aty);
	//Stat levels
	TCODColor statCol = TCODColor::green;
	int offset = 12;
	win.write(atx + offset, ++aty, std::to_string(player->stats->health), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->vigour), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->strength), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->dexterity), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->arcana), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->devotion), statCol);
	//How much the next level COSTS
	aty += 2;
	win.write(atx, aty, "Requires", TCODColor::white);
	win.writec(atx + 9, aty, FRAGMENT_GLYPH, TCODColor::amber);
	//Colour indicates whether we have enough
	TCODColor col = TCODColor::red;
	if (fragments >= player->getNextLevelCost())
		col = TCODColor::green;
	//Show it
	win.write(atx + 10, aty, std::to_string(player->getNextLevelCost()), col);
}


/*
	SHOPPING
*/


/*
Displaying the shopping times!
*/
void game::drawShopMenu(int atx, int aty)
{
	//The SHOPPING LIST
	drawMenu(currentMenu, atx, aty);
	//Description of items for purchase
	aty += currentMenu->getAllElements().size() + 10;
	item* sel = static_cast<item*>(currentMenu->getSelectedItem());
	if (sel != nullptr)
		drawItemInfo(sel, atx, aty);
}

/*
Look for a nearby shopkeeper that we can chat with.
*/
void game::talkToShopkeeper()
{
	int r = 2;
	for (int x = player->getx() - r; x <= player->getx() + r; x++) 
	{
		for (int y = player->gety() - r; y <= player->gety() + r; y++) 
		{
			person* target = currentMap->getPerson(x, y);
			if (target != nullptr) 
			{
				if (target->isShopkeeper) 
				{
					setupShopMenu(target);
					return;
				}
			}
		}
	}
}


/*
Create menu for SHOPPING!
*/
void game::setupShopMenu(person * shopkeeper)
{
	monster* m = static_cast<monster*>(shopkeeper);
	currentMenu = new menu(shopkeeper->getName());
	for (auto it : m->getStock()) {
		currentMenu->addElement(it);
	}
	//We keep track of who the shopkeeper is
	currentShopkeeper = m;
	setState(STATE_SHOP_MENU);
}


/*
Try to buy selected menu item
*/
void game::buyItemFromShop()
{
	item* it = static_cast<item*>(currentMenu->getSelectedItem());
	if (it != nullptr) {
		if (fragments >= it->getPrice()) {
			//Pay the COST
			fragments -= it->getPrice();
			//But get the ITEM
			pickUpItem(it);
			//Remove from menu and shopkeeper's inventory
			currentShopkeeper->removeItemFromStock(it);
			currentMenu->removeElement(it);
		}
	}
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
	//If the player is dead, SPECIAL STUFF happens
	if (player->isDead)
		restoreFromSavePoint();
	else {
		
		//Otherwise, check for dead NPCs
		personVector toClear;
		//Find everyone who's dead
		for (auto p : currentMap->getAllPeople()) {
			if (p->isDead)
				toClear.push_back(p);
		}
		
		//And remove them!
		for (auto p : toClear) {
			currentMap->removePerson(p);
			//If this was the BOSS, then this BOSS FIGHT is over
			if (p == currentBoss) {
				bossKillMessage();
				currentBoss = nullptr;
				//Toggle setting on map so the boss won't respawn
				currentMap->bossDestroyed = true;
			}
			//And the PLAYER gets the ITEMS we drop!
			getDeathDrops(static_cast<monster*>(p));
		}

	}
}



/*
	SAVE POINTS
*/


/*
Sets save point to our current position.
*/
void game::setSavePoint()
{
	ourSavePt.saveMap = currentMap;
	ourSavePt.savePt = player->getPosition();
}

/*
Returns player to save point.
*/
void game::restoreFromSavePoint()
{
	//Resurrect player
	deletePlayerBuffs();
	player->fullRestore();
	player->isDead = false;
	player->setTarget(nullptr);
	//Return us to our save point
	loadNewMap(ourSavePt.saveMap, CONNECT_VERTICAL, ourSavePt.savePt.first, ourSavePt.savePt.second);
	//Put us back in the clock
	turns.addEntity(player, 0);
}


/*
Buffs are fully reset.
*/
void game::deletePlayerBuffs()
{
	for (auto b : player->getAllBuffs()) {
		//Negative buff is applied!
		applyEffectToPerson(player, b->effectApplied, -b->potency);
	}
}




/*
	COORDINATE CONVERSATIONS
*/



/*
Translates a coordinate on the window into a coordinate on the map.
If the cursor is outside the map, will return a point outside the map bounds!
*/
coord game::screenToMapCoords(coord pt)
{
	coord mpt;
	mpt.first = pt.first - MAP_DRAW_X;
	mpt.second = pt.second - MAP_DRAW_Y;
	return mpt;
}




/*
	DEBUGGING
*/


/*
FOR DEBUGGING, IDIOT
*/
void game::debugMenu()
{
	std::string txt = win.getstr(1, 1);

	//WHAT DEBUG DO WE WANT TO DO?!
	if (txt == "map") {
		txt = win.getstr(1, 1);
		//Load map by NAME
		txt = makemap.getMapHandle(txt);
		loadMapFromHandle(txt, CONNECT_WARP, player->getx(), player->gety());
	}

	//Set up special game states
	else if (txt == "warpboss1") {
		player->addItem(weapon_SplinteredSword());
		player->addItem(armour_RuinedUniform());
		player->addItem(headgear_CaptainsTricorn());
		player->addItem(spell_MagicMissile());
		player->addItem(spell_ArcaneRadiance());
		player->addItem(wand_DriftwoodWand());
		player->addItem(shield_BatteredWoodenShield());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		loadMapFromHandle("maps/cbeach_2.txt", CONNECT_WARP, player->getx(), player->gety());
	}
	else if (txt == "wretch") {
		player->addItem(weapon_SplinteredSword());
		player->addItem(armour_RuinedUniform());
		player->addItem(headgear_CaptainsTricorn());
		player->addItem(spell_MagicMissile());
		player->addItem(spell_ArcaneRadiance());
		player->addItem(wand_DriftwoodWand());
		player->addItem(shield_BatteredWoodenShield());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		loadMapFromHandle("maps/wretch_cave.txt", CONNECT_WARP, player->getx(), player->gety());
	}
	else if (txt == "fairweather") {
		player->addItem(weapon_SplinteredSword());
		player->addItem(weapon_ThinKnife());
		player->addItem(weapon_StraightSword());
		player->addItem(weapon_Warhammer());
		player->addItem(armour_RuinedUniform());
		player->addItem(headgear_CaptainsTricorn());
		player->addItem(spell_MagicMissile());
		player->addItem(spell_ArcaneRadiance());
		player->addItem(wand_DriftwoodWand());
		player->addItem(ranged_ThrowingKnives());
		player->addItem(shield_BatteredWoodenShield());
		player->addItem(chime_ClericsCrackedChime());
		player->addItem(prayer_Restoration());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		fragments += 320;
		loadMapFromHandle("maps/old_fairweather.txt", CONNECT_WARP, player->getx(), player->gety());
	}
	else if (txt == "oldcrow") {
		player->addItem(weapon_SplinteredSword());
		player->addItem(weapon_ThinKnife());
		player->addItem(weapon_StraightSword());
		player->addItem(weapon_Warhammer());
		player->addItem(armour_RuinedUniform());
		player->addItem(headgear_CaptainsTricorn());
		player->addItem(armour_RuinedKnightsArmour());
		player->addItem(headgear_RuinedKnightsHelm());
		player->addItem(headgear_CrowKnightsHood());
		player->addItem(armour_CrowKnightsArmour());
		player->addItem(spell_MagicMissile());
		player->addItem(spell_ArcaneRadiance());
		player->addItem(wand_DriftwoodWand());
		player->addItem(ranged_ThrowingKnives());
		player->addItem(ranged_LaceratingKnives());
		player->addItem(shield_BatteredWoodenShield());
		player->addItem(chime_ClericsCrackedChime());
		player->addItem(prayer_Restoration());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		player->addItem(consumable_StarwaterDraught());
		fragments += 500;
		loadMapFromHandle("maps/pilgrims_road_5.txt", CONNECT_WARP, player->getx(), player->gety());
	}
}
