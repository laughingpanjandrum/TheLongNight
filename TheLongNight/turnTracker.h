
/*
This class is just for keeping track of turn order.
It keeps a list of persons and the delay until their turn comes up again.
*/

#ifndef TURN_TRACKER_H
#define TURN_TRACKER_H

#include <vector>
#include "person.h"

struct trackedEntity {
	trackedEntity(person* entity, int delayLeft):
		entity(entity), delayLeft(delayLeft) {}
	void reduceDelay() { delayLeft--; }
	person* entity;
	int delayLeft;
};

class turnTracker
{
	typedef std::vector<trackedEntity> trackedEntityVector;
public:

	//Constructor
	turnTracker();
	~turnTracker();

	//Setting
	void addEntity(person* entity, int atDelay);
	void clear() { tracking.clear(); }

	//Getting
	person* getNext();

private:
	trackedEntityVector tracking;
};

#endif
