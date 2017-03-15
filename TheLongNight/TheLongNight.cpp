#include "libtcod.hpp"
#include "game.h"

int main() {

	//MAIN MENU SITUATION
	window w;
	char c = ' ';
	imagePtr TITLE = loadImage("title");
	while (c != 'n' && c != 'l' && c != 'q') {
		w.clear();
		w.drawImage(TITLE, 1, 1);
		w.write(8, 8, "T H E    L O N G    N I G H T", TCODColor::white);
		w.write(10, 10, "[n] NEW GAME", TCODColor::white);
		w.write(10, 11, "[l] LOAD GAME", TCODColor::white);
		w.write(10, 12, "[q] QUIT GAME", TCODColor::white);
		w.refresh();
		c = w.getkey().c;
	}

	//NEW GAME
	if (c == 'n') {
		game g;
		g.mainGameLoop();
	}

	//LOAD GAME
	else if (c == 'l') {
		w.write(10, 14, "LOADING...", TCODColor::white);
		w.refresh();
		game g("currentsave");
		g.mainGameLoop();
	}

	//QUIT GAME
	
}