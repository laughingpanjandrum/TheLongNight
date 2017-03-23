#include "worldmap.h"



//Descriptions

const std::string worldmap::STARDRIFT_WRECKAGE_TXT = "Disintegrated ruins of the stardrift, which was meant to bear passengers \
across the astral planes, but instead became a smoking ruin. You'll have to make your own way.";

const std::string worldmap::CORUSCATING_BEACH_TXT = "The exiles of the Winter Court fled here, but salvation was not to be found \
on the forlorn shores of the Coruscating Beach.";

const std::string worldmap::DARKWATER_FOREST_TXT = "Forest patrolled by the Crow Knights, old guardians of the Wyrd, whose final \
task was the defend the Winter Court from incursion. But their efforts were in vain, for rats swarmed up from beneath.";

const std::string worldmap::MURDERMIRE_TXT = "The foreboding depths of the Murdermire house strange creatures that crawled up \
from the depths. Some claim it was due to the influence of Lady Tvert; others, that these horrors are the consequence of the \
rising scourge of the Void.";

const std::string worldmap::OLD_FAIRWEATHER_TXT = "Last settlement of the Wyrd. Sadly, none of these brave people remain, and \
there are no more pilgrims on the old road.";

const std::string worldmap::PILGRIMS_ROAD_TXT = "The northward road to the old city and the Winter Court was once guarded by \
the brave Crow Knights, but they have flagged in their duty of late.";

const std::string worldmap::FLOODED_LOWLANDS_TXT = "The former homeland of the Wyrd. If the Wyrd endure, their remnants will be \
found here, wandering the waterlogged ruins.";

const std::string worldmap::CRUMBLING_CITY_TXT = "Ruins of the old city that once guarded the Winter Court. Here the grand \
Duke Vorten watches over the slaves and subjects of Pash that roam the rubble-strewn streets.";

const std::string worldmap::WINTER_COURT_TXT = "The Winter Court, seat of civilization. Seek the Queen to end the scourge of \
the Void.";

const std::string worldmap::TEAR_STAINED_VALLEY_TXT = "It once had a different name, but whispers claim that Lady Tvert made a \
pact with the powers at the heart of the Void. Consequently, the valley has fallen into tragic disrepair.";


/*
	THIS IS HARDCODED!
	YES, I'M THE ACTUAL WORST. DEAL.
*/
worldmap::worldmap()
{

	//Initialize vector map
	for (int x = 0; x < 40; x++) {
		zoneVector1d* thisLine = new zoneVector1d();
		for (int y = 0; y < 40; y++) {
			thisLine->push_back(nullptr);
		}
		zones.push_back(*thisLine);
	}

	//CREATE THE WORLD!
	
	addZone("Stardrift Wreckage", TCODColor::lightBlue, 19, 19, STARDRIFT_WRECKAGE_TXT);
	addZone("Coruscating Beach", TCODColor::darkAmber, 20, 19, CORUSCATING_BEACH_TXT);
	addZone("Coruscating Beach", TCODColor::darkAmber, 21, 19, CORUSCATING_BEACH_TXT); //Wretched Mass
	addZone("Wretchcave", TCODColor::lightGrey, 22, 19,
		"Wretched travelers often find their end in this miserable cave.");

	addZone("Old Fairweather", TCODColor::lightSepia, 22, 18, OLD_FAIRWEATHER_TXT);

	addZone("Darkwater Forest", TCODColor::darkGreen, 22, 17, DARKWATER_FOREST_TXT);
	addZone("Darkwater Forest", TCODColor::darkGreen, 23, 17, DARKWATER_FOREST_TXT);
	addZone("Ruined Green Chapel", TCODColor::green, 22, 16,
		"Bastion of the Green Knights, who often ventured on pilgrimage from the Chapel of the Rose. But all the bastions of \
the Rose have now fallen, and the chapel is deserted.");
	addZone("Darkwater Forest", TCODColor::darkGreen, 22, 15, DARKWATER_FOREST_TXT);

	addZone("Court of the Rose", TCODColor::darkPink, 23, 15,
		"Homeland of the Lady of the Rose, that courageous protector of the forlorn lands.");
	addZone("Percivel's Shrine", TCODColor::green, 22, 14,
		"Shrine erected by Sir Percivel prior to his departure from the Court of the Rose. He never returned.");

	addZone("The Murdermire", TCODColor::darkSea, 21, 15, MURDERMIRE_TXT);
	addZone("The Murdermire", TCODColor::darkSea, 20, 15, MURDERMIRE_TXT);
	addZone("The Murdermire", TCODColor::darkSea, 21, 16, MURDERMIRE_TXT);
	addZone("The Murdermire", TCODColor::darkSea, 20, 16, MURDERMIRE_TXT);
	addZone("Toad Shrine", TCODColor::sea, 19, 16,
		"Shrine erected to the unworshipped gods of the deep.");

	addZone("Pilgrim's Road", TCODColor::lighterSepia, 23, 18, PILGRIMS_ROAD_TXT);
	addZone("Pilgrim's Road", TCODColor::lighterSepia, 23, 19, PILGRIMS_ROAD_TXT);
	addZone("Pilgrim's Road", TCODColor::lighterSepia, 24, 19, PILGRIMS_ROAD_TXT);
	addZone("Pilgrim's Road", TCODColor::lighterSepia, 24, 18, PILGRIMS_ROAD_TXT);
	addZone("Pilgrim's Road", TCODColor::lighterSepia, 24, 17, PILGRIMS_ROAD_TXT); //The Old Crow
	addZone("Silent Ruins", TCODColor::lightestSepia, 25, 18,
		"These disquiet ruins house the Old Crow's last lieutenant.");

	addZone("Flooded Lowlands", TCODColor::lighterGrey, 24, 16, FLOODED_LOWLANDS_TXT);
	addZone("Flooded Lowlands", TCODColor::lighterGrey, 24, 15, FLOODED_LOWLANDS_TXT);
	addZone("Flooded Lowlands", TCODColor::lighterGrey, 25, 16, FLOODED_LOWLANDS_TXT);
	addZone("Flooded Lowlands", TCODColor::lighterGrey, 25, 15, FLOODED_LOWLANDS_TXT); //Fishborn Goddess
	addZone("Ruined Township", TCODColor::lightGrey, 26, 16,
		"Even the name of this old town has been forgotten now.");
	addZone("Deep Lake", TCODColor::darkBlue, 26, 15,
		"In the centre of Deep Lake, it is said, the mausoleum of an old lord lingers.");

	addZone("Crumbling City", TCODColor::grey, 25, 14, CRUMBLING_CITY_TXT);
	addZone("Crumbling City Outskirts", TCODColor::lightGrey, 24, 14, CRUMBLING_CITY_TXT);
	addZone("Crumbling City", TCODColor::grey, 26, 14, CRUMBLING_CITY_TXT);
	addZone("Crumbling City", TCODColor::grey, 26, 13, CRUMBLING_CITY_TXT); //Duke Vorten
	addZone("Sordid Chapel", TCODColor::darkRed, 27, 14,
		"Profane chapel of Pash, a symbol of the city's misdeeds and its inevitable downfall.");

	addZone("Red-Drenched Gardens", TCODColor::red, 28, 14, 
		"This district was among the first to fall to the scourge.");
	addZone("Red-Drenched Cathedral", TCODColor::red, 28, 15,
		"Some say that the High Cleric Orsyl is still huddled in his cathedral, fearing the rise of the Void."); //High Cleric Orsyl
	addZone("Abattoir of Pash", TCODColor::darkRed, 29, 14,
		"The blood-drenched abattoir of the god whose awful countenance devoured the city whole."); //Pash

	addZone("Void-Touched District", TCODColor::lightPurple, 24, 13);
	addZone("Void-Touched District", TCODColor::lightPurple, 25, 13);

	addZone("The Winter Court", TCODColor::cyan, 26, 12, WINTER_COURT_TXT);
	addZone("The Winter Court", TCODColor::cyan, 27, 12, WINTER_COURT_TXT);

	addZone("Tear-Stained Valley", TCODColor::lightYellow, 20, 17, TEAR_STAINED_VALLEY_TXT);
	addZone("Tear-Stained Valley", TCODColor::lightYellow, 21, 17, TEAR_STAINED_VALLEY_TXT);
	addZone("Tear-Stained Valley", TCODColor::lightYellow, 19, 17, TEAR_STAINED_VALLEY_TXT); //Barton, the Prowling Minister
	addZone("Tear-Stained River", TCODColor::sea, 21, 18, TEAR_STAINED_VALLEY_TXT);
	addZone("Tear-Stained River", TCODColor::sea, 20, 18, TEAR_STAINED_VALLEY_TXT);
	addZone("Tear-Stained River", TCODColor::sea, 19, 18, TEAR_STAINED_VALLEY_TXT);

	addZone("Desolate Village", TCODColor::darkSepia, 18, 17,
		"A silent town in the shadow of Hightower.");
	addZone("Hightower Banquet", TCODColor::lightMagenta, 18, 18,
		"According to rumour, the lords and ladies of Hightower still drink wine in these forlorn halls, though what \
they celebrate is unknown.");
	addZone("Lady Tvert's Chamber", TCODColor::magenta, 18, 19,
		"Cursed be Lady Tvert, scion and champion of the oncoming obliteration!"); //Scion of Tvert

}


/*
Create and add a new zone at the given point.
*/
void worldmap::addZone(std::string name, TCODColor color, int atx, int aty, std::string description)
{
	zones.at(atx).at(aty) = new zone(name, color, description);
}
