
/*
For any menu where you have to sort through a list of options.
Each option is an element.
*/

#ifndef MENU_H
#define MENU_H

#include "libtcod.h"
#include "element.h"

class menu
{
public:
	
	//Constructors
	menu(std::string title) :
		title(title) {}
	~menu();

	//Setting up
	void addElement(std::string name, TCODColor color);
	void addElement(elementSharedPtr e) { elements.push_back(e); }
	void removeElement(elementSharedPtr e);

	//Menu navigation
	void scrollDown();
	void scrollUp();
	void setCurrentElement(elementSharedPtr e);

	//Getting
	std::string getTitle() { return title; }
	elementVector getAllElements() { return elements; }
	elementSharedPtr getSelectedItem();
	int getIdx() { return idx; }

private:

	//Menu details
	std::string title;
	elementVector elements;

	//State details
	int idx = 0;

};

#endif