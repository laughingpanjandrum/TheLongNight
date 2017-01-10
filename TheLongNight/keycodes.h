
#ifndef KEYCODES_H
#define KEYCODES_H

#include "libtcod.hpp"

typedef const TCOD_keycode_t keycodeConst;

keycodeConst KEY_NORTH = TCODK_UP;
keycodeConst KEY_SOUTH = TCODK_DOWN;
keycodeConst KEY_EAST = TCODK_RIGHT;
keycodeConst KEY_WEST = TCODK_LEFT;

keycodeConst KEY_BACK_OUT = TCODK_ESCAPE;

#endif
