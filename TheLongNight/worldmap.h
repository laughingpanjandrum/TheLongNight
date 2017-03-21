
#ifndef WORLDMAP_H
#define WORLDMAP_H


#include <string>
#include <vector>
#include <memory>

#include "libtcod.hpp"
#include "map.h"


struct zone
{
	zone() {}
	zone(std::string name, TCODColor color, std::string description = "") :
		name(name), color(color), description(description) {}

	//Flavour
	std::string name;
	TCODColor color;
	std::string description;

};

typedef std::vector<zone*> zoneVector1d;
typedef std::vector<zoneVector1d> zoneVector2d;


class worldmap
{
public:

	worldmap();
	~worldmap() {}

	zoneVector2d getZones() { return zones; }

protected:

	zoneVector2d zones;
	void addZone(std::string name, TCODColor color, int atx, int aty, std::string description = "");

	//Hardcoded area descriptions
	const static std::string STARDRIFT_WRECKAGE_TXT;
	const static std::string CORUSCATING_BEACH_TXT;
	const static std::string DARKWATER_FOREST_TXT;
	const static std::string MURDERMIRE_TXT;
	const static std::string OLD_FAIRWEATHER_TXT;
	const static std::string PILGRIMS_ROAD_TXT;
	const static std::string FLOODED_LOWLANDS_TXT;
	const static std::string CRUMBLING_CITY_TXT;
	const static std::string WINTER_COURT_TXT;
	const static std::string TEAR_STAINED_VALLEY_TXT;

};


#endif