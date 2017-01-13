
#ifndef MONSTER_H
#define MONSTER_H

#include "person.h"

class monster: public person
{
public:

	//Constructors
	monster();
	monster(std::string name, int tileCode, TCODColor color);
	monster(const monster& rhs) {} //Copy constructor
	~monster() {}

	//Copying
	monster* clone() const { return new monster(*this); }

	//Setting
	void setMeleeStats(int damage, int speed);
	void setMoveStats(int speed);
	void setHealth(int hp);

};

typedef std::vector<monster*> monsterVector;

//Monster definitions
monster* monster_DrownedDead();
monster* monster_ThinWretch();

monster* getMonsterByHandle(std::string handle);

#endif
