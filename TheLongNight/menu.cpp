#include "menu.h"


menu::~menu()
{
}

void menu::addElement(std::string name, TCODColor color)
{
	element* e = new element(name, 0, color);
	elements.push_back(e);
}

void menu::scrollDown()
{
	idx++;
	if (idx >= elements.size())
		idx = 0;
}

void menu::scrollUp()
{
	idx--;
	if (idx < 0)
		idx = elements.size() - 1;
}
