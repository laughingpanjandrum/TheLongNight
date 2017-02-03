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

miscItem * key_SordidChapelKey()
{
	miscItem* m = new miscItem("Sordid Chapel Key", KEY_TILE, TCODColor::purple,
		"This key is so soaked with blood that its material is unrecognizable. Whatever it unlocks is probably best left alone.");
	m->isKey = true;
	m->setKeyTag("sordid_chapel_door");
	return m;
}

miscItem * key_SpinalColumnShard()
{
	miscItem* m = new miscItem("Spinal Column Shard", KEY_TILE, TCODColor::lighterGrey,
		"A fragment of Orsyl's spinal column. According to the writings of Saint Lucina, one's spinal column contains the \
memories of one's ancestors.");
	m->isKey = true;
	m->setKeyTag("spinal_column_shard");
	return m;
}

miscItem * key_OldValleyKey()
{
	miscItem* m = new miscItem("Old Valley Key", KEY_TILE, TCODColor::white,
		"This bone-white key unlocks the door of the Tear-Stained Valley. Who knows how it the bloated spider absorbed it into \
its grisly bulk?");
	m->isKey = true;
	m->setKeyTag("old_valley_door");
	return m;
}

miscItem * key_WatchfulEyestalk()
{
	miscItem* m = new miscItem("Watchful Eyestalk", KEY_TILE, TCODColor::darkGreen,
		"According to the Ritual of Khull-Kallen, the first article of summoning is the Watchful Eyestalk. 'The Eye of the Lord \
behold his subjects, and lets their most profound desires be known to him. Offer it at the altar in honour of his knowledge.'");
	m->isKey = true;
	m->setKeyTag("watchful_eyestalk");
	return m;
}



/*
	Hearts
*/


miscItem * heart_WretchedHeart()
{
	miscItem* h = new miscItem("Wretched Heart", HEART_TILE, TCODColor::darkRed,
		"This small, putrid heart lay at the centre of the wretched mass, apparently conferring life to the putrid thing.");
	h->setKeyTag("wretched_heart");
	h->isKey = true;
	return h;
}

miscItem * heart_OldCrowsHeart()
{
	miscItem* h = new miscItem("Old Crow's Heart", HEART_TILE, TCODColor::lightCrimson,
		"Within the Old Crow's breast sat this small, black heart. It beats softly in your palm.");
	h->setKeyTag("old_crows_heart");
	h->isKey = true;
	return h;
}

miscItem * heart_FishBornGoddessesHeart()
{
	miscItem* h = new miscItem("Fishborn Goddess's Heart", HEART_TILE, TCODColor::darkSea,
		"Within the breast of the Fish-born Goddess sat this scaly green heart, dripping with seawater.");
	h->setKeyTag("fishborn_heart");
	h->isKey = true;
	return h;
}

miscItem * heart_OrsylsShriveledHeart()
{
	miscItem* h = new miscItem("Orsyl's Shriveled Heart", HEART_TILE, TCODColor::purple,
		"Ruined heart of High Cleric Orsyl, who stood against the Void for as long as he could. It pulses with the awful light \
of the Void.");
	h->setKeyTag("orsyls_heart");
	h->isKey = true;
	return h;
}

miscItem * heart_VortensShriveledHeart()
{
	miscItem* h = new miscItem("Vorten's Shriveled Heart", HEART_TILE, TCODColor::darkGreen,
		"Duke Vorten welcomed the Void, and was cast out of the Winter Court as a consequence. He remained at the gates while the \
Void gnawed at him, begging to be allowed back in, but the gates never opened.");
	h->setKeyTag("vortens_heart");
	h->isKey = true;
	return h;
}

miscItem * heart_VenomousSpiderHeart()
{
	miscItem* h = new miscItem("Venomous Spider Heart", HEART_TILE, TCODColor::lime,
		"This venom-drenched heart lay deep within Ghorth, the monstrous lord of spiders.");
	h->setKeyTag("venomous_spider_heart");
	h->isKey = true;
	return h;
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

miscItem * spellbook_MinasProfanedWritings()
{
	miscItem* s = new miscItem("Minas's Profaned Writings", SPELL_TILE, TCODColor::purple,
		"This filthy scroll is marked with profane runes, scrawled in blood.");
	s->setKeyTag("minas_profaned_writings");
	s->isKey = true;
	return s;
}

miscItem * spellbook_FrostbittenWritings()
{
	miscItem* s = new miscItem("Frostbitten Writings", SPELL_TILE, TCODColor::cyan,
		"These pages are brittle and cold, marked with runes and sketches of frostbitten shades. Perhaps a trained wizard could \
make some sense of them.");
	s->isKey = true;
	s->setKeyTag("frostbitten_writings");
	return s;
}

miscItem * spellbook_OrsylsTomeOfPrayer()
{
	miscItem* s = new miscItem("Orsyl's Tome of Prayer", SPELL_TILE, TCODColor::lightPurple,
		"In his final days, Orsyl's holy prayers were profaned by his contact with the Void.");
	s->setKeyTag("orsyls_tome_of_prayer");
	s->isKey = true;
	return s;
}

miscItem * spellbook_DivineMoonsparkTome()
{
	miscItem* s = new miscItem("Divine Moonspark Tome", SPELL_TILE, TCODColor::lighterYellow,
		"This tome, encrusted with mud, contains the divine knowledge of the lost saint Yutria.");
	s->setKeyTag("divine_moonspark_tome");
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

miscItem * runestone_IetrasRunestone()
{
	miscItem* r = new miscItem("Ietra's Runestone", RUNESTONE_TILE, TCODColor::magenta,
		"Ietra, last wizard of the Winter Court, once rode among the stars and sought knowledge of the ancient seers. Her tomb \
was swallowed by the Void.");
	r->isRunestone = true;
	r->setRune(new weaponRune("Ietra's Runestone", "Arcane", TCODColor::magenta, SCALE_ARC));
	return r;
}

miscItem * runestone_CharredRunestone()
{
	miscItem* r = new miscItem("Charred Runestone", RUNESTONE_TILE, TCODColor::darkFlame,
		"Horribly burnt runestone, perhaps seared in Lady Tvert's cleansing fire.");
	r->isRunestone = true;
	r->setRune(new weaponRune("Charred Runestone", "Flaming", TCODColor::darkFlame, SCALE_FIRE));
	return r;
}


/*
	Bones
*/

miscItem * bones_KhallesBones()
{
	miscItem* b = new miscItem("Khalle's Bones", KEY_TILE, TCODColor::lightestYellow,
		"All that remains of Khalle, who prayed to the old gods for the downfall of the void. Her prayers were met with silence, \
and her congregation was swallowed by the rising mud.");
	b->isKey = true;
	b->setKeyTag("khalles_bones");
	return b;
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

miscItem * misc_PieceOfJade()
{
	miscItem* m = new miscItem("Piece of Jade", FRAGMENT_GLYPH, TCODColor::lightGreen,
		"A chunk of jade that sparkles with surprising radiance. It reminds you of home.");
	m->isKey = true;
	m->setKeyTag("piece_of_jade");
	return m;
}
