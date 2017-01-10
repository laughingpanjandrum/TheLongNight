
#ifndef MONSTER_H
#define MONSTER_H

#include "person.h"

class monster: public person
{
public:
	//Constructors
	monster() : person() {}
	monster(std::string name, int tileCode, TCODColor color): person(name,tileCode,color) {}
	~monster() {}
};

//Monster definitions
monster* drownedDead();

#endif
