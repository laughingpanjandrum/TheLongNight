#include "mapLoader.h"



mapLoader::mapLoader()
{
}


/*
	LOADING
*/

map * mapLoader::loadMapFromFile(std::string filename)
{
	
	/*
	Prep
	*/
	
	//Open file
	std::ifstream mapfile(filename);
	//Stuff we'll need to know
	std::string mapname;
	int xsize = 0, ysize = 0;
	stringVector tiles;
	//These two vectors contain encodings for different tile types
	stringVector tileHandles; //These are the maptile handles we'll need
	charVector tileChars; //These appear in the file
	//The starting point, just in case we happen to have one
	coord startPt;
	
	/*
	Now we can actually load info from the file
	*/
	
	//First line is the map name
	getline(mapfile, mapname);

	//Now read in creature positions until we reach encodings
	std::string line = "";
	getline(mapfile, line);
	while (line != "-codes") {
		//Read till we reach the colon, which indicates end of name
		int i = 0;
		char c = line.at(i);
		std::string chunk = "";
		while (c != ':') {
			chunk += c;
			i++;
			c = line.at(i);
		}
		//Now read in the coords. x-coord is BEFORE the comma
		i++;
		c = line.at(i);
		std::string xcoord = "";
		while (c != ',') {
			xcoord += c;
			i++;
			c = line.at(i);
		}
		//y-coord is AFTER the comma
		std::string ycoord = "";
		for (int n = i + 1; n < line.size(); n++) {
			ycoord += line.at(n);
		}
		//Now see what kind of coordinate we made!
		int x = std::stoi(xcoord);
		int y = std::stoi(ycoord);
		if (chunk == "start") {
			//Starting position!
			startPt.first = x;
			startPt.second = y;
		}
		//Next line!
		getline(mapfile, line);
	}

	//Next series of lines define tile encodings
	getline(mapfile, line);
	while (line != "-endcodes") {
		//First character is the character that appears in the file
		tileChars.push_back(line.at(0));
		//The rest of the line, after the space, is the tile handle
		tileHandles.push_back(line.substr(2, line.size()));
		//Next line
		getline(mapfile, line);
	}

	//The rest of the file is the map itself
	while (getline(mapfile, line)) {
		//Longest line defines x-size
		if (line.size() > xsize)
			xsize = line.size();
		//ysize increases by 1 for each new line
		ysize++;
		//And now just remember this line
		tiles.push_back(line);
	}

	//Done with the file
	mapfile.close();

	//This converts from tile HANDLES to actual tile POINTERS
	tileVector* tileList = getTileList(tileHandles);

	/*
	Now we actually generate the map
	*/

	map* m = new map(xsize, ysize);
	
	//Fill in map with all the tiles we got
	for (int y = 0; y < ysize; y++) {
		for (int x = 0; x < xsize; x++) {
			//Character at this point
			char c = tiles.at(y).at(x);
			//Convert to tile
			maptile* mt = getTileByChar(c, &tileChars, tileList);
			//And add it to the map!
			m->setTile(mt, x, y);
		}
	}

	//Add start point to map
	m->setStartPoint(startPt);

	//Return final map
	return m;

}


/*
Given a list of tile handles, creates a list of actual tiles and returns it.
*/
tileVector * mapLoader::getTileList(stringVector tileHandles)
{
	tileVector* tileCodes = new tileVector();
	for (auto handle : tileHandles) {
		//Sort through all available tiles
		for (auto tile : ALL_MAPTILES) {
			if (tile->getHandle() == handle) {
				tileCodes->push_back(tile);
				break;
			}
		}
	}
	return tileCodes;
}

/*
Returns maptile corresponding to the given character.
*/
maptile * mapLoader::getTileByChar(char c, charVector* tileChars, tileVector* tileList)
{
	//Find character index
	for (int i = 0; i < tileChars->size(); i++) {
		if (tileChars->at(i) == c)
			return tileList->at(i);
	}
	return nullptr;
}
