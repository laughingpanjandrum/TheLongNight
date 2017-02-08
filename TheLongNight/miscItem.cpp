#include "miscItem.h"



miscItem::~miscItem()
{
}


/*
	Keys
*/



miscItemSharedPtr key_WretchsKey()
{
	miscItemSharedPtr m(new miscItem("Wretch's Key", KEY_TILE, TCODColor::green,
		"Key treasured by the wretches who fled the Winter Court, but could go no further than Coruscating Beach."));
	m->isKey = true;
	m->setKeyTag("wretched_door");
	return m;
}

miscItemSharedPtr key_OldCrowsKey()
{
	miscItemSharedPtr m(new miscItem("Old Crow's Key", KEY_TILE, TCODColor::lightGrey,
		"The Winter Court entrusted the crows with this key, and begged them to safeguard their lands against the scourge."));
	m->isKey = true;
	m->setKeyTag("crow_door");
	return m;
}

miscItemSharedPtr key_RuinedTownshipKey()
{
	miscItemSharedPtr m(new miscItem("Ruined Township Key", KEY_TILE, TCODColor::darkLime,
		"Someone entrusted the guardian golem with the safekeeping of this key, and it persisted even after the end of the world."));
	m->isKey = true;
	m->setKeyTag("ruined_township_hall_door");
	return m;
}

miscItemSharedPtr key_WyrdKey()
{
	miscItemSharedPtr m(new miscItem("Wyrd Key", KEY_TILE, TCODColor::lightSepia,
		"The Wyrd were once entrusted with the guardianship of the Winter Court - a duty that they upheld even when their \
world was destroyed."));
	m->isKey = true;
	m->setKeyTag("wyrd_door");
	return m;
}

miscItemSharedPtr key_GreenChapelGardenKey()
{
	miscItemSharedPtr m(new miscItem("Green Chapel Garden Key", KEY_TILE, TCODColor::lightGreen,
		"A key with its handle carved in the shape of a rose."));
	m->isKey = true;
	m->setKeyTag("green_chapel_door");
	return m;
}

miscItemSharedPtr key_SordidChapelKey()
{
	miscItemSharedPtr m(new miscItem("Sordid Chapel Key", KEY_TILE, TCODColor::purple,
		"This key is so soaked with blood that its material is unrecognizable. Whatever it unlocks is probably best left alone."));
	m->isKey = true;
	m->setKeyTag("sordid_chapel_door");
	return m;
}

miscItemSharedPtr key_SpinalColumnShard()
{
	miscItemSharedPtr m(new miscItem("Spinal Column Shard", KEY_TILE, TCODColor::lighterGrey,
		"A fragment of Orsyl's spinal column. According to the writings of Saint Lucina, one's spinal column contains the \
memories of one's ancestors."));
	m->isKey = true;
	m->setKeyTag("spinal_column_shard");
	return m;
}

miscItemSharedPtr key_OldValleyKey()
{
	miscItemSharedPtr m(new miscItem("Old Valley Key", KEY_TILE, TCODColor::white,
		"This bone-white key unlocks the door of the Tear-Stained Valley. Who knows how it the bloated spider absorbed it into \
its grisly bulk?"));
	m->isKey = true;
	m->setKeyTag("old_valley_door");
	return m;
}

miscItemSharedPtr key_BartonsKey()
{
	miscItemSharedPtr m(new miscItem("Barton's Key", KEY_TILE, TCODColor::flame,
		"Key to the church that guarded entry to Hightower, entrusted to Barton by the Lady of the Tower."));
	m->isKey = true;
	m->setKeyTag("bartons_key");
	return m;
}

miscItemSharedPtr key_HightowerKey()
{
	miscItemSharedPtr m(new miscItem("Hightower Key", KEY_TILE, TCODColor::pink,
		"Heavy brass key to the main gates of Hightower. Many assassins would have paid dearly to acquire this key at the height of\
 Lady Tvert's reign, but now the lowly slave master holds it, and no one approaches the gilded gates."));
	m->isKey = true;
	m->setKeyTag("hightower_key");
	return m;
}

miscItemSharedPtr key_WatchfulEyestalk()
{
	miscItemSharedPtr m(new miscItem("Watchful Eyestalk", KEY_TILE, TCODColor::darkGreen,
		"According to the Ritual of Khull-Kallen, the first article of summoning is the Watchful Eyestalk. 'The Eye of the Lord \
behold his subjects, and lets their most profound desires be known to him. Offer it at the altar in honour of his knowledge.'"));
	m->isKey = true;
	m->setKeyTag("watchful_eyestalk");
	return m;
}



/*
	Hearts
*/


miscItemSharedPtr heart_WretchedHeart()
{
	miscItemSharedPtr h(new miscItem("Wretched Heart", HEART_TILE, TCODColor::darkRed,
		"This small, putrid heart lay at the centre of the wretched mass, apparently conferring life to the putrid thing."));
	h->setKeyTag("wretched_heart");
	h->isKey = true;
	return h;
}

miscItemSharedPtr heart_OldCrowsHeart()
{
	miscItemSharedPtr h(new miscItem("Old Crow's Heart", HEART_TILE, TCODColor::lightCrimson,
		"Within the Old Crow's breast sat this small, black heart. It beats softly in your palm."));
	h->setKeyTag("old_crows_heart");
	h->isKey = true;
	return h;
}

miscItemSharedPtr heart_FishBornGoddessesHeart()
{
	miscItemSharedPtr h(new miscItem("Fishborn Goddess's Heart", HEART_TILE, TCODColor::darkSea,
		"Within the breast of the Fish-born Goddess sat this scaly green heart, dripping with seawater."));
	h->setKeyTag("fishborn_heart");
	h->isKey = true;
	return h;
}

miscItemSharedPtr heart_OrsylsShriveledHeart()
{
	miscItemSharedPtr h(new miscItem("Orsyl's Shriveled Heart", HEART_TILE, TCODColor::purple,
		"Ruined heart of High Cleric Orsyl, who stood against the Void for as long as he could. It pulses with the awful light \
of the Void."));
	h->setKeyTag("orsyls_heart");
	h->isKey = true;
	return h;
}

miscItemSharedPtr heart_VortensShriveledHeart()
{
	miscItemSharedPtr h(new miscItem("Vorten's Shriveled Heart", HEART_TILE, TCODColor::darkGreen,
		"Duke Vorten welcomed the Void, and was cast out of the Winter Court as a consequence. He remained at the gates while the \
Void gnawed at him, begging to be allowed back in, but the gates never opened."));
	h->setKeyTag("vortens_heart");
	h->isKey = true;
	return h;
}

miscItemSharedPtr heart_VenomousSpiderHeart()
{
	miscItemSharedPtr h(new miscItem("Venomous Spider Heart", HEART_TILE, TCODColor::lime,
		"This venom-drenched heart lay deep within Ghorth, the monstrous lord of spiders."));
	h->setKeyTag("venomous_spider_heart");
	h->isKey = true;
	return h;
}



/*
	Spellbooks
*/



miscItemSharedPtr spellbook_WaterloggedWritings()
{
	miscItemSharedPtr s(new miscItem("Waterlogged Writings", SPELL_TILE, TCODColor::lightLime,
		"Water-soaked pages filled with sketches of fishmen and descriptions of their scaly magic."));
	s->setKeyTag("waterlogged_writings");
	s->isKey = true;
	return s;
}

miscItemSharedPtr spellbook_MinasProfanedWritings()
{
	miscItemSharedPtr s(new miscItem("Minas's Profaned Writings", SPELL_TILE, TCODColor::purple,
		"This filthy scroll is marked with profane runes, scrawled in blood."));
	s->setKeyTag("minas_profaned_writings");
	s->isKey = true;
	return s;
}

miscItemSharedPtr spellbook_FrostbittenWritings()
{
	miscItemSharedPtr s(new miscItem("Frostbitten Writings", SPELL_TILE, TCODColor::cyan,
		"These pages are brittle and cold, marked with runes and sketches of frostbitten shades. Perhaps a trained wizard could \
make some sense of them."));
	s->isKey = true;
	s->setKeyTag("frostbitten_writings");
	return s;
}

miscItemSharedPtr spellbook_OrsylsTomeOfPrayer()
{
	miscItemSharedPtr s(new miscItem("Orsyl's Tome of Prayer", SPELL_TILE, TCODColor::lightPurple,
		"In his final days, Orsyl's holy prayers were profaned by his contact with the Void."));
	s->setKeyTag("orsyls_tome_of_prayer");
	s->isKey = true;
	return s;
}

miscItemSharedPtr spellbook_DivineMoonsparkTome()
{
	miscItemSharedPtr s(new miscItem("Divine Moonspark Tome", SPELL_TILE, TCODColor::lighterYellow,
		"This tome, encrusted with mud, contains the divine knowledge of the lost saint Yutria."));
	s->setKeyTag("divine_moonspark_tome");
	s->isKey = true;
	return s;
}

miscItemSharedPtr spellbook_DivineTomeOfTheEmissary()
{
	miscItemSharedPtr s(new miscItem("Divine Tome of the Emissary", SPELL_TILE, TCODColor::lightFlame,
		"The writings of the Emissaries are filled with edicts of cleansing and proclamations of divine justice, \
as befit their role in the Valley."));
	s->isKey = true;
	s->setKeyTag("divine_tome_of_the_emissary");
	return s;
}



/*
	Runestones
*/



miscItemSharedPtr runestone_CorensRunestone()
{
	miscItemSharedPtr r(new miscItem("Coren's Runestone", RUNESTONE_TILE, TCODColor::lightBlue,
		"Once Coren the Strong laid waste to the lowlands, until all the townships lay under his command. His tomb is \
here somewhere, beneath the shifting waters."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Coren's Runestone", "Strong", TCODColor::lightBlue, SCALE_STR));
	return r;
}

miscItemSharedPtr runestone_SiltrasRunestone()
{
	miscItemSharedPtr r(new miscItem("Siltra's Runestone", RUNESTONE_TILE, TCODColor::lightCrimson,
		"Once Siltra the Red-Eyed rode upon the lowlands, wielding her paired blades, and laid waste to its people. Her tomb \
is here, somewhere beneath the shifting waters."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Siltra's Runestone", "Swift", TCODColor::lightCrimson, SCALE_DEX));
	return r;
}

miscItemSharedPtr runestone_KhallesRunestone()
{
	miscItemSharedPtr r(new miscItem("Khalle's Runestone", RUNESTONE_TILE, TCODColor::darkYellow,
		"Khalle's church prayed for the downfall of the void and was wholly swallowed as a consequence. This runestone is the \
only remnant of her memory."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Khalle's Runestone", "Divine", TCODColor::darkYellow, SCALE_DEV));
	return r;
}

miscItemSharedPtr runestone_IetrasRunestone()
{
	miscItemSharedPtr r(new miscItem("Ietra's Runestone", RUNESTONE_TILE, TCODColor::magenta,
		"Ietra, last wizard of the Winter Court, once rode among the stars and sought knowledge of the ancient seers. Her tomb \
was swallowed by the Void."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Ietra's Runestone", "Arcane", TCODColor::magenta, SCALE_ARC));
	return r;
}

miscItemSharedPtr runestone_CharredRunestone()
{
	miscItemSharedPtr r(new miscItem("Charred Runestone", RUNESTONE_TILE, TCODColor::darkFlame,
		"Horribly burnt runestone, perhaps seared in Lady Tvert's cleansing fire."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Charred Runestone", "Flaming", TCODColor::darkFlame, SCALE_FIRE));
	return r;
}


/*
	Bones
*/

miscItemSharedPtr bones_KhallesBones()
{
	miscItemSharedPtr b(new miscItem("Khalle's Bones", KEY_TILE, TCODColor::lightestYellow,
		"All that remains of Khalle, who prayed to the old gods for the downfall of the void. Her prayers were met with silence, \
and her congregation was swallowed by the rising mud."));
	b->isKey = true;
	b->setKeyTag("khalles_bones");
	return b;
}

miscItemSharedPtr bones_SiltrasBones()
{
	miscItemSharedPtr b(new miscItem("Siltra's Bones", KEY_TILE, TCODColor::lightestYellow,
		"All that remains of Siltra, who once menaced the lowlands with her red-eyed army. In later days it was said that she \
was greatly changed, and that the old gods gave her a fitting reward for her deeds."));
	b->isKey = true;
	b->setKeyTag("siltras_bones");
	return b;
}




/*
	Other
*/


miscItemSharedPtr misc_VoidSigil()
{
	miscItemSharedPtr m(new miscItem("Void Sigil Key", SIGIL_TILE, TCODColor::lightPurple,
		"Sign of certain voidwalkers. Those who possess this key are permitted to use the void warping stones."));
	m->isKey = true;
	m->setKeyTag("warpstone");
	return m;
}

miscItemSharedPtr misc_PieceOfJade()
{
	miscItemSharedPtr m(new miscItem("Piece of Jade", FRAGMENT_GLYPH, TCODColor::lightGreen,
		"A chunk of jade that sparkles with surprising radiance. It reminds you of home."));
	m->isKey = true;
	m->setKeyTag("piece_of_jade");
	return m;
}
