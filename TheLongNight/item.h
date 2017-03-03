
#ifndef ITEM_H
#define ITEM_H

#include <memory>
#include "element.h"
#include "categories.h"
#include "images.h"


class item: public element
{
public:

	//Constructors
	item(): element() {}
	item(std::string name, int tileCode, TCODColor color, std::string description = "") : 
		element(name, tileCode, color, description) {}
	item(std::string name, int tileCode, TCODColor color, itemTypes category, std::string description = "");
	~item();

	//Setters
	void setDefence(int def) { this->defence = def; }
	void setBleedResist(int r) { bleedResist = r; }
	void setPoisonResist(int r) { poisonResist = r; }
	void setPlagueResist(int r) { plagueResist = r; }
	void setDamageResist(damageType dtype, int def) { damageResist.at(dtype) = def; }

	//Getters
	itemTypes getCategory() { return category; }
	virtual int getAmountLeft() { return 1; }
	virtual const int isEquippable() { return true; }

	//Universal attributes: getting
	int getDefence() { return defence; }
	int getDamageResist(damageType dtype) { return damageResist.at(dtype); }
	int getBleedResist() { return bleedResist; }
	int getPoisonResist() { return poisonResist; }
	int getPlagueResist() { return plagueResist; }

	//Stacking
	virtual const bool isStackable() { return false; }
	virtual const void add(int amount) {}
	virtual const void restoreToMax() {}

	//Shopkeeping
	int getPrice() { return price; }
	void setPrice(int p) { price = p; }
	void setKeyEaten(std::string tag) { keyEaten = tag; eatsKeyWhenBought = true; }
	std::string getKeyEaten() { return keyEaten; }
	bool eatsKeyWhenBought = false;

	//Image data
	bool hasImage() { return imgPtr != nullptr; }
	virtual imageSharedPtr getImage() { return imgPtr; }
	virtual void setImage(imagePtr img) { imgPtr = imageSharedPtr(img); }

	//Flags
	bool inSirPercivelsSet = false; //The parts of this set form a key when all are acquired

protected:

	//Sorting
	itemTypes category;

	//Shopkeeping
	int price = 0;
	std::string keyEaten;

	//Any item can have these attributes
	int defence = 0;
	int bleedResist = 0;
	int poisonResist = 0;
	int plagueResist = 0;
	std::vector<int> damageResist;

	//Image data
	imageSharedPtr imgPtr;

};


typedef std::shared_ptr<item> itemSharedPtr;
typedef std::vector<itemSharedPtr> itemVector;


/*
For shopkeepers.
Keeps track of a set of sellable items that are unlocked with a code.
*/
struct unlockableStock {
	unlockableStock(itemSharedPtr it, std::string unlockCode) :
		it(it), unlockCode(unlockCode) {}
	itemSharedPtr it;
	std::string unlockCode;
};

typedef std::vector<unlockableStock*> stockUnlockVector;


/*
Weapon infusions.
Keeps track of special effects applied to weapons.
*/
struct weaponRune {
	weaponRune() {}
	weaponRune(std::string name, std::string nameMod, TCODColor color, statScaling addScalingType) :
		name(name), nameMod(nameMod), color(color), addScalingType(addScalingType) {}
	//Flavour
	std::string nameMod;
	std::string name;
	TCODColor color;
	//Effects
	statScaling addScalingType;
};

typedef std::shared_ptr<weaponRune> weaponRuneSharedPtr;
typedef std::vector<weaponRuneSharedPtr> weaponRuneVector;

//Names of item categories.
const std::string getItemCategoryName(itemTypes cat);

#endif

