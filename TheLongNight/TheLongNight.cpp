#include "libtcod.hpp"
#include "window.h"

int main() {
	window* w = new window();
	w->drawFont();
	w->refresh();
	w->waitForKeypress();
}