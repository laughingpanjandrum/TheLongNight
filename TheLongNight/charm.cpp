#include "charm.h"



charm::~charm()
{
}

void charm::addEffect(effect e, int pot)
{
	effects.push_back(e);
	effectPotency.push_back(pot);
}
