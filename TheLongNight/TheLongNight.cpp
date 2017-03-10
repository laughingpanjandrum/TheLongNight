#include "libtcod.hpp"
#include "game.h"

int main() {

	window w;
	char c = ' ';
	while (c != 'n' && c != 'l') {
		w.clear();
		w.write(10, 10, "[n] NEW GAME", TCODColor::white);
		w.write(10, 11, "[l] LOAD GAME", TCODColor::white);
		w.refresh();
		c = w.getkey().c;
	}

	//NEW GAME
	if (c == 'n') {
		game g;
		g.mainGameLoop();
	}

	//LOAD GAME
	else {
		w.write(10, 14, "LOADING...", TCODColor::white);
		w.refresh();
		game g("currentsave");
		g.mainGameLoop();
	}
	
}