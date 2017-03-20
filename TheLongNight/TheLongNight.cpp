
#include "libtcod.hpp"
#include "game.h"

#include <iostream>
#include <fstream>

int main() {

	//Gather profiles.
	std::ifstream prFile(SAVE_FILE_LOCATION + PROFILES_FILE);
	std::vector<std::string> allProfiles;
	std::string profileName;
	while (getline(prFile, profileName)) {
		allProfiles.push_back(profileName);
	}
	prFile.close();

	//Iterator to current profile
	auto iter = allProfiles.begin();

	//MAIN MENU SITUATION
	window w;
	char c = ' ';
	imagePtr TITLE = loadImage("title");

	std::string selectedProfile = "";
	
	while (c != 'n' && c != 'l' && c != 'q') {
		w.clear();
		w.drawImage(TITLE, 1, 1);

		//Title and options
		w.write(8, 8, "T H E    L O N G    N I G H T", TCODColor::white);
		w.write(10, 10, "[n] NEW GAME", TCODColor::white);
		w.write(10, 11, "[l] LOAD GAME", TCODColor::white);
		w.write(10, 12, "[q] QUIT GAME", TCODColor::white);

		//Profile menu
		int atx = 50;
		w.write(atx, 10, "SELECT PROFILE", TCODColor::white);
		if (iter != allProfiles.end())
			w.write(atx + 2, 11, (*iter), TCODColor::lightSepia);
		else
			w.write(atx + 2, 11, "NO PROFILES EXIST", TCODColor::darkGrey);
		w.write(atx, 12, "Press [TAB] to switch profiles", TCODColor::lighterGrey);

		//Refresh and get input
		w.refresh();
		auto kp = w.getkey();
		c = kp.c;

		//In-loop input
		if (kp.vk == TCODK_TAB && iter != allProfiles.end()) {
			iter++;
			if (iter == allProfiles.end())
				iter = allProfiles.begin();
		}

		if (iter != allProfiles.end())
			selectedProfile = (*iter);

		//Catch attempting to load if no profile exists
		if (c == 'l' && selectedProfile.size() == 0)
			c = ' ';

	}

	//NEW GAME
	if (c == 'n') {

		//New profile! Select name
		w.clear();
		std::string prName = w.getstr(10, 10, "ENTER PROFILE NAME: ");

		//Add new profile
		std::ofstream prFile(SAVE_FILE_LOCATION + PROFILES_FILE, std::fstream::app);
		prFile << prName + "\n";
		prFile.close();

		//Launch game
		game g(prName, true);
		g.mainGameLoop();
	}

	//LOAD GAME
	else if (c == 'l' && selectedProfile.size() > 0) {
		w.write(10, 14, "LOADING...", TCODColor::white);
		w.refresh();
		game g(selectedProfile);
		g.mainGameLoop();
	}

	//QUIT GAME
	
}