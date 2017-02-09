

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

typedef std::shared_ptr<charm> charmSharedPtr;


//Defined items
charmSharedPtr charm_ArcanaDrenchedCharm();
charmSharedPtr charm_ClericsHolyPendant();
charmSharedPtr charm_BloodstainedCharm();
charmSharedPtr charm_KhallesHeadband();
charmSharedPtr charm_IdolOfPash();
charmSharedPtr charm_EvisceratingRing();
charmSharedPtr charm_BloodDrinkersBand();
charmSharedPtr charm_WretchedFleshBand();
charmSharedPtr charm_WretchedFleshmask();
charmSharedPtr charm_FrozenFlowerCharm();
charmSharedPtr charm_ToxicantsCharm();
charmSharedPtr charm_ObscuringCharm();
charmSharedPtr charm_FragrantAmulet();
charmSharedPtr charm_RavenousIdol();

#endif