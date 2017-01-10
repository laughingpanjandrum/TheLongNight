#include "turnTracker.h"



turnTracker::turnTracker()
{
}


turnTracker::~turnTracker()
{
}

void turnTracker::addEntity(person * entity, int atDelay)
{
	trackedEntity newTrack(entity, atDelay);
	tracking.push_back(newTrack);
}

/*
Return the character whose turn is up next.
*/
person * turnTracker::getNext()
{
	//If the list is empty, we have a problem! Return nullptr as a warning
	if (tracking.size() == 0)
		return nullptr;
	//If we find anyone with delay zero, we send them back.
	//Otherwise, we reduce everyone's delay and try again.
	person* next = nullptr;
	while (next == nullptr) {
		for (auto it = tracking.begin(); it != tracking.end(); it++) {
			if ((*it).delayLeft <= 0) {
				//Remove from list
				next = (*it).entity;
				tracking.erase(it);
				//And break out
				return next;
			}
		}
		//If we didn't return anything, reduce all delays by one and try again
		for (auto it = tracking.begin(); it != tracking.end(); it++) {
			(*it).reduceDelay();
		}
	}
	return next;
}
