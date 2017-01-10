
#ifndef MONSTER_H
#define MONSTER_H

#include "person.h"

class monster: public person
{
public:
	//Constructors
	monster();
	monster(std::string name, int tileCode, TCODColor color);
	~monster() {}
};

//Monster definitions
monster* drownedDead();

#endif
