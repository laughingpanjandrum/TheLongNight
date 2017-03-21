
#ifndef WORLDMAP_H
#define WORLDMAP_H


#include <string>
#include <vector>
#include <memory>

#include "libtcod.hpp"
#include "map.h"


struct mapMemoryPiece
{
	mapMemoryPiece(std::string name, TCODColor color) :
		mapName(name), mapColor(color) {}
	std::string mapName;
	TCODColor mapColor;
	bool isUnknown = true;
	mapMemoryVector1d mapConnections;
};

typedef std::shared_ptr<mapMemoryPiece> mapMemorySharedPtr;
typedef std::vector<mapMemorySharedPtr> mapMemoryVector1d;



class worldmap
{
public:

	worldmap();
	~worldmap();

protected:

	mapMemoryPiece startPt;

};

#endif