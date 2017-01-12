
#ifndef KEYCODES_H
#define KEYCODES_H

#include "libtcod.hpp"

typedef const TCOD_keycode_t keycodeConst;

keycodeConst KEY_NORTH = TCODK_UP;
keycodeConst KEY_SOUTH = TCODK_DOWN;
keycodeConst KEY_EAST = TCODK_RIGHT;
keycodeConst KEY_WEST = TCODK_LEFT;

keycodeConst KEY_CYCLE = TCODK_TAB;
keycodeConst KEY_BACK_OUT = TCODK_ESCAPE;
keycodeConst KEY_ACCEPT = TCODK_ENTER;

#endif
