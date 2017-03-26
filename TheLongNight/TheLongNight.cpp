
#include "libtcod.hpp"
#include "game.h"

#include <iostream>
#include <fstream>
#include <ctime>

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
	TCODColor TEXT_COLOR_LIGHT(23, 44, 63);
	TCODColor TEXT_COLOR_DARK(16, 34, 51);
	TCODColor TEXT_COLOR_GREY(79, 79, 79);
	TCODColor BACK_COLOR(0, 0, 0);

	//Random title image
	imagePtr TITLE = loadImage("img/title", true);

	std::string selectedProfile = "";
	
	while (c != 'n' && c != 'l' && c != 'q') {
		w.clear();
		w.drawImage(TITLE, 0, 0); 
		
		//Title and options
		//w.write(8, 8, "T H E    L O N G    N I G H T", TCODColor::white);
		w.write(3, 18, "[n] NEW GAME", TEXT_COLOR_LIGHT, BACK_COLOR);
		w.write(3, 19, "[l] LOAD GAME", TEXT_COLOR_LIGHT, BACK_COLOR);
		w.write(3, 20, "[q] QUIT GAME", TEXT_COLOR_LIGHT, BACK_COLOR);

		w.write(3, 69, "Press [F11] at any time to toggle fullscreen.", TEXT_COLOR_LIGHT, BACK_COLOR);

		//Profile menu
		int atx = 3;
		int aty = 22;
		w.write(atx, aty, "SELECT PROFILE", TEXT_COLOR_DARK, BACK_COLOR);
		if (iter != allProfiles.end())
			w.write(atx + 2, ++aty, (*iter), TEXT_COLOR_GREY, BACK_COLOR);
		else
			w.write(atx + 2, ++aty, "NO PROFILES EXIST", TEXT_COLOR_GREY, BACK_COLOR);
		w.write(atx, ++aty, "Press [TAB] to switch profiles", TEXT_COLOR_DARK, BACK_COLOR);

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
		else if (kp.vk == TCODK_F11)
			TCODConsole::setFullscreen(!TCODConsole::isFullscreen());

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