
#ifndef PERSON_H
#define PERSON_H

#include "element.h"

class person: public element
{
public:
	//Constructors/destructors
	person(): person("Player", PLAYER_TILE, TCODColor::white) {}
	person(std::string name, int tileCode, TCODColor color): element(name,tileCode,color) {}
	~person();
};

#endif

