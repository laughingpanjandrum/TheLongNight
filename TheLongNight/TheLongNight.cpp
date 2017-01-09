#include "libtcod.hpp"
#include "window.h"

int main() {
	window* w = new window();
	w->write(10, 10, "HELLO", TCODColor::blue);
	w->refresh();
	w->getkey();
}