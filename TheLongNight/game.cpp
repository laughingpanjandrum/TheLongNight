#include "game.h"



const std::string game::VOID_EDGE_MAP = "maps/void_edge.txt";
const std::string game::VOID_RETURN_MAP = "maps/sordid_chapel.txt";


game::game()
{
	
	//We start by creating an empty map, just for now
	map* newmap = new map();
	newmap = makemap.loadMapFromFile(makemap.getStartMapHandle());
	setCurrentMap(newmap);
	newmap->respawnAllMonsters(storyEventsReady);
	
	//Remember the first map
	addKnownMap(newmap, makemap.getStartMapHandle());

	//Load in story events
	loadStoryEvents("maps/storyFlags.txt");
	loadTextDumps("dialogue/journalText.txt");

	//Set up store inventories
	initializeShops();
	
	//Character create
	player = personSharedPtr (new person());
	player->isPlayer = true;
	player->isHostile = false;
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
		//Events
		TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key, &mouse);
		//Process player action, unless AUTO-WALKING
		if (isAutoWalking)
			playerAutoWalk();
		else {
			processCommand();
			processMouseClick();
		}
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
	personSharedPtr target = currentMap->getPerson(xnew, ynew);
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
pathSharedPtr game::getPathToCoord(coord startxy, coord endxy)
{
	TCODPath *path = currentMap->getPath();
	path->compute(startxy.first, startxy.second, endxy.first, endxy.second);
	return pathSharedPtr(path);
}


/*
Builds a vector of x,y coordinate pairs in a straight line from the start point to the endpoint.
*/
coordVectorSharedPtr game::getLine(coord startxy, coord endxy)
{
	coordVectorSharedPtr path = coordVectorSharedPtr(new coordVector());
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
		path->push_back(thisPt);
	}
	//Done
	return path;
}

/*
Returns the first person we find walking along the given path vector.
*/
personSharedPtr game::getTargetOnPath(coordVectorSharedPtr path)
{
	for (auto pt : *path) {
		if (currentMap->isWalkable(pt.first, pt.second)) {
			personSharedPtr target = currentMap->getPerson(pt.first, pt.second);
			if (target != nullptr)
				return target;
		}
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
	//Make sure dead things are removed from the map
	clearDeadCreatures();
	//Add player to turn tracker
	turns.addEntity(player, playerTurnDelay);
	playerTurnDelay = 0;
	//Cycle through all turns
	personSharedPtr nextTurn = turns.getNext();
	while (nextTurn != player) {
		doMonsterTurn(nextTurn);
		nextTurn = turns.getNext();
	}
}


/*
	AI ACTIONS
*/

/*
Returns a list of all adjacent coords that the given ai will willingly walk upon.
*/
pathVector game::getAllAdjacentWalkable(monsterSharedPtr ai)
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
bool game::aiIsValidMove(monsterSharedPtr ai, int xnew, int ynew)
{
	
	//Make sure it's in bounds
	if (!currentMap->inBounds(xnew, ynew))
		return false;
	else if (!currentMap->isWalkable(xnew, ynew))
		return false;
	
	//Make sure we won't attack a friendly
	personSharedPtr here = currentMap->getPerson(xnew, ynew);
	if (here != nullptr && !here->isPlayer) {
		if (!ai->canSwapWithAllies)
			return false;
	}
	
	//Otherwise, we're good to go!
	return true;
}

/*
AI moves directly towards its target, melee-attacking if possible.
Returns whether we're done moving.
We might not be done if we get multiple moves in one turn!
*/
bool game::aiMoveToTarget(monsterSharedPtr ai)
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
	personSharedPtr t = ai->getTarget();
	coord bestPt = pts.at(0);
	int bestDist = hypot(bestPt.first - t->getx(), bestPt.second - t->gety());

	//Look for the shortest distance (or the greatest distance, if we're the timid type)
	for (int i = 1; i < pts.size(); i++) {
		coord pt = pts.at(i);
		int newDist = hypot(pt.first - t->getx(), pt.second - t->gety());

		//Figure out what we want - to get closer or further away
		bool cond;
		if ((ai->fear > 0) || (ai->keepsDistance && !ai->hasFreeMoves() && randint(1, 3) == 1))
			cond = newDist > bestDist;
		else
			cond = newDist < bestDist;
		//Now see if the new point is better according to our condition
		if (cond) {
			bestDist = newDist;
			bestPt = pt;
		}
	}

	//We might need to remember our prior location
	int oldx = ai->getx();
	int oldy = ai->gety();

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
Returns whether we want to cast a given spell.
*/
bool game::aiShouldCastSpell(monsterSharedPtr ai, spellSharedPtr sp)
{
	//We're good to go!
	return true;
}

/*
See if we want to cast any of our spells.
*/
bool game::aiTryUseSpell(monsterSharedPtr ai)
{

	//Possible fog emission!
	if (ai->emitsFog > 0)
		currentMap->createFogCloud(ai->getx(), ai->gety(), ai->emitsFog);

	//If silenced, can't cast at all
	if (ai->silence > 0)
		return false;
	
	//Random chance to not cast a spell
	int r = randint(1, 100);
	if (r > ai->getSpellCastChance())
		return false;
	
	//Sort through all our spells and see if any of them look likely
	personSharedPtr target = ai->getTarget();
	
	for (auto sp : ai->getSpellsKnown()) {

		if (aiShouldCastSpell(ai, sp)) {

			attackType aType = sp->getAttackType();
			if (aType == ATTACK_RANGE) {

				//Ranged spell - do we have the reach for it?
				coordVectorSharedPtr path = coordVectorSharedPtr(getLine(ai->getPosition(), target->getPosition()));
				personSharedPtr willHit = getTargetOnPath(path);

				if (willHit == target && canPlayerSeePoint(ai->getx(), ai->gety())) {
					//Check spell range
					int dist = hypot(ai->getx() - target->getx(), ai->gety() - target->gety());
					if (dist <= sp->getAttackRange()) {
						
						//ANIMATION BLAST!
						spellTitleAnimation(ai, sp);
						coordVectorSharedPtr path = coordVectorSharedPtr(getLine(ai->getPosition(), target->getPosition()));
						/*if (sp->useAlternateAnimation) {
							addAnimations(new glowPath(path, sp->getColor(), TCODColor::white));
						}
						else {
							addAnimations(new bulletPath(path, BULLET_TILE, sp->getColor()));
						}*/
						
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
					//ANIMATION BLAST!
					spellTitleAnimation(ai, sp);
					//Do the AOE!
					doAOE(sp, ai);
					turns.addEntity(ai, ai->getAttackDelay());
					return true;
				}

			}

			else if (aType == ATTACK_BUFF_SELF) {

				//ANIMATION BLAST!
				spellTitleAnimation(ai, sp);

				//Cast spell on self - usually a good idea!
				dischargeSpellOnTarget(sp, ai, ai);

			}

		}
	}
	
	//We didn't use any abilities
	return false;
}

/*
Try to spawn something nearby!
*/
void game::aiSpawnCreature(monsterSharedPtr ai)
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
		monsterSharedPtr m = ai->getRandomSpawn();
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
void game::aiDoCombatAction(monsterSharedPtr ai)
{
	//First see if we want to spawn something!
	if (ai->canSpawnCreatures() && ai->wantsToSpawn())
		aiSpawnCreature(ai);
	//If not, try casting a spell. If that fails, move towards our target.
	else if (!aiTryUseSpell(ai) && !ai->immobile)
		//We keep moving until the move function says we're done.
		while (!aiMoveToTarget(ai)) {}
}



/*
AI tries to find something to kill.
*/
void game::aiFindTarget(monsterSharedPtr ai)
{
	//If the player can see us, we can see them. ONE SIMPLE RULE.
	if (canPlayerSeePoint(ai->getx(), ai->gety())) {
		int dist = hypot(player->getx() - ai->getx(), player->gety() - ai->gety());
		if (dist <= player->detectionRange) {
			ai->setTarget(player);
			//If we are a BOSS, it looks like a BOSS FIGHT just started
			if (ai->isBoss)
				setBoss(ai);
		}
	}
}



/*
Monster does its turn and then is placed back into the turn tracker.
*/
void game::doMonsterTurn(personSharedPtr p)
{
	monsterSharedPtr ai = std::static_pointer_cast<monster>(p);
	//If we're dead, we probably shouldn't do anything
	if (ai->isDead)
		return;
	//Likewise, if we're non-hostile, we don't do anything
	if (!ai->isHostile)
		return;
	//If we're blind, we skip our turn
	if (ai->isBlind()) {
		turns.addEntity(ai, SPEED_NORMAL);
		return;
	}
	//Do we have a target?
	personSharedPtr target = ai->getTarget();
	if (target == nullptr)
		aiFindTarget(ai);
	//If we have one, just move towards it
	if (target != nullptr) {
		aiDoCombatAction(ai);
	}
	else {
		//If not, just wait
		turns.addEntity(ai, 1);
	}
}



/*
	MENUS
*/


/*
Just displays a menu! It's pretty simple.
*/
void game::drawMenu(menu * m, int atx, int aty)
{

	//Box it in
	int height = m->getAllElements().size() + 4;
	win.clearRegion(atx - 1, aty - 1, 46, height);
	win.drawBox(atx - 1, aty - 1, 46, height, TCODColor::darkSepia);

	//Title
	win.write(atx, aty++, m->getTitle(), TCODColor::white);

	//Elements
	auto elements = m->getAllElements();
	for (auto it = elements.begin(); it != elements.end(); it++) 
	{

		//SHOW NAME and tile
		win.write(atx + 4, ++aty, (*it)->getMenuName(), (*it)->getColor());
		win.writec(atx + 3, aty, (*it)->getTileCode(), (*it)->getColor());

		//Mouse highlighting?!
		if (mouse.cx <= (atx + (*it)->getName().size()) && mouse.cx >= atx && mouse.cy == aty) {
			//Mouse over!
			//win.writec(atx + 1, aty, '>', TCODColor::white);
			m->setCurrentElement(*it);
		}
		//Indicate whether this is the highlighted element
		
		if (m->getSelectedItem() == (*it)) {
			win.writec(atx + 1, aty, '>', TCODColor::white);
		}

		//Special context-sensitive attributes
		if (state == STATE_VIEW_INVENTORY_CATEGORY) {
			if (player->hasItemEquipped(std::static_pointer_cast<item>(*it)))
				win.writec(atx + 2, aty, '#', TCODColor::white);
		}

	}
}

/*
Change menu selection
*/
void game::navigateMenu(TCOD_key_t kp)
{
	if (kp.c == 'w')
		currentMenu->scrollUp();
	else
		currentMenu->scrollDown();
}

/*
Press ENTER in a menu
*/
void game::acceptCurrentMenuIndex()
{
	//What this does depends on which MENU we're in
	if (state == STATE_VIEW_INVENTORY) {
		//Selected menu object should be an ITEM
		itemSharedPtr sel = std::static_pointer_cast<item>(currentMenu->getSelectedItem());
		selectInventoryCategory(sel->getCategory());
	}
	else if (state == STATE_VIEW_INVENTORY_CATEGORY) {
		//Equip the selected item
		itemSharedPtr sel = std::static_pointer_cast<item>(currentMenu->getSelectedItem());
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
	else if (state == STATE_WARP)
		doWarp(currentMenu->getSelectedItem()->getName());
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
void game::drawScreen(bool doRefresh) 
{
	win.clear();
	
	//Always draw the interface, except with our inventory!
	if (state != STATE_VIEW_INVENTORY_CATEGORY)
		drawInterface(MAP_DRAW_X + 47, MAP_DRAW_Y);
	
	//Figure out else what to draw
	if (state == STATE_VIEW_INVENTORY || state == STATE_VIEW_INVENTORY_CATEGORY)
		drawInventory(MAP_DRAW_X, MAP_DRAW_Y);
	else if (state == STATE_LEVEL_UP_MENU)
		drawLevelUpMenu(MAP_DRAW_X, MAP_DRAW_Y);
	else if (state == STATE_SHOP_MENU)
		drawShopMenu(MAP_DRAW_X, MAP_DRAW_Y);
	else if (state == STATE_WARP)
		drawMenu(currentMenu, MAP_DRAW_X, MAP_DRAW_Y);
	else
		drawMap(MAP_DRAW_X, MAP_DRAW_Y);

	
	//win.drawFont();
	if (doRefresh)
		win.refresh();
}

/*
Just draws the current map.
Input: Coordinates to start drawing at (the top left corner of the map)
*/
void game::drawMap(int leftx, int topy)
{

	//Box it in
	win.drawBox(leftx - 1, topy - 2, 41, 43, TCODColor::darkSepia);
	
	//Draw map name just above
	win.write(leftx, topy - 1, centreText(currentMap->getName(), 38), TCODColor::white);
	
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
Returns adjustment to light level at the given point based on the presence of nearby light-emitters.
*/
float game::getLightEmitters(int x, int y)
{
	float mod = 0.0f;
	for (auto p : currentMap->getAllPeople()) {
		if (p->getLightEmitted() != 0) {
			float dist = hypot(x - p->getx(), y - p->gety());
			mod += p->getLightEmitted() / (dist / 2);
		}
	}
	return mod;
}


/*
Returns whether the given point is visible to the player.
*/
bool game::canPlayerSeePoint(int x, int y)
{
	//If we're in a fog zone, we can only see other fog zones.
	if (currentMap->isFoggy(player->getx(), player->gety()))
		if (!currentMap->isFoggy(x, y))
			return false;
	//Otherwise, it's just the map's FOV.
	return currentMap->isPointInFOV(x, y);
}

/*
Returns what to draw at the given point.
*/
drawData game::getDrawData(int x, int y)
{
	
	//Basic data comes from the map tile
	maptile* m = currentMap->getTile(x, y);
	drawDataSharedPtr toDraw( new drawData(m->getTileCode(), m->getColor(), m->getBgColor()) );
	
	//If this point isn't visible, don't draw it!
	if (!canPlayerSeePoint(x, y) || player->isBlind()) {
		
		//What to draw if out of FOV
		if (currentMap->inMemoryMap(x, y)) {
			toDraw->color = win.mixColors(toDraw->color, TCODColor::black, 0.9);
			toDraw->bgcolor = win.mixColors(toDraw->bgcolor, TCODColor::black, 0.9);
		}
		else {
			toDraw->tileCode = EMPTY_TILE;
			toDraw->color = TCODColor::black;
			toDraw->bgcolor = TCODColor::black;
		}
	
	}
	else {
		
		//If we can see it, add it to the memory map
		currentMap->addToMemoryMap(x, y);
		
		//Is there a player here?
		personSharedPtr p = currentMap->getPerson(x, y);
		
		if (p != nullptr) {
			//Invisible? IF SO, CANNOT SEE PERSON!
			if (p->invisibility < 1) {
				toDraw->tileCode = p->getTileCode();
				toDraw->color = p->getColor();
			}
		}
		
		else {
			//Is there an item here?
			itemSharedPtr it = currentMap->getItem(x, y);
			if (it != nullptr) {
				toDraw->tileCode = it->getTileCode();
				toDraw->color = it->getColor();
			}
		}

		//Fog effects
		if (currentMap->isFoggy(x, y)) {
			//Whiten front, and background colour is fully white
			toDraw->bgcolor = TCODColor::lightestGrey;
			toDraw->color.scaleHSV(0.2, 1.0);
		}
		
		//Darken tiles that are further away
		int distance = hypot(x - player->getx(), y - player->gety());
		//Basic adjustment
		float modifier = 1.0 - distance * 0.07;
		//Additional darkness adjustment
		modifier -= currentMap->getDarknessAdjustment();
		modifier += getLightEmitters(x, y);
		//Check for nearby light-emitters
		//Cap values
		if (modifier > 0.9)
			modifier = 0.9;
		else if (modifier < 0.1)
			modifier = 0.1;
		//Scale lighting appropriately
		toDraw->color.scaleHSV(1.0, modifier);
		toDraw->bgcolor.scaleHSV(1.0, modifier);
	
	}

	//Allow animations to adjust draw data
	getAnimationDataOverride(toDraw, x, y);

	//Highlight cursor position
	if (targetModeOn && x == targetPt.first && y == targetPt.second)
		toDraw->bgcolor = TCODColor::pink;
	
	//Done! Return it all
	return *toDraw;

}

/*
Draws everything that's not the map!
*/
void game::drawInterface(int leftx, int topy)
{
	
	int atx = leftx;
	int aty = topy;

	//Box it in
	win.drawBox(atx - 1, aty - 1, 50, 30, TCODColor::darkSepia);

	//Messages
	int my = ITEM_DRAW_Y + 3;
	for (auto m : messages) {
		win.write(ITEM_DRAW_X, my++, m.txt, m.color);
	}
	
	//Health
	win.drawCounter(player->getHealth(), "LIFE", atx, aty, TCODColor::darkRed, TCODColor::darkGrey, 20);
	win.write(atx + 4, ++aty, player->getHealth().getAsString(), TCODColor::darkRed);
	
	//Vigour
	win.drawCounter(player->getVigour(), "VIG ", atx, ++aty, TCODColor::darkGreen, TCODColor::darkGrey, 20);
	win.write(atx + 4, ++aty, player->getVigour().getAsString(), TCODColor::darkGreen);
	
	//	Equipment
	
	//Weapons
	weaponSharedPtr wp = player->getWeapon();
	if (wp != nullptr) {
		//Weapon
		win.writec(atx, ++aty, wp->getTileCode(), wp->getColor());
		win.write(atx + 2, aty, wp->getMenuName(), wp->getColor());
		//Check mouseover
		if (isMouseOver(atx, wp->getMenuName().size(), aty))
			drawItemInfo(wp, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}
	else
		win.write(atx + 2, ++aty, "no weapon", TCODColor::darkGrey);
	
	//Offhand item
	weaponSharedPtr of = player->getOffhand();
	if (of != nullptr) {
		win.writec(atx, ++aty, of->getTileCode(), of->getColor());
		win.write(atx + 2, aty, of->getMenuName(), of->getColor());
		if (isMouseOver(atx, of->getMenuName().size(), aty))
			drawItemInfo(wp, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}
	
	//Helmet
	armourSharedPtr helm = player->getHelmet();
	if (helm != nullptr) {
		win.writec(atx, ++aty, helm->getTileCode(), helm->getColor());
		win.write(atx + 2, aty, helm->getMenuName(), helm->getColor());
		if (isMouseOver(atx, helm->getMenuName().size(), aty))
			drawItemInfo(helm, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}
	else
		win.write(atx + 2, ++aty, "no helmet", TCODColor::darkGrey);
	
	//Armour
	armourSharedPtr ar = player->getArmour();
	if (ar != nullptr) {
		win.writec(atx, ++aty, ar->getTileCode(), ar->getColor());
		win.write(atx + 2, aty, ar->getMenuName(), ar->getColor());
		if (isMouseOver(atx, ar->getMenuName().size(), aty))
			drawItemInfo(ar, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}
	else
		win.write(atx + 2, ++aty, "no armour", TCODColor::darkGrey);

	//Charms
	charmVector chList = player->getCharms();
	for (auto ch : chList) {
		win.writec(atx, ++aty, ch->getTileCode(), ch->getColor());
		win.write(atx + 2, aty, ch->getMenuName(), ch->getColor());
		if (isMouseOver(atx, ch->getMenuName().size(), aty))
			drawItemInfo(ch, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}

	aty++;
	
	//Consumable selected, or menu if we have that open
	if (state == STATE_SELECT_CONSUMABLE) {
		drawMenu(currentMenu, atx, ++aty);
		aty += currentMenu->getAllElements().size() + 3;
	}
	else {
		//Current consumable
		consumableSharedPtr c = player->getSelectedConsumable();
		if (c != nullptr) {
			//It exists
			win.writec(atx, ++aty, c->getTileCode(), c->getColor());
			win.write(atx + 2, aty, c->getMenuName(), c->getColor());
			//Info about it, if we're touching it
			if (mouse.cx >= atx && mouse.cx <= atx + c->getMenuName().size() && mouse.cy == aty)
				drawItemInfo(c, ITEM_DRAW_X, ITEM_DRAW_Y, false);
		}
		else
			win.write(atx + 2, ++aty, "no consumable", TCODColor::darkGrey);
	}
	
	//List known spells
	int s = 1;
	for (auto sp : player->getSpellsKnown()) {
		//Spell numeral
		win.write(atx + 1, ++aty, '[' + std::to_string(s) + ']', TCODColor::white);
		//Vigour cost
		win.writec(atx + 5, aty, VIGOUR_GLYPH, TCODColor::green);
		win.write(atx + 6, aty, std::to_string(sp->getVigourCost()), TCODColor::green);
		//Spell name
		win.write(atx + 8, aty, sp->getName(), sp->getColor());
		//Indicate whether this is the selected spell
		if (sp == player->getCurrentSpell())
			win.writec(atx, aty, '>', TCODColor::white);
		s++;
		//Show description if our mouse is hovering over this spell
		if (mouse.cx > atx && mouse.cx < atx + sp->getName().size() && mouse.cy == aty)
			drawItemInfo(sp, ITEM_DRAW_X, ITEM_DRAW_Y, false);
	}

	//Money
	win.writec(atx, ++aty, FRAGMENT_GLYPH, TCODColor::amber);
	win.write(atx + 2, aty, std::to_string(fragments), TCODColor::lightAmber);
	
	//Buffs
	if (player->hasFreeMoves()) {
		win.write(atx, ++aty, "CHARGE!", TCODColor::yellow);
	}
	
	//Status effects
	aty = listStatusEffects(player, atx, aty);

	//Infusions
	if (player->spellAcidInfusion > 0)
		win.write(atx, ++aty, "Infusion: ACID " + std::to_string(player->spellAcidInfusion), TCODColor::lime);
	if (player->spellColdInfusion > 0)
		win.write(atx, ++aty, "Infusion: COLD " + std::to_string(player->spellColdInfusion), TCODColor::cyan);

	//Draws whatever we have HIGHLIGHTED
	aty += 2;
	drawMouseover(atx, aty);

	//List controls
	atx += 15;
	aty = MAP_DRAW_Y + 43;
	win.writec(atx, ++aty, 'c', TCODColor::green);
	win.write(atx + 2, aty, "Select consumable", TCODColor::white);
	win.writec(atx, ++aty, 't', TCODColor::green);
	win.write(atx + 2, aty, "Toggle targeting mode", TCODColor::white);
	win.writec(atx, ++aty, 'f', TCODColor::green);
	win.write(atx + 2, aty, "Switch to secondary weapon", TCODColor::white);
	win.writec(atx, ++aty, 'q', TCODColor::green);
	win.write(atx + 2, aty, "Cast current spell", TCODColor::white);
	win.writec(atx, ++aty, 'e', TCODColor::green);
	win.write(atx + 2, aty, "Use current consumable", TCODColor::white);
	win.writec(atx, ++aty, 'h', TCODColor::green);
	win.write(atx + 2, aty, "Talk to friendly NPC", TCODColor::white);

	//BOSS HEALTH BAR, if we're fighting a boss
	atx = MAP_DRAW_X;
	aty = MAP_DRAW_Y + 42;
	if (currentBoss != nullptr && currentBoss->showBossHealthBar) {
		win.write(atx, aty, currentBoss->getName(), currentBoss->getColor());
		win.drawCounter(currentBoss->getHealth(), "", atx, aty + 1, TCODColor::red, TCODColor::darkGrey, 40);
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
			itemSharedPtr sel = std::static_pointer_cast<item>(currentMenu->getSelectedItem());

			//Draw the item and the image for the item
			drawItemInfo(sel, ITEM_DRAW_X, ITEM_DRAW_Y);
		
		}
	}
	
	else {
		
		//We can go to the level-up menu from here, & also save le game
		win.write(atx, aty + 20, "Press [p] to LEVEL UP", TCODColor::white);
		win.write(atx, aty + 21, "Press [s] to SAVE THE GAME", TCODColor::white);
	
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

	//Poison resist
	win.write(atx, ++aty, "POIS Resist", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getSpecialEffectBuildup(EFFECT_POISON)->getMaxValue()), TCODColor::lime);

	//Plague resist
	win.write(atx, ++aty, "PLAGUE Resist", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getSpecialEffectBuildup(EFFECT_PLAGUE)->getMaxValue()), TCODColor::sepia);

	aty++;

	//Magic stuff
	win.write(atx, ++aty, "SPELL POWER", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getSpellPower()), TCODColor::magenta);
	win.write(atx, ++aty, "DIVINE POWER", mainCol);
	win.write(atx + offset, aty, std::to_string(player->getDivinePower()), TCODColor::darkYellow);
	aty++;

	//Attacks/movement
	win.write(atx, ++aty, "Move Speed", mainCol);
	win.write(atx + offset, aty, getAttackSpeedName(player->getMoveDelay()), TCODColor::white);
	win.write(atx, ++aty, "Attack Speed", mainCol);
	win.write(atx + offset, aty, getAttackSpeedName(player->getAttackDelay()), TCODColor::white);

	//Damage dealt
	win.write(atx, ++aty, "Melee DMG", mainCol);
	std::string txt = std::to_string(player->getMeleeDamage());
	win.write(atx + offset, aty, txt, TCODColor::darkRed);
	int dx = atx + offset + txt.size();
	//Additional damage types dealt
	for (int dt = 0; dt != ALL_DAMAGE_TYPES; dt++) {
		damageType dtype = static_cast<damageType>(dt);
		int dmg = player->getDamageOfType(dtype);
		if (dmg > 0) {
			txt = '+' + std::to_string(dmg);
			win.write(dx, aty, txt, getDamageTypeColor(dtype));
			dx += txt.size();
		}
	}

	//List buffs
	aty++;
	for (auto b : player->getAllBuffs()) {
		win.write(atx, ++aty, b->name, b->color);
	}

	//Done! Wait for input
	win.refresh();
	while (win.getkey().vk != KEY_BACK_OUT) {}
}


/*
Display detailed stats on a monster.
*/
void game::drawMonsterInfo(monsterSharedPtr m, int atx, int aty)
{

	//Box it in!
	TCODColor maincol = TCODColor::white;
	int offset = 12;
	int w = 41;
	int h = 41;
	win.clearRegion(atx, aty, w, h);
	win.drawBox(atx, aty, w, h, TCODColor::sepia);

	//Monster name + description
	atx += 1;
	win.write(atx, ++aty, m->getName(), m->getColor());
	aty = win.writeWrapped(atx, ++aty, 35, m->description, TCODColor::lighterGrey);
	aty += 2;

	//Movement
	win.write(atx, ++aty, "Move Rate", maincol);
	win.write(atx + offset, aty, getAttackSpeedName(m->getMoveDelay()), TCODColor::orange);
	aty++;

	//Melee stats
	win.write(atx, ++aty, "Melee Rate", maincol);
	win.write(atx + offset, aty, getAttackSpeedName(m->getAttackDelay()), TCODColor::orange);
	win.write(atx, ++aty, "Melee DMG", maincol);
	win.write(atx + offset, aty, std::to_string(m->getMeleeDamage()), TCODColor::red);

	//Special damage types
	for (int dt = 0; dt < ALL_DAMAGE_TYPES; dt++) {
		damageType dtype = static_cast<damageType>(dt);
		int sdmg = m->getDamageOfType(dtype);
		if (sdmg > 0) {
			win.write(atx, ++aty, " +" + getDamageTypeName(dtype), maincol);
			win.write(atx + offset, aty, std::to_string(sdmg), getDamageTypeColor(dtype));
		}
	}

	//Status effects
	weaponSharedPtr wp = m->getWeapon();
	if (wp != nullptr) {
		for (int idx = 0; idx < wp->getStatusEffectCount(); idx++) {
			//Values
			statusEffects sType = static_cast<statusEffects>(idx);
			int sdmg = wp->getStatusEffectDamage(sType);
			//Display
			if (sdmg > 0) {
				win.write(atx, ++aty, " +" + getStatusEffectName(sType), maincol);
				win.write(atx + offset, aty, std::to_string(sdmg), getStatusEffectColor(sType));
			}
		}
	}

	aty++;

	//Defence
	int df = m->getDefence();
	win.write(atx, ++aty, "Defence", maincol);
	win.write(atx + offset, aty, std::to_string(df), TCODColor::sepia);

	//Other resistances, if any
	for (int dt = 0; dt < ALL_DAMAGE_TYPES; dt++) {
		damageType dtype = static_cast<damageType>(dt);
		int res = m->getDamageResist(dtype);
		if (res > 0) {
			win.write(atx + 1, ++aty, getDamageTypeName(dtype), maincol);
			win.write(atx + offset, aty, std::to_string(res), getDamageTypeColor(dtype));
		}
	}

	//Bleed Resist
	win.write(atx, ++aty, "Bleed Res", maincol);
	std::string res = std::to_string(m->getSpecialEffectBuildup(EFFECT_BLEED)->getMaxValue());
	win.write(atx + offset, aty, res, getStatusEffectColor(EFFECT_BLEED));
	//Poison Resist
	win.write(atx, ++aty, "Poison Res", maincol);
	res = std::to_string(m->getSpecialEffectBuildup(EFFECT_POISON)->getMaxValue());
	win.write(atx + offset, aty, res, getStatusEffectColor(EFFECT_POISON));
	//Plague Resist
	win.write(atx, ++aty, "Plague Res", maincol);
	res = std::to_string(m->getSpecialEffectBuildup(EFFECT_PLAGUE)->getMaxValue());
	win.write(atx + offset, aty, res, getStatusEffectColor(EFFECT_PLAGUE));


	//Weaknesses
	for (int dt = 0; dt < ALL_DAMAGE_TYPES; dt++) {
		damageType dtype = static_cast<damageType>(dt);
		if (m->isWeakTo(dtype)) {
			win.write(atx, ++aty, "Weak to " + getDamageTypeName(dtype), getDamageTypeColor(dtype));
		}
	}

	aty++;

	//Special abilities
	if (m->getSpellsKnown().size() > 0) {
		win.write(atx, ++aty, "Special abilities:", maincol);
		for (auto sp : m->getSpellsKnown()) {
			win.write(atx + 1, ++aty, sp->getName(), sp->getColor());
		}
	}

	//Wait for player to finish!
	aty += 3;
	win.write(atx, aty, centreText("PRESS [ESC]", 38), TCODColor::white);
	win.refresh();
	while (win.getkey().vk != KEY_BACK_OUT) {}

}


/*
Display info about whatever we've highlighted with the CURSOR.
*/
void game::drawMouseover(int atx, int aty)
{
	
	//What are we pointing at?
	coord mpt = screenToMapCoords(coord(mouse.cx, mouse.cy));
	if (currentMap->inBounds(mpt.first, mpt.second) && canPlayerSeePoint(mpt.first, mpt.second)) {
	
		//Show highlighted object: person?
		personSharedPtr target = currentMap->getPerson(mpt.first, mpt.second);
		if (target != nullptr)
			drawTargetInfo(target, atx, aty);
		else {
			//Item?
			itemSharedPtr it = currentMap->getItem(mpt.first, mpt.second);
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
	
	else {
		//If the mouse isn't in play, show the player's current target
		personSharedPtr target = player->getTarget();
		if (target != nullptr)
			drawTargetInfo(target, atx, aty);
	}
}


/*
Info about a person we have highlighted on the map.
*/
void game::drawTargetInfo(personSharedPtr target, int atx, int aty)
{
	
	//Health
	win.write(atx, aty, target->getName(), target->getColor());
	win.write(atx, ++aty, "HP:" + target->getHealth().getAsString(), TCODColor::darkRed);
	
	//Status effects
	aty = listStatusEffects(target, atx, aty);
	
	//Text description
	win.writeWrapped(atx, ++aty, 20, target->description, TCODColor::lightGrey);
}


/*
Lists all status effects currently afflicting the given character.
Returns the next line after the last one we draw on.
*/
int game::listStatusEffects(personSharedPtr target, int atx, int aty)
{

	//Status effects: BLEED
	if (target->getBleedDuration() > 0) {
		win.write(atx, ++aty, "BLEEDING " + std::to_string(target->getBleedDuration()), TCODColor::crimson);
	}
	else {
		auto bl = target->getSpecialEffectBuildup(EFFECT_BLEED);
		if (bl->getValue() > 0)
			win.drawCounter(*bl, "BLEED", atx, ++aty, TCODColor::crimson, TCODColor::darkGrey, 10);
	}

	//Status effects: POISON
	if (target->getPoisonDuration() > 0) {
		win.write(atx, ++aty, "POISONED x" + std::to_string(target->getPoisonDuration()), TCODColor::lime);
	}
	else {
		auto pois = target->getSpecialEffectBuildup(EFFECT_POISON);
		if (pois->getValue() > 0)
			win.drawCounter(*pois, "POISON", atx, ++aty, TCODColor::lime, TCODColor::darkGrey, 10);
	}

	//Status effects: PLAGUE
	if (target->getPlagueDamage() > 0) {
		win.write(atx, ++aty, "PLAGUE " + std::to_string(target->getPlagueDamage()), TCODColor::sepia);
	}
	else {
		auto plague = target->getSpecialEffectBuildup(EFFECT_PLAGUE);
		if (plague->getValue() > 0)
			win.drawCounter(*plague, "PLAGUE", atx, ++aty, TCODColor::sepia, TCODColor::darkGrey, 10);
	}

	//Blinding
	if (target->isBlind()) {
		win.write(atx + 1, ++aty, "Blinded " + std::to_string(target->getBlindnessDuration()), TCODColor::lightestYellow);
	}

	//Fear
	if (target->fear > 0)
		win.write(atx + 1, ++aty, "Afraid " + std::to_string(target->fear), TCODColor::orange);

	//Acidified
	if (target->acidic > 0)
		win.write(atx + 1, ++aty, "Acidic " + std::to_string(target->acidic), getDamageTypeColor(DAMAGE_ACID));

	//Burning
	if (target->burning > 0)
		win.write(atx + 1, ++aty, "Burning " + std::to_string(target->burning), getDamageTypeColor(DAMAGE_FIRE));

	//Electrified
	if (target->electrified > 0)
		win.write(atx + 1, ++aty, "Electrified " + std::to_string(target->electrified), getDamageTypeColor(DAMAGE_ELECTRIC));

	//Frozen
	if (target->frozen > 0)
		win.write(atx + 1, ++aty, "Frozen " + std::to_string(target->frozen), getDamageTypeColor(DAMAGE_COLD));

	//Entangled
	if (target->isEntangled())
		win.write(atx, ++aty, "Entangled " + std::to_string(target->getEntangleDuration()), TCODColor::lightGrey);

	//Deathlink
	if (target->deathlink > 0)
		win.write(atx, ++aty, "Deathlink " + std::to_string(target->deathlink), TCODColor::darkFlame);
	
	//Silence
	if (target->silence > 0)
		win.write(atx, ++aty, "Silence " + std::to_string(target->silence), TCODColor::lightBlue);

	return aty++;
}


/*
Allows animations to adjust draw data when drawing the map.
*/
drawDataSharedPtr game::getAnimationDataOverride(drawDataSharedPtr baseData, int x, int y)
{
	for (auto a : playingAnimations) {
		a->getDrawData(baseData, x, y);
	}
	return baseData;
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
	Animation plays every time someone casts a spell.
*/
void game::spellTitleAnimation(personSharedPtr caster, spellSharedPtr sp)
{
	addAnimations(new textSplash(caster->getx() - 3, caster->gety() - 1, sp->getName(), sp->getColor()));
}


/*
	ITEM INFO
*/

/*
This just figures out which particular info-drawing function to use.
*/
void game::drawItemInfo(itemSharedPtr it, int atx, int aty, bool drawImage)
{

	//Box around it
	win.clearRegion(atx - 1, aty - 1, 46, 19);
	win.drawBox(atx - 1, aty - 1, 46, 19, TCODColor::darkSepia);
	
	//Tile and name
	win.writec(atx, aty, it->getTileCode(), it->getColor());
	win.write(atx + 2, aty, it->getName(), it->getColor());
	
	//Type of item
	win.write(atx, ++aty, getItemCategoryName(it->getCategory()), TCODColor::darkGrey);
	
	//Indicate whether equipped or not
	if (player->hasItemEquipped(it))
		win.write(atx + 18, aty, "EQUIPPED", TCODColor::grey);
	
	//Show price, if one is defined
	if (it->getPrice() > 0) {
		win.write(atx, ++aty, "PRICE: ", TCODColor::white);
		win.writec(atx + 6, aty, FRAGMENT_GLYPH, TCODColor::amber);
		win.write(atx + 8, aty, std::to_string(it->getPrice()), TCODColor::amber);
		//And key devoured, if applicable
		if (it->eatsKeyWhenBought) {
			auto keyEaten = getItemByHandle(it->getKeyEaten());
			win.write(atx, ++aty, "Formed from the " + keyEaten->getName() + '.', keyEaten->getColor());
		}
	}
	
	//Item description
	aty = win.writeWrapped(atx + 1, aty + 1, 40, it->description, TCODColor::lightGrey);
	
	//Rest of item info
	aty += 1;
	atx += 1;
	auto cat = it->getCategory();
	switch (cat) {
	case(ITEM_WEAPON): drawWeaponInfo(std::static_pointer_cast<weapon>(it), atx, aty); break;
	case(ITEM_OFFHAND): drawWeaponInfo(std::static_pointer_cast<weapon>(it), atx, aty); break;
	case(ITEM_BODY_ARMOUR): drawArmourInfo(std::static_pointer_cast<armour>(it), atx, aty); break;
	case(ITEM_HELMET): drawArmourInfo(std::static_pointer_cast<armour>(it), atx, aty); break;
	case(ITEM_SPELL): drawSpellInfo(std::static_pointer_cast<spell>(it), atx, aty); break;
	case(ITEM_CONSUMABLE): drawConsumableInfo(std::static_pointer_cast<consumable>(it), atx, aty); break;
	case(ITEM_CHARM): drawCharmInfo(std::static_pointer_cast<charm>(it), atx, aty); break;
	case(ITEM_MISC): drawMiscItemInfo(std::static_pointer_cast<miscItem>(it), atx, aty); break;
	}

	//Item image
	if (drawImage)
		drawItemImage(it, 50, 1);

}

/*
Weapon descriptions.
*/
void game::drawWeaponInfo(weaponSharedPtr it, int atx, int aty)
{
	int offset = 13;
	TCODColor maincol = TCODColor::white;
	
	//Stuff only relevant to weapons
	if (it->getCategory() == ITEM_WEAPON) {

		//Profane?
		if (it->isProfane) {
			win.write(atx, aty++, "*PROFANE*", TCODColor::purple);
		}
		
		//Damage
		win.write(atx, aty, "DAMAGE:", TCODColor::darkRed);
		win.write(atx + offset, aty, std::to_string(it->getDamage()), maincol);
		
		//Special damage types
		for (int d = 0; d < ALL_DAMAGE_TYPES; d++) {
			damageType dt = static_cast<damageType>(d);
			int dmg = it->getDamageOfType(dt);
			if (dmg > 0) {
				win.write(atx, ++aty, " " + getDamageTypeName(dt), getDamageTypeColor(dt));
				win.write(atx + offset, aty, '+' + std::to_string(dmg), TCODColor::white);
			}
		}
		
		//Status effects applied
		for (int idx = 0; idx < it->getStatusEffectCount(); idx++) {
			statusEffects sType = static_cast<statusEffects>(idx);
			int sdmg = it->getStatusEffectDamage(sType);
			if (sdmg > 0) {
				win.write(atx + offset, ++aty, '+' + std::to_string(sdmg), TCODColor::white);
				win.write(atx + 1, aty, getStatusEffectName(sType), getStatusEffectColor(sType));
			}
		}
		
		//Scaling
		win.write(atx, ++aty, "SCALING:", TCODColor::red);
		std::string scaling = "";
		if (it->getScalingDamage(SCALE_STR))
			scaling += "Str ";
		if (it->getScalingDamage(SCALE_DEX))
			scaling += "Dex ";
		if (it->getScalingDamage(SCALE_ARC))
			scaling += "Arc ";
		if (it->getScalingDamage(SCALE_DEV))
			scaling += "Dev ";
		win.write(atx + offset, aty, scaling, maincol);
		//Rate
		win.write(atx, ++aty, "SPEED", TCODColor::yellow);
		win.write(atx + offset, aty, getAttackSpeedName(it->getAttackDelay()), maincol);
	}

	//Damage to self
	int dmgToSelf = it->getDamageToSelf();
	if (dmgToSelf > 0) {
		win.write(atx, ++aty, "DMG TO SELF", TCODColor::red);
		win.write(atx + offset, aty, std::to_string(dmgToSelf), maincol);
		win.write(atx, ++aty, "  (Damage to self is restored upon kill)", maincol);
	}

	//Defence, if any
	if (it->getDefence() > 0) {
		win.write(atx, ++aty, "DEFENCE", TCODColor::sepia);
		win.write(atx + offset, aty, std::to_string(it->getDefence()), maincol);
	}

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
	if (it->getBleedResist() > 0) {
		win.write(atx, ++aty, "BLEED Resist", TCODColor::crimson);
		win.write(atx + offset, aty, std::to_string(it->getBleedResist()), maincol);
	}
	//Poison resist
	if (it->getPoisonResist() > 0) {
		win.write(atx, ++aty, "POISON Resist", TCODColor::lime);
		win.write(atx + offset, aty, std::to_string(it->getPoisonResist()), maincol);
	}

	//Damage penalty
	int dp = it->getDamagePenalty();
	if (dp > 0) {
		win.write(atx, ++aty, "DMG Penalty", TCODColor::red);
		win.write(atx + offset, aty, std::to_string(dp) + '%', maincol);
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
	spellSharedPtr atk = it->getSpecialAttack();
	if (atk != nullptr) {
		//Name of special attack
		win.writec(atx, ++aty, VIGOUR_GLYPH, TCODColor::green);
		win.write(atx + 1, aty, std::to_string(atk->getVigourCost()), TCODColor::green);
		win.write(atx + 4, aty, atk->getName(), atk->getColor());
		//Special attack description
		win.writeWrapped(atx + 4, ++aty, 30, atk->description, TCODColor::lightGrey);
	}

	//Rune, if any
	aty += 2;
	weaponRuneSharedPtr rune = it->getRune();
	if (rune != nullptr) {
		win.write(atx, aty, "RUNE:", TCODColor::white);
		win.write(atx + 5, aty, rune->name, rune->color);
	}
}

/*
Armour descriptions.
*/
void game::drawArmourInfo(armourSharedPtr it, int atx, int aty)
{
	int offset = 12;
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
	
	//Poison resist
	int pr = it->getPoisonResist();
	if (pr) {
		win.write(atx, ++aty, "POISON RES", TCODColor::lime);
		win.write(atx + offset, aty, std::to_string(pr), maincol);
	}

	//Plague resist
	int plr = it->getPlagueResist();
	if (plr) {
		win.write(atx, ++aty, "PLAGUE RES", TCODColor::amber);
		win.write(atx + offset, aty, std::to_string(plr), maincol);
	}
	
	//Move speed adjustment, if body armour
	if (it->getCategory() == ITEM_BODY_ARMOUR) {
		win.write(atx, ++aty, "SPEED", TCODColor::orange);
		win.write(atx + offset, aty, getAttackSpeedName(it->getMoveSpeed()), maincol);
	}
}

/*
Spell description.
*/
void game::drawSpellInfo(spellSharedPtr it, int atx, int aty)
{
	
	//Type: arcane or divine
	if (it->usesSpellPower)
		win.write(atx, aty, "ARCANE", TCODColor::magenta);
	else if (it->usesDivinePower)
		win.write(atx, aty, "DIVINE", TCODColor::darkYellow);
	if (it->isProfane)
		win.write(atx + 7, aty, "(Profane)", TCODColor::purple);
	
	//Spell type
	win.write(atx, ++aty, "Type:", TCODColor::lightBlue);
	attackType at = it->getAttackType();
	if (at == ATTACK_BUFF_SELF)
		win.write(atx + 5, aty, "Buff Self", TCODColor::white);
	else if (at == ATTACK_BUFF_WEAPON)
		win.write(atx + 5, aty, "Weapon Buff", TCODColor::white);
	else if (at == ATTACK_RANGE)
		win.write(atx + 5, aty, "Ranged Attack", TCODColor::white);
	else if (at == ATTACK_AOE)
		win.write(atx + 5, aty, "Area Attack", TCODColor::white);
	else if (at == ATTACK_MELEE)
		win.write(atx + 5, aty, "Melee Attack", TCODColor::white);
	
	//Spell details
	if (at == ATTACK_BUFF_WEAPON) {
		//Buff details
		win.write(atx + 1, ++aty, "Adds", TCODColor::white);
		weaponBuff buff = it->getWeaponBuff();
		std::string txt = std::to_string(buff.bonusDamage) + " " + getDamageTypeName(buff.dtype);
		win.write(atx + 6, aty, txt, getDamageTypeColor(buff.dtype));
		win.write(atx + 7 + txt.size(), aty, "damage", TCODColor::white);
	}
	
	else if (at != ATTACK_BUFF_SELF && at != ATTACK_MELEE) {
		//Range of attack
		win.write(atx, ++aty, "Range:", TCODColor::lightBlue);
		win.write(atx + 6, aty, std::to_string(it->getAttackRange()), TCODColor::white);
	}
	
	//Effects conferred
	for (int i = 0; i < it->getEffectsCount(); i++) {
		//Effect type and potency
		effect eType = it->getEffectType(i);
		int potency = it->getEffectPotency(i);
		//Display it
		win.write(atx + 1, ++aty, std::to_string(potency), TCODColor::white);
		win.write(atx + 4, aty, getEffectName(eType), TCODColor::lightGrey);
	}
}


/*
Consumables info
*/
void game::drawConsumableInfo(consumableSharedPtr it, int atx, int aty)
{
	
	if (it->isRangedAttackItem()) {
		//This is just a container for a spell attack!
		drawSpellInfo(it->getRangedAttack(), atx, aty);
	}
	
	else if (it->getWeaponBuff() != nullptr) {
		
		//This is a weapon buff.
		auto buff = it->getWeaponBuff();
		std::string txt1 = "Weapon deals +" + std::to_string(buff->bonusDamage) + " ";
		win.write(atx, aty, txt1, TCODColor::white);
		
		//Buff type: damage type or status effect?
		std::string txt2;
		
		if (buff->etype != EFFECT_NONE) {
			//Status effect
			txt2 = getStatusEffectName(buff->etype);
			win.write(atx + txt1.size(), aty, txt2, getStatusEffectColor(buff->etype));
		}
		
		else {
			//Damage type
			txt2 = getDamageTypeName(buff->dtype);
			win.write(atx + txt1.size(), aty, txt2, getDamageTypeColor(buff->dtype));
		}
		
		//Coda
		win.write(atx + txt1.size() + txt2.size(), aty, " damage.", TCODColor::white);
	
	}
	
	else {
		
		//Just applies some sort of effect to the player.
		for (auto eff : it->getEffects()) {
			win.write(atx, ++aty, std::to_string(it->getPotency()), TCODColor::white);
			win.write(atx + 4, aty, getEffectName(eff), TCODColor::lightGrey);
		}
	
	}
}


/*
Charm info.
*/
void game::drawCharmInfo(charmSharedPtr it, int atx, int aty)
{
	
	//List effects
	auto allEffects = it->getAllEffects();
	auto allPotencies = it->getAllPotencies();
	for (int i = 0; i < allEffects.size(); i++) {
		effect eff = allEffects.at(i);
		int pot = allPotencies.at(i);
		win.write(atx, ++aty, std::to_string(pot), TCODColor::white);
		win.write(atx + 3, aty, getEffectName(eff), TCODColor::lightGrey);
	}
	
	//Damage resistances
	int offset = 14;
	TCODColor maincol = TCODColor::lightGrey;
	for (int r = 0; r != ALL_DAMAGE_TYPES; r++) {
		damageType dr = static_cast<damageType>(r);
		int res = it->getDamageResist(dr);
		if (res > 0) {
			win.write(atx, ++aty, "DEF:" + getDamageTypeName(dr), getDamageTypeColor(dr));
			win.write(atx + offset, aty, std::to_string(res), maincol);
		}
	}

	//Status effect resistances
	//Bleed resist
	int br = it->getBleedResist();
	if (br) {
		win.write(atx, ++aty, "BLEED RES", TCODColor::crimson);
		win.write(atx + offset, aty, std::to_string(br), maincol);
	}

	//Poison resist
	int pr = it->getPoisonResist();
	if (pr) {
		win.write(atx, ++aty, "POISON RES", TCODColor::lime);
		win.write(atx + offset, aty, std::to_string(pr), maincol);
	}

	//Plague resist
	int plr = it->getPlagueResist();
	if (plr) {
		win.write(atx, ++aty, "PLAGUE RES", TCODColor::amber);
		win.write(atx + offset, aty, std::to_string(plr), maincol);
	}

}


/*
Miscellaneous item info
*/
void game::drawMiscItemInfo(miscItemSharedPtr it, int atx, int aty)
{

	//What we actually do
	win.writeWrapped(atx, aty++, 30, it->getWhatWeDo(), TCODColor::white);

	if (it->isRunestone) {
		win.write(atx, aty++, "Equip to slot into your current weapon.", TCODColor::white);
		//Specific effects
		weaponRuneSharedPtr rune = it->getRune();
		if (rune != nullptr) {
			//Description
			if (rune->addScalingType == SCALE_STR || rune->addScalingType == SCALE_DEX || rune->addScalingType == SCALE_ARC ||
				rune->addScalingType == SCALE_DEV) {
				
				//Scales with a stat
				std::string scaleType;
				switch (rune->addScalingType) {
				case(SCALE_STR): scaleType = "Strength"; break;
				case(SCALE_DEX): scaleType = "Dexterity"; break;
				case(SCALE_ARC): scaleType = "Arcane"; break;
				case(SCALE_DEV): scaleType = "Devotion"; break;
				}
				
				win.write(atx, ++aty, "Improves scaling with " + scaleType + ".", rune->color);
			}
			else {

				//Adds bonus damage of a particular type
				std::string addsDmg;
				switch (rune->addScalingType) {
				case(SCALE_ACID): addsDmg = "Acid"; break;
				case(SCALE_COLD): addsDmg = "Cold"; break;
				case(SCALE_FIRE): addsDmg = "Fire"; break;
				case(SCALE_ELECTRIC): addsDmg = "Electric"; break;
				case(SCALE_MAGIC): addsDmg = "Magic"; break;
				case(SCALE_PHYSICAL): addsDmg = "Physical"; break;
				case(SCALE_POISON): addsDmg = "Poison"; break;
				case(SCALE_BLEED): addsDmg = "Bleed"; break;
				}

				win.write(atx, ++aty, "Weapon deals additional " + addsDmg + " damage.", rune->color);

			}
		}
	}
}


/*
Draws an item's image onto the screen, if it has one.
*/
void game::drawItemImage(itemSharedPtr it, int atx, int aty, bool drawOtherArmourPiece)
{

	if (it->hasImage()) {

		//Box around the image
		win.clearRegion(atx - 1, aty - 1, 41, 61);
		win.drawBox(atx - 1, aty - 1, 41, 61, TCODColor::darkSepia);

		//If this is a HELMET item, we also draw the armour!
		if (it->getCategory() == ITEM_HELMET && drawOtherArmourPiece) {
			auto a = player->getArmour();
			if (a != nullptr && a->hasImage())
				win.drawImage(a->getImage(), atx, aty);
		}
		
		//Draw this item
		win.drawImage(it->getImage(), atx, aty);

		//If this is an ARMOUR item, we also draw the helmet!
		if (it->getCategory() == ITEM_BODY_ARMOUR && drawOtherArmourPiece) {
			auto h = player->getHelmet();
			if (h != nullptr && h->hasImage())
				win.drawImage(h->getImage(), atx, aty);
		}
	
	}
}




/*
	COMMAND PROCESSING
*/



void game::processCommand()
{
	if (!key.pressed) {

		//Meta
		if (key.c == 'Q')
			isGameOver = true;
		else if (key.c == 'F')
			TCODConsole::setFullscreen(!TCODConsole::isFullscreen());
		else if (key.c == 'l')
			startAutoWalk();

		//Menus
		else if (key.vk == KEY_BACK_OUT)
			menuBackOut();
		else if (key.c == 'i')
			createInventoryMenu();
		else if (key.vk == KEY_ACCEPT)
			acceptCurrentMenuIndex();
		else if (key.c == 'p') {
			if (state == STATE_VIEW_INVENTORY)
				setupLevelUpMenu();
			else
				drawPlayerInfo(MAP_DRAW_X, MAP_DRAW_Y);
		}
		else if (key.c == 'm') {
			if (state == STATE_VIEW_INVENTORY) {
				saveGame();
				addMessage("Game saved!", TCODColor::white);
			}
		}

		//Using stuff
		else if (key.c == 'c')
			openConsumableMenu();
		else if (key.c == 'x')
			openSpellMenu();
		else if (isNumberKey(key))
			useAbilityByHotkey(key);
		else if (key.c == 'e')
			useConsumable();
		else if (key.c == 'q')
			castSpell(player->getCurrentSpell());
		else if (key.c == 'f') {
			player->swapWeapon();
			playerTurnDelay += SPEED_NORMAL;
		}

		//Movement
		else if (key.c == 't')
			toggleTargetMode();
		else if (key.c == 'z')
			playerTurnDelay = player->getMoveDelay();
		else if (isMovementKey(key))
			if (state == STATE_VIEW_MAP)
				processMove(key);
			else
				navigateMenu(key);

		//Chitchat
		else if (key.c == 'h')
			talkToNPC();

		//Debug
		else if (key.c == '~')
			debugMenu();

	}
}

void game::processMouseClick()
{
	if (mouse.lbutton_pressed) {
		if (state == STATE_VIEW_MAP) {
			//Start autowalking to here, if in map mode
			startAutoWalk();
		}
		else {
			//Menu selection otherwise
			acceptCurrentMenuIndex();
		}
	}
	else if (mouse.rbutton_pressed) {
		//View monster info
		coord clk = screenToMapCoords(coord(mouse.cx, mouse.cy));
		if (currentMap->inBounds(clk.first, clk.second)) {
			personSharedPtr m = currentMap->getPerson(clk.first, clk.second);
			if (m != nullptr && !m->isPlayer) {
				drawMonsterInfo(std::static_pointer_cast<monster>(m), MAP_DRAW_X - 1, MAP_DRAW_Y - 1);
			}
		}
	}
}


/*
Returns whether mouse in the given region.
*/
bool game::isMouseOver(int x, int xwidth, int y)
{
	return (mouse.cx >= x && mouse.cx <= x + xwidth && mouse.cy == y);
}



/*
		AUTO-WALKING ALONG A PATH
*/


/*
Set up an auto-walk!
*/
void game::startAutoWalk()
{
	coord pos = screenToMapCoords(coord(mouse.cx, mouse.cy));
	//Make sure we're going to a valid space
	if (currentMap->inBounds(pos.first, pos.second)) {
		//We can only walk to spaces we can see
		if (canPlayerSeePoint(pos.first, pos.second)) {
			//And we can't walk into enemies
			if (currentMap->getPerson(pos.first, pos.second) == nullptr) {
				isAutoWalking = true;
				playerAutoPath = getPathToCoord(player->getPosition(), pos);
			}
		}
	}
}



/*
Move along a predetermined path.
Stops if player fails to move or if the path ends.
*/
void game::playerAutoWalk()
{
	if (!playerAutoPath->isEmpty()) {
		//New coordinates
		int x = player->getx();
		int y = player->gety();
		//If the move fails, give up
		if (!playerAutoPath->walk(&x, &y, false))
			isAutoWalking = false;
		else
			movePerson(player, x, y);
	}
	else
		isAutoWalking = false;
}

/*
We use up our selected consumable.
*/
void game::useConsumable()
{
	consumableSharedPtr toUse = player->getSelectedConsumable();
	if (toUse != nullptr) {
		//Make sure we have enough
		if (toUse->getAmountLeft() > 0) {
			
			//Use it!
			if (toUse->consumeOnUse)
				toUse->lose();
			
			//And perform the proper effect
			int potency = toUse->getPotency();

			//Is this a ranged-attack item?
			if (toUse->isRangedAttackItem()) {
				//We're basically casting a SPELL here
				doRangedSpell(toUse->getRangedAttack());
			}
			
			else if (toUse->getWeaponBuff() != nullptr) {
				//Buffs a weapon
				weaponSharedPtr wp = player->getWeapon();
				if (wp != nullptr)
					wp->setBuff(*toUse->getWeaponBuff());
			}

			else if (toUse->addsPermanentBuff) {
				//We add some kind of permanent buff that has to be tracked
				for (auto eff : toUse->getEffects()) {
					std::string effName = toUse->getName() + " - " + getEffectName(eff);
					if (player->addBuff(effName, toUse->getColor(), eff, potency))
						applyEffectToPerson(player, eff, potency);
				}
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
	consumableSharedPtr it =std::static_pointer_cast<consumable>(currentMenu->getSelectedItem());
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
void game::doAOE(spellSharedPtr sp, personSharedPtr caster)
{
	
	//Animation of the event; colour goes from spell colour to a lighter version on the outside
	int r = sp->getAttackRange();
	TCODColor col1 = sp->getColor();
	TCODColor col2 = win.mixColors(col1, TCODColor::white, 0.5);
	addAnimations(new shockwave(caster->getx(), caster->gety(), col1, col2));
	
	//Execute the spell
	for (int x = caster->getx() - r; x <= caster->getx() + r; x++) {
		for (int y = caster->gety() - r; y <= caster->gety() + r; y++) {
			//Check for target here
			personSharedPtr target = currentMap->getPerson(x, y);
			//AOE doesn't affect caster
			if (target != nullptr && target != caster) {
				dischargeSpellOnTarget(sp, caster, target);
			}
		}
	}
	//addAnimations(new explosion(caster->getPosition(), r, col1, col2));
}

/*
Apply any effect to any person.
Its strength is the given potency.
*/
void game::applyEffectToPerson(personSharedPtr target, effect eff, int potency, personSharedPtr caster)
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
	else if (eff == DO_WARP && target->isPlayer)
		setupWarpMenu();
	else if (eff == MAKE_RITUAL_OFFERING && target->isPlayer)
		makeRitualOffering();
	else if (eff == TELEPORT_TO_VOID)
		teleportToVoid();
	else if (eff == TELEPORT_BACK_FROM_VOID)
		teleportOutOfVoid();
	else if (eff == NAVIGATE_STAIRS)
		takeStairs();

	//Special spell effect
	else if (eff == CASTER_MELEE_ATTACK)
		meleeAttack(caster, target);
	else if (eff == HURT_CASTER)
		caster->takeDamage(potency);
	else if (eff == HEAL_CASTER)
		caster->addHealth(potency);
	else if (eff == ASTRAL_IMPRISONMENT) {
		if (caster->isProfane())
			currentMap->removePerson(caster);
	}
	else if (eff == ACQUIRE_FRAGMENTS)
		fragments += potency;

	//Special effects, other
	else if (eff == KNOCKBACK_TARGET)
		knockbackTarget(caster, target, potency);
	else if (eff == PULL_CLOSER)
		pullTarget(caster, target, potency);
	else if (eff == TELEPORT_VIA_WATER)
		waterWarp(target, potency);
	else if (eff == TELEPORT)
		teleport(target, potency);
	else if (eff == DROP_OOZE)
		currentMap->setTile(tile_Ooze(), target->getx(), target->gety());
	else if (eff == DROP_WEB)
		currentMap->setTile(tile_Web(), target->getx(), target->gety());
	else if (eff == DROP_DRAINING_POOL)
		currentMap->setTile(tile_DrainingPool(), target->getx(), target->gety());
	else if (eff == DROP_CORRODING_BLOOD)
		currentMap->setTile(tile_CorrodingBlood(), target->getx(), target->gety());
	else if (eff == CREATE_FOG)
		currentMap->createFogCloud(target->getx(), target->gety(), potency);

	//Some effect that the person should take care of
	else
		target->applyEffect(eff, potency);

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
	if (kp.c == 'w')
		ynew--;
	else if (kp.c == 's')
		ynew++;
	else if (kp.c == 'd')
		xnew++;
	else if (kp.c == 'a')
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
	return kp.c == 'w' || kp.c == 's' || kp.c == 'a' || kp.c == 'd';
	//return kp.vk == KEY_NORTH || kp.vk == KEY_EAST || kp.vk == KEY_SOUTH || kp.vk == KEY_WEST;
}

/*
Returns whether the given key is a number key.
*/
bool game::isNumberKey(TCOD_key_t kp)
{
	return kp.vk == TCODK_0 || kp.vk == TCODK_1 || kp.vk == TCODK_2 || kp.vk == TCODK_3 || kp.vk == TCODK_4 || kp.vk == TCODK_5 ||
		kp.vk == TCODK_6 || kp.vk == TCODK_7 || kp.vk == TCODK_8 || kp.vk == TCODK_9;
}

/*
Returns numeric value corresponding to the given keypress.
*/
int game::getNumberByKeycode(TCOD_key_t kp) {
	if (kp.vk == TCODK_0)
		return 0;
	else if (kp.vk == TCODK_1)
		return 1;
	else if (kp.vk == TCODK_2)
		return 2;
	else if (kp.vk == TCODK_3)
		return 3;
	else if (kp.vk == TCODK_4)
		return 4;
	else if (kp.vk == TCODK_5)
		return 5;
	else if (kp.vk == TCODK_6)
		return 6;
	else if (kp.vk == TCODK_7)
		return 7;
	else if (kp.vk == TCODK_8)
		return 8;
	else if (kp.vk == TCODK_9)
		return 9;
	return -1;
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
void game::movePerson(personSharedPtr p, int xnew, int ynew)
{

	//In bounds?
	if (currentMap->inBounds(xnew, ynew)) {
		
		//Possible to walk on?
		if (currentMap->isWalkable(xnew, ynew)) {
		
			//Is someone already here?
			personSharedPtr here = currentMap->getPerson(xnew, ynew);
			
			if (here != nullptr) {

				//Player delay, if this is the player
				if (p->isPlayer)
					playerTurnDelay = p->getAttackDelay();
				
				//We attack! Or switch places!
				if (p->isHostile != here->isHostile) {
					meleeAttack(p, here);
				}
				else if (p->canSwapWithAllies) {
					//A friendly is here; swap places if we can!
					auto tempPos = p->getPosition();
					p->setPosition(here->getPosition());
					here->setPosition(tempPos);
				}
			}
			
			else {

				//If we're entangled, this just reduces it - but we don't get to move
				if (p->isEntangled()) {
					p->struggle();
				}
				else {
					//Adjust position and deal with the consequences
					p->setPosition(xnew, ynew);
					standOnTile(p);
				}
				
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
void game::standOnTile(personSharedPtr victim)
{
	//If we're the PLAYER, we pick up any items that are here
	if (victim->isPlayer) {
		itemSharedPtr itemHere = currentMap->getItem(victim->getx(), victim->gety());
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
		else {
			addMessage("The " + t->getName() + " is locked!", t->getColor());
		}
	}
}



/*
	INTER - MAP MOVEMENT
*/


/*
Try taking the stairs!
*/
void game::takeStairs()
{
	std::string newHandle = currentMap->getConnection(CONNECT_VERTICAL);
	if (newHandle.size()) {
		loadMapFromHandle(newHandle, CONNECT_VERTICAL, player->getx(),player->gety());
	}
}



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
	if (newMap == nullptr) {
		newMap = makemap.loadMapFromFile(handle);
		checkForAreaText(handle);
	}
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
	if (currentMap != nullptr)
		currentMap->removePerson(player);
	
	//Set new map
	setCurrentMap(newMap);
	turns.clear();
	
	//Respawn monsters on map
	newMap->respawnAllMonsters(storyEventsReady);
	
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
	for (auto person : newMap->getAllPeople())
		if (!person->isPlayer)
			turns.addEntity(person, 1);
	
	//Update FOV for new map
	currentMap->updateFOV(player->getx(), player->gety());
}



/*
Warp between warp stones.
*/
void game::setupWarpMenu()
{
	
	//Add our current warp point, if it's not in there already
	savePoint currentPt(currentMap, coord(player->getx(), player->gety()));
	addWarpPoint(currentPt);
	//We need a particular item to be able to use warp stones
	if (!player->hasKey("warpstone")) {
		addMessage("This mysterious stone does not respond to your presence.", TCODColor::white);
		return;
	}
	
	//Menu of known save points
	menu* warpMenu = new menu("WARP STONES DISCOVERED");
	for (auto warpPt : warpPoints) {
		elementSharedPtr e(new element(warpPt.name, 0, TCODColor::white));
		warpMenu->addElement(e);
	}
	
	//Ready to rock
	currentMenu = warpMenu;
	setState(STATE_WARP);
}


/*
Only adds it if it's not already in there
*/
void game::addWarpPoint(savePoint pt)
{
	
	//See if it's already in here
	for (auto otherPt : warpPoints) {
		if (otherPt.name == currentMap->getName())
			return;
	}
	
	//ADD NEW ONE!
	pt.name = currentMap->getName();
	warpPoints.push_back(pt);
}


/*
Actually warp us to a selected point
*/
void game::doWarp(std::string warpPointName)
{

	//Find warp point matching the given name
	savePoint* warp = nullptr;
	for (auto warpPt : warpPoints) {
		if (warpPt.name == warpPointName) {
			warp = &warpPt;
			break;
		}
	}

	//Make sure we actually got one
	if (warp != nullptr) {
		restoreFromSavePoint(warp);
		menuBackOut();
	}

}



/*
	COMBAT
*/



/*
One creature attacks another in melee.
*/
void game::meleeAttack(personSharedPtr attacker, personSharedPtr target)
{
	addMessage(attacker->getName() + " strikes " + target->getName() + '!', attacker->getColor());

	//Multiattack?
	for (int a = 0; a < attacker->attacksPerHit; a++) {

		//First: NORMAL DAMAGE
		int damage = attacker->getMeleeDamage();

		//Damage buffs
		float dbuff = (float)attacker->scaleNextAttack / 100.0;
		damage += dbuff * (float)damage;
		if (attacker->scaleNextAttack)
			//This buff only lasts for 1 attack
			attacker->scaleNextAttack = 0;

		//Deal the damage
		target->takeDamage(damage, DAMAGE_PHYSICAL);

		//Now deal special damage types
		for (int dt = 0; dt < ALL_DAMAGE_TYPES; dt++) {
			damageType dtype = static_cast<damageType>(dt);
			int sdmg = attacker->getDamageOfType(dtype);
			if (sdmg > 0) {
				target->takeDamage(sdmg, dtype);
			}
		}

		//Next: SPELL DISCHARGES, if we have one readied
		if (attacker->buffNextMelee != nullptr) {
			dischargeSpellOnTarget(attacker->buffNextMelee, attacker, target);
			attacker->buffNextMelee = nullptr;
		}

		//Next: STATUS EFFECTS and SPECIAL EFFECTS
		weaponSharedPtr wp = attacker->getWeapon();
		if (wp != nullptr) {

			//Status effects applied
			for (int idx = 0; idx < wp->getStatusEffectCount(); idx++) {
				//Values
				statusEffects sType = static_cast<statusEffects>(idx);
				int sdmg = wp->getStatusEffectDamage(sType);
				if (sdmg > 0) {
					//Buffs to a particular effect type
					if (sType == EFFECT_BLEED && attacker->getBleedDuration() > 0)
						sdmg += attacker->bleedScaling;
					sdmg = sdmg * attacker->bleedDamageFactor;
					//Deal the damage
					target->takeStatusEffectDamage(sType, sdmg);
				}
			}

			//Damage to self
			int dmgToSelf = wp->getDamageToSelf();
			if (dmgToSelf > 0) {
				attacker->takeDamage(dmgToSelf);
				wp->addToDamageReservoir(dmgToSelf);
			}
		
		}

	}

	//Enemy turns it back on us
	if (target->divineRetribution > 0) {
		attacker->takeDamage(target->divineRetribution, DAMAGE_BLESSED);
	}
	
	//Update targeting
	if (target->isDead) {
		
		//Remove target data
		attacker->clearTarget();
		addMessage(target->getName() + " dies!", TCODColor::white);
		
		//Possible health restoration on kill!
		weaponSharedPtr wp = attacker->getWeapon();
		if (wp != nullptr) {
			attacker->addHealth(wp->getDamageReservoir());
			wp->clearDamageReservoir();
		}

		//Possible damage buff on kill
		attacker->gainFlatDamageBuff(attacker->killDamageBuff);

		//Target sometimes EXPLODES
		if (target->deathfireInfusion > 0)
			doAOE(spellSharedPtr(new spell(
				"Deathfire", TCODColor::flame, ATTACK_AOE, 3, APPLY_FIRE_DAMAGE, target->deathfireInfusion)),
				attacker);

		//Deathlink can return us to life!
		if (target->deathlink > 0) {
			spellTitleAnimation(target, prayer_Deathlink());
			target->addHealth(target->deathlink);
			target->deathlink = 0;
			target->isDead = false;
		}
	
	}
	else
		attacker->setTarget(target);

	//Dead target may restore HP!
	if (target->isDead)
		attacker->addHealth(attacker->healthOnKill);

	//If we're an AI, this expends all of our free moves
	if (!target->isPlayer && target->hasFreeMoves())
		target->clearFreeMoves();

	//Multiattack ends after use
	attacker->attacksPerHit = 1;

}

/*
Target gets shoved away.
*/
void game::knockbackTarget(personSharedPtr knocker, personSharedPtr target, int distance)
{
	int xv = get1dVector(knocker->getx(), target->getx());
	int yv = get1dVector(knocker->gety(), target->gety());
	for (int i = 0; i < distance; i++) {
		movePerson(target, target->getx() + xv, target->gety() + yv);
	}
}

/*
Target gets yanked closer.
*/
void game::pullTarget(personSharedPtr puller, personSharedPtr target, int distance)
{
	int xv = get1dVector(target->getx(), puller->getx());
	int yv = get1dVector(target->gety(), puller->gety());
	for (int i = 0; i < distance; i++) {
		movePerson(target, target->getx() + xv, target->gety() + yv);
	}
}


/*
Teleport from one water tile to another.
Only works if we're already on a water tile.
*/
bool game::waterWarp(personSharedPtr target, int distance)
{
	if (currentMap->getTile(target->getx(), target->gety())->isWater) {
		//Find another water tile within a certain range
		coordVector waterPts;
		for (int x = target->getx() - distance; x <= target->getx() + distance; x++) {
			for (int y = target->gety() - distance; y <= target->gety() + distance; y++) {
				//Make sure we're in bounds
				if (currentMap->inBounds(x, y) && currentMap->isWalkable(x, y) && currentMap->getPerson(x, y) == nullptr) {
					//See if this point is WATER
					maptile* t = currentMap->getTile(x, y);
					if (t->isWater)
						waterPts.push_back(coord(x, y));
				}
			}
		}
		//Warp to a point we found
		if (waterPts.size()) {
			int i = randrange(waterPts.size());
			coord pt = waterPts.at(i);
			movePerson(target, pt.first, pt.second);
			return true;
		}
	}
	return false;
}


/*
Warp to a random point within the given radius.
*/
void game::teleport(personSharedPtr target, int distance)
{
	//Make list of warp-able points
	coordVector pts;
	for (int x = target->getx() - distance; x <= target->getx() + distance; x++) {
		for (int y = target->gety() - distance; y <= target->gety() + distance; y++) {
			if (currentMap->inBounds(x, y) && currentMap->isWalkable(x, y) && currentMap->getPerson(x, y) == nullptr) {
				pts.push_back(coord(x, y));
			}
		}
	}
	//Make sure we actually found at least one viable point
	if (pts.size() > 0) {
		//And then jump to one!
		int idx = randrange(pts.size());
		coord pt = pts.at(idx);
		movePerson(target, pt.first, pt.second);
	}
}




/*
Player warps to the VOID EDGE.
Yeah, this is hardcoded.
*/
void game::teleportToVoid()
{
	loadMapFromHandle(VOID_EDGE_MAP, CONNECT_WARP, player->getx(), player->gety());
	setSavePoint();
}

/*
Warp AWAY from the Void!
*/
void game::teleportOutOfVoid()
{
	loadMapFromHandle(VOID_RETURN_MAP, CONNECT_WARP, player->getx(), player->gety());
	setSavePoint();
}



/*
These are how we unlock the final boss!
*/
void game::makeRitualOffering()
{
	auto t = currentMap->getTile(player->getx(), player->gety());
	std::string keyReq = t->unlockCode;
	if (player->hasKey(keyReq)) {
		//We make an offering!
		player->loseKey(keyReq);
		t->activateAltar();
		addMessage("The " + t->getName() + " shines with ethereal light.", TCODColor::white);
		offeringsMade++;
		if (offeringsMade == 4) {
			//SPECIAL EVENT! We get the final key.
			pickUpItem(key_EffigyOfTheSlumberingLord());
		}
	}
	else if (!t->isAltarActive) {
		addMessage("The altar is silent. A proper offering is required.", TCODColor::white);
	}
	else {
		addMessage("An offering already rests upon this altar.", TCODColor::white);
	}
}


/*
	BOSS STUFF
*/



void game::setBoss(monsterSharedPtr m)
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
bool game::itemPickupMessage(itemSharedPtr it)
{
	int atx = MAP_DRAW_X;
	int aty = MAP_DRAW_Y + 10;
	//win.clearRegion(atx - 1, aty, 42, 25);
	//win.drawBox(atx - 1, aty, 42, 25, TCODColor::darkSepia);
	//What we can do
	std::string txt = "[SPACE] Equip  [ESC] Store";
	win.write(atx, ++aty, centreText(txt, 20), TCODColor::white);
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
void game::pickUpItem(itemSharedPtr it)
{
	
	//Get it
	bool stackedWithOther = player->addItem(it);
	
	//Message about it
	addMessage("Got " + it->getName() + "!", it->getColor());
	bool equipItem = itemPickupMessage(it);
	
	//Can't auto-use items that work that way!
	if (it->getCategory() == ITEM_CONSUMABLE) {
		auto co = std::static_pointer_cast<consumable>(it);
		if (co->oneUseOnly)
			equipItem = false;
	}
	
	//Equip, if that's the kind of thing we do
	if (equipItem && !stackedWithOther)
		player->equipItem(it);
	
	//Items from SIR PERCIVEL'S SET accumulate to give us a special key
	if (it->inSirPercivelsSet) {
		gotSirPercivelsSet++;
		if (gotSirPercivelsSet == 5)
			pickUpItem(key_PercivelsSign());
	}
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
		itemSharedPtr e(new item(getItemCategoryName(cat), 0, TCODColor::lightGrey, cat));
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
void game::getDeathDrops(monsterSharedPtr m)
{
	//Items
	for (auto it : m->getItemDrops()) {
		pickUpItem(it);
	}
	//Fragments
	int amount = m->getFragmentsDropped() * player->getFragmentPickupMult();
	fragments += amount;
}


/*
Equip the selected item.
*/
void game::equipItem(itemSharedPtr it)
{
	if (it->getCategory() == ITEM_CONSUMABLE) {
		consumableSharedPtr c = std::static_pointer_cast<consumable>(it);
		if (c->oneUseOnly) {
			//Instantly apply item effect
			for (auto eff : c->getEffects()) {
				applyEffectToPerson(player, eff, c->getPotency());
			}
			//Message about it
			addMessage("Devoured " + c->getName() + '!', c->getColor());
			//Expend item
			player->loseItemForever(c);
			//Close menu
			menuBackOut();
			return;
		}
	}

	//Just equip the item normally
	player->equipItem(it);
}



/*
	SPELL MAGIC
*/


/*
Push hotkey corresponding to a spell to CAST IT!
*/
void game::useAbilityByHotkey(TCOD_key_t kp)
{
	int n = getNumberByKeycode(kp) - 1;
	if (n != -1) {
		player->setCurrentSpell(n);
	}
}

/*
Firing a ranged spell is... COMPLICATED
*/
void game::doRangedSpell(spellSharedPtr sp)
{
	
	//Figure out where we're aiming
	coord tp;
	if (targetModeOn)
		tp = targetPt;
	else
		tp = screenToMapCoords(coord(mouse.cx, mouse.cy));
	
	//Make sure this point is in bounds
	if (currentMap->inBounds(tp.first, tp.second)) {
		
		//Get a path to the target
		coordVectorSharedPtr path = coordVectorSharedPtr(getLine(player->getPosition(), tp));

		//Bullet animation!
		addAnimations(new bulletPath(path, BULLET_TILE, sp->getColor()));
		
		//See if there's something to hit on the path
		personSharedPtr target;
		if (sp->alwaysHitTarget) {
			target = currentMap->getPerson(tp.first, tp.second);
		}
		else {
			target = getTargetOnPath(path);
		}

		//And slam this dude if he exists!
		if (target != nullptr) {
			
			//Is it in range?
			int dist = hypot(player->getx() - target->getx(), player->gety() - target->gety());
			
			if (dist <= sp->getAttackRange()) {
				
				//We hit!
				dischargeSpellOnTarget(sp, player, target);
			
			}
			
			else {
				addMessage("Out of range!", TCODColor::white);
			}
		}
	}
	else {
		addMessage("Aim with the mouse!", TCODColor::white);
	}
}

/*
Player starts the casting of a spell.
*/
void game::castSpell(spellSharedPtr sp)
{
	if (sp == nullptr)
		return;
	int vcost = sp->getVigourCost();

	//Can't cast spells if silenced
	if (player->silence > 0) {
		addMessage("You are silenced!", TCODColor::white);
		return;
	}
	
	if (player->getVigour().getValue() >= vcost) {

		//Casting cost
		player->paySpellCost(sp);

		//ANIMATION BLAST!
		spellTitleAnimation(player, sp);

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
		else if (aType == ATTACK_BUFF_WEAPON) {
			//Spell is applied to our current weapon
			dischargeSpellOnWeapon(sp, player, player->getWeapon());
		}
		else if (aType == ATTACK_AOE) {
			//Hits everything within its radius
			doAOE(sp, player);
		}
		else if (aType == ATTACK_RANGE) {
			//Hits target marked with the cursor
			doRangedSpell(sp);
		}

		//Time taken
		if (player->instantSpellCast > 0) {
			player->instantSpellCast--;
		}
		else {
			playerTurnDelay += SPEED_NORMAL;
		}
		
	}
}

/*
Spell actually affects the target
*/
void game::dischargeSpellOnTarget(spellSharedPtr sp, personSharedPtr caster, personSharedPtr target)
{

	//Ignore stink based spells, possibly!
	if (sp->isStenchBased && target->isStinkResistant()) {
		return;
	}
	
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

		//Additional potency if profane spell is cast from a profane object
		if (sp->isProfane) {
			weaponSharedPtr wp = caster->getWeapon();
			weaponSharedPtr offhand = caster->getOffhand();
			if ((offhand != nullptr && offhand->isProfane) || (wp != nullptr && wp->isProfane))
				potency += potency / 2;
		}

		//Additional potency from low health, possibly
		if (sp->inverseHealthScaling) {
			float f = caster->getHealth().getPercent();
			potency += (1.0 - f) * 100;
		}

		//Permanent buff or temporary?
		if (sp->addPermanentBuff) {
			
			//Permanent buffs need to be tracked
			if (target->addBuff(sp->getName(), sp->getColor(), sp->getEffectType(idx), potency)) {
				applyEffectToPerson(target, sp->getEffectType(idx), potency);
			}
			else {
				addMessage("You cannot stack this buff!", TCODColor::white);
			}
		}
		else {

			//Just a normal spell casting
			applyEffectToPerson(target, sp->getEffectType(idx), potency, caster);
			
			//Infusions apply extra effects
			if (caster->spellAcidInfusion && target != caster) {
				int power = caster->spellAcidInfusion;
				if (sp->isAcidSpell)
					power = power * 2;
				target->takeDamage(power, DAMAGE_ACID);
				caster->spellAcidInfusion = 0;
			}
			if (caster->spellColdInfusion && target != caster) {
				int power = caster->spellColdInfusion;
				if (sp->isColdSpell)
					power = power * 2;
				target->takeDamage(power, DAMAGE_COLD);
				caster->spellColdInfusion = 0;
			}

		}
	
	}

	//Certain animations will be played here
	auto attackType = sp->getAttackType();
	if (attackType == ATTACK_BUFF_SELF) {
		if (sp->useAlternateAnimation) {
			addAnimations(new flashCharacter(target,sp->getColor()));
		}
		else {
			//coordVectorSharedPtr pts = coordVectorSharedPtr(new coordVector());
			//pts->push_back(target->getPosition());
			//addAnimations(new glyphCycle(pts, sp->getColor(), target->getColor()));
		}
	}

}

/*
Spell affects weapon instead of person!
*/
void game::dischargeSpellOnWeapon(spellSharedPtr sp, personSharedPtr caster, weaponSharedPtr target)
{
	//Make sure the weapon actually exists
	if (target != nullptr) {
		target->setBuff(sp->getWeaponBuff());
		//Expend vigour
		caster->paySpellCost(sp);
		//Animation
		coordVector pts;
		pts.push_back(caster->getPosition());
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
	spellSharedPtr sp = std::static_pointer_cast<spell>(currentMenu->getSelectedItem());
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
	elementSharedPtr e = currentMenu->getSelectedItem();
	std::string name = e->getName();
	//Can we afford it?
	if (fragments >= player->getNextLevelCost()) {
		fragments -= player->getNextLevelCost();
		//Buff selected stat
		if (name == "HEALTH") {
			player->stats->health++;
			player->increaseMaxHealth(10);
		}
		else if (name == "VIGOUR") {
			player->stats->vigour++;
			player->increaseMaxVigour(2);
		}
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
	}
}

void game::drawLevelUpMenu(int atx, int aty)
{
	//Stats menu
	drawMenu(currentMenu, atx, aty);
	//Stat levels
	TCODColor statCol = TCODColor::green;
	int offset = 15;
	aty++;
	win.write(atx + offset, ++aty, std::to_string(player->stats->health), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->vigour), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->strength), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->dexterity), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->arcana), statCol);
	win.write(atx + offset, ++aty, std::to_string(player->stats->devotion), statCol);
	//How much the next level COSTS
	aty += 3;
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
	SHOPPING / TALKING
*/



/*
Initializes store inventories
*/
void game::initializeShops()
{

	//Gorem
	shopSharedPtr goremShop = shopSharedPtr(new shop("gorem_shop"));
	goremShop->addItem(shield_WoodenWyrdShield(), 25);
	goremShop->addItem(chime_WyrdBellbranch(), 50);
	goremShop->addItem(wand_BleachwoodWand(), 100);
	goremShop->addItem(prayer_WyrdChantOfStrength(), 25);
	allShops.push_back(goremShop);

	//Ghulluk
	shopSharedPtr ghullukShop = shopSharedPtr(new shop("ghulluk_shop"));
	allShops.push_back(ghullukShop);
	//Watchful Eyestalk
	shopSharedPtr eyestalk = shopSharedPtr(new shop("watchful_eyestalk"));
	eyestalk->addItem(key_UnderpoolKey(), 0);
	allUnlockableShops.push_back(eyestalk);
	//Mawtooth Fragment
	shopSharedPtr mawtooth = shopSharedPtr(new shop("mawtooth_fragment"));
	mawtooth->addItem(key_AbattoirKey(), 0);
	allUnlockableShops.push_back(mawtooth);
	//Withered Finger
	shopSharedPtr finger = shopSharedPtr(new shop("withered_finger"));
	finger->addItem(key_GreenKnightsKey(), 0);
	allUnlockableShops.push_back(finger);
	//Heart of the Old Gods
	shopSharedPtr oldgods = shopSharedPtr(new shop("heart_of_the_old_gods", true));
	oldgods->addItem(consumable_PutridBrew(), 500);
	oldgods->addItem(consumable_GodsbloodBrew(), 500);
	oldgods->addItem(charm_VenomousImbecilesRing(), 300);
	oldgods->addItem(charm_IconOfFamine(), 300);
	oldgods->addItem(charm_AtalundraPactRing(), 500);
	oldgods->addItem(charm_DivinePactRing(), 500);
	oldgods->addItem(charm_AgonyRing(), 500);
	allUnlockableShops.push_back(oldgods);

	//Utric
	shopSharedPtr utricShop = shopSharedPtr(new shop("utric_shop"));
	utricShop->addItem(charm_ArcanaDrenchedCharm(), 100);
	utricShop->addItem(spell_ArcaneBlade(), 50);
	utricShop->addItem(spell_Frostbolt(), 50);
	utricShop->addItem(spell_GottricsArcaneProtection(), 100);
	allShops.push_back(utricShop);
	//	Waterlogged Writings
	shopSharedPtr waterlogged = shopSharedPtr(new shop("waterlogged_writings"));
	waterlogged->addItem(spell_AcidBlade(), 50);
	waterlogged->addItem(spell_AcidBurst(), 75);
	waterlogged->addItem(spell_AcidSpit(), 50);
	allUnlockableShops.push_back(waterlogged);
	//	Minas's Profaned Writings
	shopSharedPtr profWrit = shopSharedPtr(new shop("minas_profaned_writings"));
	profWrit->addItem(spell_ProfanedBlade(), 100);
	profWrit->addItem(spell_VoidJaunt(), 100);
	profWrit->addItem(spell_DevouringVoidCloud(), 150);
	allUnlockableShops.push_back(profWrit);
	//	Frostbitten Writings
	shopSharedPtr frostbitten = shopSharedPtr(new shop("frostbitten_writings"));
	frostbitten->addItem(spell_FrostBlast(), 100);
	frostbitten->addItem(spell_FrozenBlade(), 100);
	frostbitten->addItem(spell_Chillbite(), 100);
	allUnlockableShops.push_back(frostbitten);
	//	Singed Writings
	shopSharedPtr singed = shopSharedPtr(new shop("singed_writings"));
	singed->addItem(spell_LightningStrike(), 100);
	singed->addItem(spell_ArcLightning(), 100);
	singed->addItem(spell_ElectricBlade(), 100);
	allUnlockableShops.push_back(singed);

	//Muira
	shopSharedPtr muiraShop = shopSharedPtr(new shop("muira_shop"));
	muiraShop->addItem(charm_ClericsHolyPendant(), 100);
	muiraShop->addItem(prayer_RayOfLight(), 25);
	muiraShop->addItem(prayer_BlessedRadiance(), 50);
	muiraShop->addItem(prayer_RemovePoison(), 25);
	allShops.push_back(muiraShop);
	//	Orsyl's Tome of Prayer
	shopSharedPtr orsyl = shopSharedPtr(new shop("orsyls_tome_of_prayer"));
	orsyl->addItem(prayer_ProfaneRadiance(), 50);
	orsyl->addItem(prayer_DrawOutTheBlood(), 75);
	orsyl->addItem(prayer_Intoxify(), 50);
	allUnlockableShops.push_back(orsyl);
	//	Divine Moonspark Tome
	shop* moonspark = new shop("divine_moonspark_tome");
	moonspark->addItem(prayer_DivineRetribution(), 100);
	moonspark->addItem(prayer_YutriasDivineSpark(), 100);
	moonspark->addItem(prayer_YutriasProtection(), 100);
	allUnlockableShops.push_back(shopSharedPtr(moonspark));
	//	Divine Tome of the Emissary
	shop* emissary = new shop("divine_tome_of_the_emissary");
	emissary->addItem(prayer_DivineRestoration(), 100);
	emissary->addItem(prayer_DivineJudgement(), 100);
	allUnlockableShops.push_back(shopSharedPtr(emissary));
	//	Divine Nightmare Tome
	shop* nightmare = new shop("divine_nightmare_tome");
	nightmare->addItem(prayer_NightmarePrayer(), 150);
	nightmare->addItem(prayer_WordOfUnmaking(), 150);
	allUnlockableShops.push_back(shopSharedPtr(nightmare));
	//	Tome of the Dead
	shop* dead = new shop("tome_of_the_dead");
	dead->addItem(prayer_Deathlink(), 100);
	dead->addItem(prayer_ProfoundStill(), 100);
	allUnlockableShops.push_back(shopSharedPtr(dead));

	//Elena
	shop* elena = new shop("elena_shop");
	elena->addItem(ranged_ThrowingKnives(), 50);
	elena->addItem(ranged_LaceratingKnives(), 75);
	elena->addItem(ranged_WitchsJar(), 75);
	elena->addItem(consumable_TinyGreenFlower(), 75);
	elena->addItem(consumable_InvigoratingTea(), 150);
	allShops.push_back(shopSharedPtr(elena));
	//With Piece of Jade
	shop* jade = new shop("piece_of_jade");
	jade->addItem(consumable_TinyGreenFlower(), 50);
	jade->addItem(ranged_PyromancersFlask(), 50);
	jade->addItem(ranged_CorrodingJar(), 50);
	jade->addItem(ranged_HeavyJavelin(), 100);
	jade->addItem(ranged_LightingJavelin(), 100);
	jade->addItem(ranged_FrostKnives(), 100);
	allUnlockableShops.push_back(shopSharedPtr(jade));
	//With Piece of Bone
	shop* bone = new shop("piece_of_bone");
	bone->addItem(consumable_PutridFlower(), 100);
	bone->addItem(consumable_InvigoratingTea(), 500);
	bone->addItem(ranged_WarpingJavelin(), 100);
	bone->addItem(headgear_PilgrimsHood(), 300);
	bone->addItem(armour_PilgrimsCoat(), 500);
	allUnlockableShops.push_back(shopSharedPtr(bone));

	//Ydella
	shop* ydella = new shop("ydella_shop");
	allShops.push_back(shopSharedPtr(ydella));
	//	Wretched Heart
	shop* wretched = new shop("wretched_heart", true);
	wretched->addItem(charm_WretchedFleshmask(), 200);
	allUnlockableShops.push_back(shopSharedPtr(wretched));
	//	Old Crow's Heart
	shop* old_crow = new shop("old_crows_heart", true);
	old_crow->addItem(weapon_OldCrowsLongKnife(), 200);
	allUnlockableShops.push_back(shopSharedPtr(old_crow));
	//	Fishborn Goddess Heart
	shop* fishborn = new shop("fishborn_heart", true);
	fishborn->addItem(prayer_SinkBeneath(), 200);
	allUnlockableShops.push_back(shopSharedPtr(fishborn));
	//	Orsyl's Heart
	shop* or = new shop("orsyls_heart", true);
	or->addItem(chime_OrsylsProfaneChime(), 300);
	allUnlockableShops.push_back(shopSharedPtr(or));
	//	Vorten's Heart
	shop* vorten = new shop("vortens_heart", true);
	vorten->addItem(weapon_ArmOfTheDuke(), 300);
	allUnlockableShops.push_back(shopSharedPtr(vorten));
	//	Venomous Spider's Heart
	shop* spider = new shop("venomous_spider_heart", true);
	spider->addItem(weapon_SpiderboneShard(), 150);
	spider->addItem(prayer_SpidersPrayer(), 150);
	allUnlockableShops.push_back(shopSharedPtr(spider));
	//	Lady Tvert's Heart
	shop* tvert = new shop("lady_tverts_heart", true);
	tvert->addItem(weapon_LadyTvertsClaws(), 200);
	allUnlockableShops.push_back(shopSharedPtr(tvert));
	//	Dead Sparrow's Heart
	shop* sparrow = new shop("dead_sparrows_heart", true);
	sparrow->addItem(wand_SparrowsStaff(), 400);
	allUnlockableShops.push_back(shopSharedPtr(sparrow));
	//	Coren's Heart
	shop* coren = new shop("corens_heart", true);
	coren->addItem(weapon_CorensGreataxe(), 400);
	coren->addItem(charm_FrenzyCharm(), 500);
	allUnlockableShops.push_back(shopSharedPtr(coren));
	//	Khalle's Heart
	shop* khalle = new shop("khalles_heart", true);
	khalle->addItem(weapon_KhallesHolyScythe(), 400);
	khalle->addItem(prayer_TouchOfDoom(), 300);
	allUnlockableShops.push_back(shopSharedPtr(khalle));
	//	Ietra's Heart
	shop* ietra = new shop("ietras_heart", true);
	ietra->addItem(weapon_IetrasFlameScythe(), 400);
	ietra->addItem(spell_Firestorm(), 400);
	allUnlockableShops.push_back(shopSharedPtr(ietra));
	//	Pale Heart
	shop* pale = new shop("pale_heart", true);
	pale->addItem(weapon_MoonpaleScythe(), 500);
	pale->addItem(charm_PaleShadesRing(), 100);
	allUnlockableShops.push_back(shopSharedPtr(pale));
	//	Plague-Ridden Heart
	shop* plague = new shop("plague_ridden_heart", true);
	plague->addItem(weapon_SelvixsHookblade(), 200);
	allUnlockableShops.push_back(shopSharedPtr(plague));
	//Heart of Farin
	shop* farin = new shop("heart_of_farin", true);
	farin->addItem(spell_LightOfFarin(), 200);
	allUnlockableShops.push_back(shopSharedPtr(farin));
	//Rat King's Heart
	shop* rat = new shop("rat_kings_heart", true);
	rat->addItem(weapon_RatboneCleaver(), 300);
	allUnlockableShops.push_back(shopSharedPtr(rat));

}


/*
Finds the shop with the corresponding nametag.
*/
shopSharedPtr game::getShopByTag(std::string tag)
{
	for (auto s : allShops) {
		if (s->tag == tag)
			return s;
	}
	return nullptr;
}


//Look for someone to talk to
void game::talkToNPC()
{
	int r = 1;
	for (int x = player->getx() - r; x <= player->getx() + r; x++)
	{
		for (int y = player->gety() - r; y <= player->gety() + r; y++)
		{
			if (x != player->getx() || y != player->gety()) 
			{
				monsterSharedPtr target = std::static_pointer_cast<monster>(currentMap->getPerson(x, y));
				if (target != nullptr)
				{
					//Talking
					if (target->hasDialogue()) {
						doDialogue(target);
						return;
					}
					//Shopping, no talking
					if (target->isShopkeeper) {
						setupShopMenu(target);
						return;
					}
				}
			}
		}
	}
}

void game::drawDialogueLine(personSharedPtr target, std::string line)
{
	//Box it in
	win.clearRegion(MAP_DRAW_X, MAP_DRAW_Y, 40, 15);
	win.drawBox(MAP_DRAW_X, MAP_DRAW_Y, 40, 15, TCODColor::sepia);

	//Controls
	int atx = MAP_DRAW_X + 1;
	int aty = MAP_DRAW_Y + 1;
	std::string txt = "[SPACE] Chat   [ESCAPE] Goodbye";
	win.write(atx, aty, centreText(txt, 38), TCODColor::white);

	//NPC name
	aty += 2;
	win.write(atx, aty, centreText(target->getName(), 38), target->getColor());

	//Write dialogue!
	aty += 4;
	win.writeWrapped(atx, aty, 38, line, TCODColor::lightGrey);

	//Done drawing!
	win.refresh();
}


/*
Chitchat with an npc!
*/
void game::doDialogue(monsterSharedPtr target)
{
	while (key.vk != KEY_BACK_OUT) {

		//Draw the screen underneath
		drawScreen(false);

		drawDialogueLine(target, target->getCurrentDialogueLine());

		//Check for input
		TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key, &mouse);
		if (!key.pressed && key.vk == KEY_ACCEPT) {
			target->getNextDialogueLine();
			//See if anything special happens (and if we should end the dialogue as well)
			if (checkForDialogueEvent(target->getCurrentDialogueLine(), target))
				return;
		}

	}
}

/*
Sometimes special things happen when we're chatting with a friendly NPC.
They're triggered by special codes embedded in dialogue!
If we return True, the dialogue should end.
*/
bool game::checkForDialogueEvent(std::string line, monsterSharedPtr target)
{
	if (line == "|SHOP") {
		target->backUpDialogue(); //Go back to the previous line of dialogue
		setupShopMenu(target);
		//Shopping always puts a stop to talking
		return true;
	}
	else if (line == "|STORY_FLAG") {
		//The next line after this is a story flag!
		addStoryFlag(target->getNextDialogueLine());
		//Progress to the next line
		return checkForDialogueEvent(target->getNextDialogueLine(), target);
	}
	else if (line == "|STORY_FLAG_SKIP") {
		std::string storyFlag = target->getNextDialogueLine();
		std::string lineName = target->getNextDialogueLine();
		//If we have the provided story flag, skip ahead in the dialogue.
		if (hasStoryFlag(storyFlag)) {
			//WE HAVE IT. Now look for the line name
			while (target->getNextDialogueLine() != lineName) {}
		}
		//Progress to next line
		return checkForDialogueEvent(target->getNextDialogueLine(), target);
	}
	else if (line == "|END_DIALOGUE") {
		//Automatically ends the dialogue
		target->getNextDialogueLine();
		return true;
	}
	else if (line == "|BACK_UP_AND_END") {
		//End the dialogue and return to the previous line.
		target->backUpDialogue();
		return true;
	}
	return false;
}

/*
Displaying the shopping times!
*/
void game::drawShopMenu(int atx, int aty)
{
	//The SHOPPING LIST
	drawMenu(currentMenu, atx, aty);
	//Description of items for purchase
	aty += currentMenu->getAllElements().size() + 10;
	itemSharedPtr sel = std::static_pointer_cast<item>(currentMenu->getSelectedItem());
	if (sel != nullptr)
		drawItemInfo(sel, ITEM_DRAW_X, ITEM_DRAW_Y);
}


/*
Checks to see if the player has unlocked any of this shopkeeper's extra inventories.
If he has, appends them.
*/
void game::checkForStockUnlocks(monsterSharedPtr shopkeeper)
{
	//Loop through all the shopkeeper's unlocks
	for (auto unlock : shopkeeper->getShopUnlocks()) {

		//If player has the key corresponding to this unlock, UNLOCK IT
		if (player->hasKey(unlock)) {

			//Try to match them to shops we have
			for (auto iter = allUnlockableShops.begin(); iter != allUnlockableShops.end(); iter++) {

				//Compare
				if ((*iter)->tag == unlock) {

					//A match! Combine with the present shop
					auto currentShop = getShopByTag(shopkeeper->getShopTag());
					for (auto it : (*iter)->stock) {
						currentShop->addItem(it, it->getPrice());
					}
					
					//And then delete this unlock, cuz we can only unlock it once
					allUnlockableShops.erase(iter);

					//Is there special dialogue about this?!
					std::string specialTxt = shopkeeper->getDialogueForKey(unlock);
					if (specialTxt.size() > 0) {
						drawDialogueLine(shopkeeper, specialTxt);
						win.refresh();
						while (win.getkey().vk != KEY_ACCEPT) {}
					}
					
					//And we're done; stop iterating
					break;

				}

			}

		}

	}
}


/*
Create menu for SHOPPING!
*/
void game::setupShopMenu(personSharedPtr shopkeeper)
{
	monsterSharedPtr m = std::static_pointer_cast<monster>(shopkeeper);
	//Update shop inventory
	checkForStockUnlocks(m);
	//Create shopping menu
	currentMenu = new menu(m->getName());
	auto sh = getShopByTag(m->getShopTag());
	for (auto it : sh->stock) {
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
	itemSharedPtr it = std::static_pointer_cast<item>(currentMenu->getSelectedItem());
	shopSharedPtr currentShop = getShopByTag(currentShopkeeper->getShopTag());
	if (it != nullptr) {
		if (fragments >= it->getPrice()) {
			//Pay the COST
			fragments -= it->getPrice();
			//But get the ITEM
			pickUpItem(it);
			//Remove from menu and shopkeeper's inventory
			currentShop->removeItem(it);
			currentMenu->removeElement(it);
			//Player might lose a key as a consequence
			if (it->eatsKeyWhenBought) {
				player->loseKey(it->getKeyEaten());
				//Also lose other items that eat this key
				destroyShopItemsWithKey(currentShop, it->getKeyEaten());
			}
		}
	}
}


/*
Remove items from the current shop that also correspond to this key.
*/
void game::destroyShopItemsWithKey(shopSharedPtr currentShop, std::string keyTag)
{
	itemVector toRemove;
	
	//Make a list of items that use this key
	for (auto iter = currentShop->stock.begin(); iter != currentShop->stock.end(); iter++) {
		if ((*iter)->eatsKeyWhenBought && (*iter)->getKeyEaten() == keyTag) {
			toRemove.push_back(*iter);
		}
	}
	
	//KILL THEM ALL
	for (auto it : toRemove) {
		currentShop->removeItem(it);
		currentMenu->removeElement(it);
	}

	//We keep a list of items destroyed, just in case the player regains the key
	shopSharedPtr saveShop = shopSharedPtr(new shop(keyTag, true));
	for (auto it : toRemove)
		saveShop->addItem(it, it->getPrice());
	allUnlockableShops.push_back(saveShop);

}




/*
	STORY EVENTS (ie NPC movement)
*/


/*
Add a new story flag, and check for an associated event!
*/
void game::addStoryFlag(std::string f)
{
	//Add flag
	storyFlags.push_back(f);
	//See if we should be queuing an event
	for (auto iter = storyEventsWaiting.begin(); iter != storyEventsWaiting.end(); iter++) {
		if ((*iter).storyFlag == f) {
			//Yup, this is the one!
			queueStoryEvent(*iter);
			storyEventsWaiting.erase(iter);
			break;
		}
	}
}


/*
Returns whether we have the specified story flag.
*/
bool game::hasStoryFlag(std::string f)
{
	return std::find(storyFlags.begin(), storyFlags.end(), f) != storyFlags.end();
}


/*
Load in all story events and their triggers from a file.
*/
void game::loadStoryEvents(std::string filename)
{
	
	//Open file
	std::ifstream storyfile(filename);
	//Stuff we need to keep track of
	std::string storyFlag;
	std::string monsterTag;
	std::string mapFlag;
	coord pt;
	//Read in events, one at a time
	std::string line;
	std::string chunk;
	bool buildingEvent = false;
	
	while (getline(storyfile, line)) {

		//Make sure the line actually has a size
		if (line.size()) {

			if (line.at(0) == '!') {
				
				//If the first character is a '!', this is the start or end of a chunk
				buildingEvent = !buildingEvent;
				if (buildingEvent) {
					//The rest of the line is the STORY FLAG
					storyFlag = line.substr(1, line.size());
				}
				else {
					//We're finished building, save this event
					storyEvent ev(storyFlag, mapFlag, monsterTag, pt);
					saveStoryEvent(ev);
				}
			
			}

			else if (buildingEvent) {
				
				//Substring of first 4 characters tells us what item we're adding
				std::string title = line.substr(0, 3);
				std::string content = line.substr(4, line.size());
				if (title == "map")
					mapFlag = content;
				else if (title == "npc")
					monsterTag = content;
				else if (title == "spt") {
					//Read in coordinates
					int c = 4;
					//x-coord
					std::string chunk = "";
					while (line.at(c) != ',') {
						chunk += line.at(c);
						c++;
					}
					pt.first = std::stoi(chunk);
					//y-coord
					chunk = line.substr(c + 1, line.size());
					pt.second = std::stoi(chunk);
				}

			}
		}
	}
	//Done, close file
	storyfile.close();
}




/*
Loads up the blocks of text that are displayed when certain maps load.
*/
void game::loadTextDumps(std::string filename)
{
	std::ifstream txtfile(filename);
	std::string line;
	//Text we're currently constructing
	areaText* txt = new areaText();
	//Read em in
	while (getline(txtfile, line)) {
		if (line.size() > 0) {
			//End of chunk?
			if (line == "!end") {
				areaTextList.push_back(txt);
				txt = new areaText();
			}
			//New chunk?
			else if (line.at(0) == '!') {
				//The rest of the line is the name of the map file
				std::string mapTag = line.substr(1, line.size());
				//Convert to proper file name & save
				mapTag = makemap.getMapHandle(mapTag);
				txt->mapTag = mapTag;
			}
			//Title?
			else if (line.at(0) == '+') {
				std::string title = line.substr(1, line.size());
				txt->title = title;
			}
			//Otherwise, text chunk
			else {
				txt->text = line;
			}
		}
	}
	//Done, close file
	txtfile.close();
}


/*
Checks for & displays area text, if relevant!
*/
void game::checkForAreaText(std::string mapTag)
{
	areaText* txt = findAreaText(mapTag);
	if (txt != nullptr) {
		//Display it!
		int atx = MAP_DRAW_X - 1;
		int aty = MAP_DRAW_Y - 1;
		win.clearRegion(atx, aty, 41, 41);
		win.drawBox(atx, aty, 41, 41, TCODColor::sepia);
		//Title
		atx += 1;
		aty += 1;
		win.write(atx, aty, centreText(txt->title, 38), TCODColor::lighterBlue);
		//Text itself
		aty += 2;
		aty = win.writeWrapped(atx, aty, 38, txt->text, TCODColor::lighterGrey);
		//Refresh and wait
		aty = MAP_DRAW_Y + 38;
		win.write(atx, aty, centreText("[PRESS ESC]", 38), TCODColor::white);
		win.refresh();
		while (win.getkey().vk != KEY_BACK_OUT) {}
	}
}


/*
Returns area text for the given area, if it exists.
*/
areaText* game::findAreaText(std::string mapTag)
{
	for (auto txt : areaTextList) {
		if (txt->mapTag == mapTag)
			return txt;
	}
	return nullptr;
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
	//Map updates
	currentMap->progressFog();
}

/*
Removes all dead creatures from the map.
*/
void game::clearDeadCreatures()
{

	//If the player is dead, SPECIAL STUFF happens
	if (player->isDead) {
		//Message about our death
		win.write(MAP_DRAW_X + 15, MAP_DRAW_Y + 20, "Y O U   D I E D", TCODColor::lightestRed);
		win.refresh();
		while (win.getkey().vk != KEY_ACCEPT) {}
		//We lose all of our fragments
		fragments = 0;
		//Reload game
		restoreFromSavePoint();
	}
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
				monsterSharedPtr b = std::static_pointer_cast<monster>(p);
				if (b->showBossHealthBar) {
					bossKillMessage();
				}
				currentBoss = nullptr;
				//Toggle setting on map so the boss won't respawn
				currentMap->bossDestroyed = true;
			}
			//And the PLAYER gets the ITEMS we drop!
			getDeathDrops(std::static_pointer_cast<monster>(p));
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
	//Set this as our save point
	ourSavePt.saveMap = currentMap;
	ourSavePt.savePt = player->getPosition();
	//And reload the map around us
	restoreFromSavePoint();
}

/*
Returns player to save point.
*/
void game::restoreFromSavePoint(savePoint* warpTo)
{
	
	//Clear all animations
	playingAnimations.clear();
	
	//If no point is provided, we warp to our main save point
	if (warpTo == nullptr)
		warpTo = &ourSavePt;
	
	//Resurrect player
	deletePlayerBuffs();
	player->fullRestore();
	player->isDead = false;
	player->setTarget(nullptr);
	isAutoWalking = false; //Make sure we don't keep autowalking on death!
	
	//Return us to our save point
	loadNewMap(warpTo->saveMap, CONNECT_VERTICAL, warpTo->savePt.first, warpTo->savePt.second);
	
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
	player->clearBuffs();
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


void getAllItems(personSharedPtr player)
{
	
	player->addItem(weapon_ArmOfTheDuke());
	player->addItem(weapon_BloodDrinkersKnife());
	player->addItem(weapon_CityGuardianWarhammer());
	player->addItem(weapon_CorensGreataxe());
	player->addItem(weapon_CrimsonNail());
	player->addItem(weapon_CrowHalfsword());
	player->addItem(weapon_CrowKnife());
	player->addItem(weapon_CrowKnightSword());
	player->addItem(weapon_DragonboneSword());
	player->addItem(weapon_EtherealGreatsword());
	player->addItem(weapon_EtherealSword());
	player->addItem(weapon_FishmansHarpoon());
	player->addItem(weapon_FishmansKnife());
	player->addItem(weapon_IetrasFlameScythe());
	player->addItem(weapon_KhallesHolyScythe());
	player->addItem(weapon_KythielsScythe());
	player->addItem(weapon_LadyTvertsClaws());
	player->addItem(weapon_LightningWarhammer());
	player->addItem(weapon_MoonpaleScythe());
	player->addItem(weapon_NotchedGreatsword());
	player->addItem(weapon_SacrificialKnife());
	player->addItem(weapon_SelvixsHookblade());
	player->addItem(weapon_SentinelsPike());
	player->addItem(weapon_SerpentsTooth());
	player->addItem(weapon_SirPercivelsSword());
	player->addItem(weapon_SlaveMastersWhip());
	player->addItem(weapon_SpiderboneShard());
	player->addItem(weapon_SplinteredSword());
	player->addItem(weapon_StraightSword());
	player->addItem(weapon_ThinKnife());
	player->addItem(weapon_VoidCrystalGreatsword());
	player->addItem(weapon_VoidfireKnife());
	player->addItem(weapon_VoidTouchedKnife());
	player->addItem(weapon_Warhammer());

	player->addItem(shield_BatteredSteelShield());
	player->addItem(shield_BatteredWoodenShield());
	player->addItem(shield_CityGuardianShield());
	player->addItem(shield_DragonboneShield());
	player->addItem(shield_EtherealShield());
	player->addItem(shield_KhallesBellShield());
	player->addItem(shield_GhostlyShield());
	player->addItem(shield_RottenBeastShield());
	player->addItem(shield_SirPercivelsShield());
	player->addItem(shield_VoidTouchedShield());
	player->addItem(shield_WoodenWyrdShield());

	player->addItem(chime_ClericsCrackedChime());
	player->addItem(chime_GreenKnightsChime());
	player->addItem(chime_OrsylsProfaneChime());
	player->addItem(chime_RotbloodBell());
	player->addItem(chime_WyrdBellbranch());

	player->addItem(wand_BleachwoodWand());
	player->addItem(wand_DriftwoodWand());
	player->addItem(wand_EtherealWand());
	player->addItem(wand_FishmansToadstaff());
	player->addItem(wand_GottricsWand());
	player->addItem(wand_MoshkasSingingStaff());
	player->addItem(wand_SparrowsStaff());

	player->addItem(headgear_CaptainsTricorn());
	player->addItem(armour_RuinedUniform());
	player->addItem(headgear_RuinedKnightsHelm());
	player->addItem(armour_RuinedKnightsArmour());
	player->addItem(headgear_CrowKnightsHood());
	player->addItem(armour_CrowKnightsArmour());
	player->addItem(headgear_FishpriestHat());
	player->addItem(armour_FishscaleCoat());
	player->addItem(headgear_ClericsHood());
	player->addItem(armour_ClericsVestments());
	player->addItem(headgear_CityGuardHelm());
	player->addItem(armour_CityGuardArmour());
	player->addItem(headgear_PashHood());
	player->addItem(armour_PashRobes());
	player->addItem(headgear_CursedKnightsHelm());
	player->addItem(armour_CursedKnightsArmour());
	player->addItem(headgear_GreyThiefsHood());
	player->addItem(armour_GreyThiefsRags());
	player->addItem(headgear_EtherealCrown());
	player->addItem(armour_EtherealRobes());
	player->addItem(headgear_SparrowKnightsHelm());
	player->addItem(armour_SparrowKnightsArmour());
	player->addItem(headgear_SentinelsHelm());
	player->addItem(armour_SentinelsArmour());
	player->addItem(headgear_GhostlyHelm());
	player->addItem(armour_GhostlyArmour());
	player->addItem(headgear_GravekeepersHood());
	player->addItem(armour_GravekeepersRags());
	player->addItem(headgear_PilgrimsHood());
	player->addItem(armour_PilgrimsCoat());
	player->addItem(headgear_SirPercivelsHelm());
	player->addItem(armour_SirPercivelsArmour());

	player->addItem(charm_ArcanaDrenchedCharm());
	player->addItem(charm_BloodDrinkersBand());
	player->addItem(charm_BloodstainedCharm());
	player->addItem(charm_ClericsHolyPendant());
	player->addItem(charm_EvisceratingRing());
	player->addItem(charm_FragrantAmulet());
	player->addItem(charm_FrenzyCharm());
	player->addItem(charm_FrozenFlowerCharm());
	player->addItem(charm_IdolOfPash());
	player->addItem(charm_KhallesHeadband());
	player->addItem(charm_PaleShadesRing());
	player->addItem(charm_RavenousIdol());
	player->addItem(charm_SirPercivelsRing());
	player->addItem(charm_ToxicantsCharm());
	player->addItem(charm_VenomrubyRing());
	player->addItem(charm_VoidwalkersRing());
	player->addItem(charm_WretchedFleshmask());

	player->addItem(spell_AcidBlade());
	player->addItem(spell_AcidBurst());
	player->addItem(spell_AcidSpit());
	player->addItem(spell_ArcaneBlade());
	player->addItem(spell_ArcaneRadiance());
	player->addItem(spell_ArcLightning());
	player->addItem(spell_AtalundraArcaneSpear());
	player->addItem(spell_AtalundraDeathSurge());
	player->addItem(spell_AtalundraSoulBlade());
	player->addItem(spell_Chillbite());
	player->addItem(spell_DevouringVoidCloud());
	player->addItem(spell_ElectricBlade());
	player->addItem(spell_FrostBlast());
	player->addItem(spell_Frostbolt());
	player->addItem(spell_FrozenBlade());
	player->addItem(spell_GottricsArcaneProtection());
	player->addItem(spell_LightOfFarin());
	player->addItem(spell_LightningStrike());
	player->addItem(spell_MagicMissile());
	player->addItem(spell_ProfanedBlade());
	player->addItem(spell_VoidJaunt());

	player->addItem(prayer_BlessedRadiance());
	player->addItem(prayer_Deathlink());
	player->addItem(prayer_DivineRetribution());
	player->addItem(prayer_DrawOutTheBlood());
	player->addItem(prayer_Intoxify());
	player->addItem(prayer_NightmarePrayer());
	player->addItem(prayer_ProfaneRadiance());
	player->addItem(prayer_ProfoundStill());
	player->addItem(prayer_RayOfLight());
	player->addItem(prayer_RemovePoison());
	player->addItem(prayer_Restoration());
	player->addItem(prayer_SpidersPrayer());
	player->addItem(prayer_WordOfUnmaking());
	player->addItem(prayer_WyrdChantOfStrength());
	player->addItem(prayer_YutriasDivineSpark());
	player->addItem(prayer_YutriasProtection());

	player->addItem(consumable_BlackHoney());
	player->addItem(consumable_BloodDrinkersEyes());
	player->addItem(consumable_IntoxicatingWine());
	player->addItem(consumable_InvigoratingTea());
	player->addItem(consumable_TinyGreenFlower());
	player->addItem(consumable_TinyRedFlower());
	player->addItem(consumable_WitchwaterFlask());
	player->addItem(horn_HeraldsWhiteHorn());

	player->addItem(ranged_CorrodingJar());
	player->addItem(ranged_FrostKnives());
	player->addItem(ranged_HeavyJavelin());
	player->addItem(ranged_LaceratingKnives());
	player->addItem(ranged_LightingJavelin());
	player->addItem(ranged_PoisonThrowingKnives());
	player->addItem(ranged_PyromancersFlask());
	player->addItem(ranged_ThrowingKnives());
	player->addItem(ranged_WarpingJavelin());
	player->addItem(ranged_WitchsJar());
	player->addItem(ranged_VoidEssenceJar());

	player->addItem(oil_CorrosiveOil());
	player->addItem(oil_CursedWater());
	player->addItem(oil_DeepRedOil());
	player->addItem(oil_FrozenOil());
	player->addItem(oil_HangmansBlood());
	player->addItem(oil_HolyWater());
	player->addItem(oil_PyromancersOil());

	player->addItem(runestone_BloodDrenchedRunestone());
	player->addItem(runestone_CharredRunestone());
	player->addItem(runestone_CorensRunestone());
	player->addItem(runestone_CorrodingRunestone());
	player->addItem(runestone_FrozenRunestone());
	player->addItem(runestone_IetrasRunestone());
	player->addItem(runestone_KhallesRunestone());
	player->addItem(runestone_KinslayersRunestone());
	player->addItem(runestone_SiltrasRunestone());
	player->addItem(runestone_StarweaversRunestone());
	player->addItem(runestone_ThundrousRunestone());

	player->addItem(key_DeadSparrowKey());
	player->addItem(key_FarinsKey());
	player->addItem(key_GreenChapelGardenKey());
	player->addItem(key_HightowerKey());
	player->addItem(key_LadyTvertsKey());
	player->addItem(key_MoonpaleKey());
	player->addItem(key_MoshkasKey());
	player->addItem(key_OldCrowsKey());
	player->addItem(key_RuinedTownshipKey());
	player->addItem(key_SordidChapelKey());
	player->addItem(key_SpinalColumnShard());
	player->addItem(key_WretchsKey());
	player->addItem(key_WyrdKey());
	player->addItem(misc_VoidSigil());

	player->addItem(bones_CorensBones());
	player->addItem(bones_KhallesBones());
	player->addItem(bones_SiltrasBones());
	player->addItem(bones_IetrasBones());

	player->addItem(heart_FishBornGoddessesHeart());
	player->addItem(heart_LadyTvertsHeart());
	player->addItem(heart_OldCrowsHeart());
	player->addItem(heart_OrsylsShriveledHeart());
	player->addItem(heart_PaleHeart());
	player->addItem(heart_PlagueRiddenHeart());
	player->addItem(heart_VenomousSpiderHeart());
	player->addItem(heart_VortensShriveledHeart());
	player->addItem(heart_WretchedHeart());
	player->addItem(heart_HeartOfTheOldGods());

	player->addItem(key_WatchfulEyestalk());
	player->addItem(key_WitheredFinger());
	player->addItem(key_BeatingHeart());
	player->addItem(key_MawtoothFragment());
	player->addItem(key_EffigyOfTheSlumberingLord());

	for (int i = 0; i <= 9; i++)
		player->addItem(consumable_StarwaterDraught());

}


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
	else if (txt == "item") {
		txt = win.getstr(1, 1);
		//Get item by name
		itemSharedPtr it = getItemByHandle(txt);
		if (it != nullptr)
			player->addItem(it);
	}
	else if (txt == "story") {
		txt = win.getstr(1, 1);
		//Add a story flag
		addStoryFlag(txt);
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
		fragments += 125;
		loadMapFromHandle("maps/cbeach_2.txt", CONNECT_WARP, player->getx(), player->gety());
	}

	else if (txt == "allitems") {
		getAllItems(player);
		fragments += 15000;
	}
	else if (txt == "finalitems") {
		player->addItem(consumable_PutridBrew());
		player->addItem(consumable_GodsbloodBrew());
		player->addItem(charm_VenomousImbecilesRing());
		player->addItem(charm_IconOfFamine());
		player->addItem(charm_AtalundraPactRing());
		player->addItem(charm_DivinePactRing());
		player->addItem(charm_AgonyRing());
		player->addItem(key_IetrasAmberPendant());
	}

}



/*
	SAVING AND LOADING
*/


/*
Generate a new save game.
*/
void game::saveGame()
{

	//Save object
	savegame* newSave = new savegame(allMapHandles, allMaps, currentMap->getMapTag(), player->getPosition());

	//Dump all this information into a big ole' file.
	newSave->dumpToFile("currentsave");

}


/*
This loads a save from a given file.
*/
void game::loadSaveGame(std::string fname)
{
	
	//Generate savegame
	savegame* sg = new savegame(fname);
	
	//	Translate save game into actual reality
	
	//Load up all the maps we have saved up
	for (auto mapTag : sg->getAllMapTags()) {

		map* newmap = new map();
		newmap = makemap.loadMapFromFile(mapTag);
		newmap->respawnAllMonsters(storyEventsReady);
		addKnownMap(newmap, mapTag);

	}

	//Character create
	player = personSharedPtr(new person());
	player->isPlayer = true;
	player->isHostile = false;
	player->stats = new statline(1, 1, 1, 1, 1, 1, 1);

	//Load in story events
	loadStoryEvents("maps/storyFlags.txt");
	loadTextDumps("dialogue/journalText.txt");

	//Set up store inventories
	initializeShops();
	
	//We start on a given map
	loadMapFromHandle(sg->getStartMapTag(), CONNECT_WARP, 0, 0);
	player->setPosition(sg->getStartPosition());

	currentMap->addPerson(player, player->getx(), player->gety());
	currentMap->updateFOV(player->getx(), player->gety());

	//Make our starting position a save point
	setSavePoint();

	//Add monsters to clock
	for (auto m : currentMap->getAllPeople()) {
		if (!m->isPlayer)
			turns.addEntity(m, 1);
	}

}
