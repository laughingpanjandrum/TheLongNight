#include "menu.h"


menu::~menu()
{
}

void menu::addElement(std::string name, TCODColor color)
{
	elementSharedPtr e(new element(name, 0, color));
	elements.push_back(e);
}

void menu::removeElement(elementSharedPtr e)
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
Sets our index to the given element.
*/
void menu::setCurrentElement(elementSharedPtr e)
{
	int i = 0;
	for (auto it : elements) {
		if (it == e) {
			idx = i;
			return;
		}
		i++;
	}
}

/*
Returns the element at the selected index.
If there are no menu elements, returns nullptr.
*/
elementSharedPtr menu::getSelectedItem()
{
	if (elements.size())
		return elements.at(idx);
	return nullptr;
}
