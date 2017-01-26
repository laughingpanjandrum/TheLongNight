#include "miscItem.h"



miscItem::~miscItem()
{
}


/*
	Keys
*/



miscItem * key_WretchsKey()
{
	miscItem* m = new miscItem("Wretch's Key", KEY_TILE, TCODColor::green,
		"Key treasured by the wretches who fled the Winter Court, but could go no further than Coruscating Beach.");
	m->isKey = true;
	m->setKeyTag("wretched_door");
	return m;
}

miscItem * key_OldCrowsKey()
{
	miscItem* m = new miscItem("Old Crow's Key", KEY_TILE, TCODColor::lightGrey,
		"The Winter Court entrusted the crows with this key, and begged them to safeguard their lands against the scourge.");
	m->isKey = true;
	m->setKeyTag("crow_door");
	return m;
}

miscItem * key_RuinedTownshipKey()
{
	miscItem* m = new miscItem("Ruined Township Key", KEY_TILE, TCODColor::darkLime,
		"Someone entrusted the guardian golem with the safekeeping of this key, and it persisted even after the end of the world.");
	m->isKey = true;
	m->setKeyTag("ruined_township_hall_door");
	return m;
}

miscItem * key_WyrdKey()
{
	miscItem* m = new miscItem("Wyrd Key", KEY_TILE, TCODColor::lightSepia,
		"The Wyrd were once entrusted with the guardianship of the Winter Court - a duty that they upheld even when their \
world was destroyed.");
	m->isKey = true;
	m->setKeyTag("wyrd_door");
	return m;
}

miscItem * key_GreenChapelGardenKey()
{
	miscItem* m = new miscItem("Green Chapel Garden Key", KEY_TILE, TCODColor::lightGreen,
		"A key with its handle carved in the shape of a rose.");
	m->isKey = true;
	m->setKeyTag("green_chapel_door");
	return m;
}



/*
	Spellbooks
*/



miscItem * spellbook_WaterloggedWritings()
{
	miscItem* s = new miscItem("Waterlogged Writings", SPELL_TILE, TCODColor::lightLime,
		"Water-soaked pages filled with sketches of fishmen and descriptions of their scaly magic.");
	s->setKeyTag("waterlogged_writings");
	s->isKey = true;
	return s;
}



/*
	Runestones
*/



miscItem * runestone_CorensRunestone()
{
	miscItem* r = new miscItem("Coren's Runestone", RUNESTONE_TILE, TCODColor::lightBlue,
		"Once Coren the Strong laid waste to the lowlands, until all the townships lay under his command. His tomb is \
here somewhere, beneath the shifting waters.");
	r->isRunestone = true;
	r->setRune(new weaponRune("Coren's Runestone", "Strong", TCODColor::lightBlue, SCALE_STR));
	return r;
}

miscItem * runestone_SiltrasRunestone()
{
	miscItem* r = new miscItem("Siltra's Runestone", RUNESTONE_TILE, TCODColor::lightCrimson,
		"Once Siltra the Red-Eyed rode upon the lowlands, wielding her paired blades, and laid waste to its people. Her tomb \
is here, somewhere beneath the shifting waters.");
	r->isRunestone = true;
	r->setRune(new weaponRune("Siltra's Runestone", "Swift", TCODColor::lightCrimson, SCALE_DEX));
	return r;
}

miscItem * runestone_KhallesRunestone()
{
	miscItem* r = new miscItem("Khalle's Runestone", RUNESTONE_TILE, TCODColor::darkYellow,
		"Khalle's church prayed for the downfall of the void and was wholly swallowed as a consequence. This runestone is the \
only remnant of her memory.");
	r->isRunestone = true;
	r->setRune(new weaponRune("Khalle's Runestone", "Divine", TCODColor::darkYellow, SCALE_DEV));
	return r;
}




/*
	Other
*/


miscItem * misc_VoidSigil()
{
	miscItem* m = new miscItem("Void Sigil Key", SIGIL_TILE, TCODColor::lightPurple,
		"Sign of certain voidwalkers. Those who possess this key are permitted to use the void warping stones.");
	m->isKey = true;
	m->setKeyTag("warpstone");
	return m;
}
