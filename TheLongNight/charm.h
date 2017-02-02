

#ifndef CHARM_H
#define CHARM_H


#include "item.h"

class charm : public item
{
public:

	//Constructors
	charm() : item() {}
	charm(std::string name, int tileCode, TCODColor color, std::string description) :
		item(name, tileCode, color, ITEM_CHARM, description) {}
	~charm();

	//Setting
	void addEffect(effect e, int pot);

	//Getting
	int getEffectsCount() { return effects.size(); }
	effectVector getAllEffects() { return effects; }
	std::vector<int> getAllPotencies() { return effectPotency; }

private:

	//A charm is just a bag of effects!
	effectVector effects;
	std::vector<int> effectPotency;

};

//Defined items
charm* charm_ArcanaDrenchedCharm();
charm* charm_ClericsHolyPendant();
charm* charm_BloodstainedCharm();
charm* charm_KhallesHeadband();
charm* charm_IdolOfPash();
charm* charm_EvisceratingRing();
charm* charm_BloodDrinkersBand();
charm* charm_WretchedFleshBand();
charm* charm_WretchedFleshmask();
charm* charm_FrozenFlowerCharm();
charm* charm_ToxicantsCharm();

#endif