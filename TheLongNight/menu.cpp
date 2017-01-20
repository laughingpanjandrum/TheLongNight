#include "menu.h"


menu::~menu()
{
}

void menu::addElement(std::string name, TCODColor color)
{
	element* e = new element(name, 0, color);
	elements.push_back(e);
}

void menu::removeElement(element * e)
{
	auto iter = std::find(elements.begin(), elements.end(), e);
	if (iter != elements.end())
		elements.erase(iter);
	//Also adjust the position of the selection index, if necessary
	if (idx >= elements.size())
		idx--;
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

/*
Returns the element at the selected index.
If there are no menu elements, returns nullptr.
*/
element * menu::getSelectedItem()
{
	if (elements.size())
		return elements.at(idx);
	return nullptr;
}
