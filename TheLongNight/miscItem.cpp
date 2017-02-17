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
	m->setKeyTag("wretched_door");
	return m;
}

miscItemSharedPtr key_OldCrowsKey()
{
	miscItemSharedPtr m(new miscItem("Old Crow's Key", KEY_TILE, TCODColor::lightGrey,
		"The Winter Court entrusted the crows with this key, and begged them to safeguard their lands against the scourge."));
	m->setKeyTag("crow_door");
	return m;
}

miscItemSharedPtr key_RuinedTownshipKey()
{
	miscItemSharedPtr m(new miscItem("Ruined Township Key", KEY_TILE, TCODColor::darkLime,
		"Someone entrusted the guardian golem with the safekeeping of this key, and it persisted even after the end of the world."));
	m->setKeyTag("ruined_township_hall_door");
	return m;
}

miscItemSharedPtr key_WyrdKey()
{
	miscItemSharedPtr m(new miscItem("Wyrd Key", KEY_TILE, TCODColor::lightSepia,
		"The Wyrd were once entrusted with the guardianship of the Winter Court - a duty that they upheld even when their \
world was destroyed."));
	m->setKeyTag("wyrd_door");
	return m;
}

miscItemSharedPtr key_GreenChapelGardenKey()
{
	miscItemSharedPtr m(new miscItem("Green Chapel Garden Key", KEY_TILE, TCODColor::lightGreen,
		"A key with its handle carved in the shape of a rose."));
	m->setKeyTag("green_chapel_door");
	return m;
}

miscItemSharedPtr key_SordidChapelKey()
{
	miscItemSharedPtr m(new miscItem("Sordid Chapel Key", KEY_TILE, TCODColor::purple,
		"This key is so soaked with blood that its material is unrecognizable. Whatever it unlocks is probably best left alone."));
	m->setKeyTag("sordid_chapel_door");
	return m;
}

miscItemSharedPtr key_SpinalColumnShard()
{
	miscItemSharedPtr m(new miscItem("Spinal Column Shard", KEY_TILE, TCODColor::lighterGrey,
		"A fragment of Orsyl's spinal column. According to the writings of Saint Lucina, one's spinal column contains the \
memories of one's ancestors."));
	m->setKeyTag("spinal_column_shard");
	return m;
}

miscItemSharedPtr key_OldValleyKey()
{
	miscItemSharedPtr m(new miscItem("Old Valley Key", KEY_TILE, TCODColor::white,
		"This bone-white key unlocks the door of the Tear-Stained Valley. Who knows how it the bloated spider absorbed it into \
its grisly bulk?"));
	m->setKeyTag("old_valley_door");
	return m;
}

miscItemSharedPtr key_BartonsKey()
{
	miscItemSharedPtr m(new miscItem("Barton's Key", KEY_TILE, TCODColor::flame,
		"Key to the church that guarded entry to Hightower, entrusted to Barton by the Lady of the Tower."));
	m->setKeyTag("bartons_key");
	return m;
}

miscItemSharedPtr key_HightowerKey()
{
	miscItemSharedPtr m(new miscItem("Hightower Key", KEY_TILE, TCODColor::pink,
		"Heavy brass key to the main gates of Hightower. Many assassins would have paid dearly to acquire this key at the height of\
 Lady Tvert's reign, but now the lowly slave master holds it, and no one approaches the gilded gates."));
	m->setKeyTag("hightower_key");
	return m;
}

miscItemSharedPtr key_LadyTvertsKey()
{
	miscItemSharedPtr m(new miscItem("Lady Tvert's Key", KEY_TILE, TCODColor::lightBlue,
		"The key to the Hightower basement. Lady Tvert kept the only copy on a chain around her neck. Hightower \
festivals gloried in their hedonistic excess, and the sole thing the nobles were forbidden was to enter the mysterious basement."));
	m->setKeyTag("lady_tverts_key");
	return m;
}

miscItemSharedPtr key_DeadSparrowKey()
{
	miscItemSharedPtr m(new miscItem("Dead Sparrow's Key", KEY_TILE, TCODColor::magenta,
		"Key to the tower of the Dead Sparrow, that watchful aeon who keeps the surrounding lands safe from harm."));
	m->setKeyTag("dead_sparrows_key");
	return m;
}

miscItemSharedPtr key_MoshkasKey()
{
	miscItemSharedPtr m(new miscItem("Moshka's Key", KEY_TILE, TCODColor::fuchsia,
		"Moshka, mentor of the Sparrow, entrusted his old friend with this precious key, before departing to learn the \
mysteries of the stars."));
	m->setKeyTag("moshkas_key");
	return m;
}

miscItemSharedPtr key_MoonpaleKey()
{
	miscItemSharedPtr m(new miscItem("Moon-Pale Key", KEY_TILE, TCODColor::lighterBlue,
		"Key created by the Eyeless Queen. Seeing that the Moon-Pale King's lands were slowly becoming unmoored \
from reality, she forged this key to the King to return to the material world whenever he desired. Alas, the king \
did not care to return, and instead remained in his palace until only a withered husk remained."));
	m->setKeyTag("moonpale_key");
	return m;
}

miscItemSharedPtr key_WatchfulEyestalk()
{
	miscItemSharedPtr m(new miscItem("Watchful Eyestalk", KEY_TILE, TCODColor::darkGreen,
		"According to the Ritual of Khull-Kallen, the first article of summoning is the Watchful Eyestalk. 'The Eye of the Lord \
behold his subjects, and lets their most profound desires be known to him. Offer it at the altar in honour of his knowledge.'"));
	m->setKeyTag("watchful_eyestalk");
	return m;
}

miscItemSharedPtr key_MawtoothFragment()
{
	miscItemSharedPtr m(new miscItem("Mawtooth Fragment", KEY_TILE, TCODColor::lightestRed,
		"According to the Ritual of Khull-Khallen, the second Article of Summoning is the Mawtooth Fragment. As it is written:\
 'The Mouthpiece of the Lord issues His Commands, and lets His Will be known to the world.Offer it at the \
Altar in honour of His Wisdom.'"));
	m->setKeyTag("mawtooth_fragment");
	return m;
}

miscItemSharedPtr key_IetrasAmberPendant()
{
	miscItemSharedPtr m(new miscItem("Ietra's Amber Pendant", KEY_TILE, TCODColor::amber,
		"Amber stone on a cord, which Ietra clutched until her dying moment. Its significance is unknown."));
	m->setKeyTag("ietras_amber_pendant");
	return m;
}



/*
	Hearts
*/


miscItemSharedPtr heart_WretchedHeart()
{
	miscItemSharedPtr h(new miscItem("Wretched Heart", HEART_TILE, TCODColor::darkRed,
		"This small, rotten heart lay at the centre of the wretched mass, apparently conferring life to the putrid thing."));
	h->setKeyTag("wretched_heart");
	return h;
}

miscItemSharedPtr heart_OldCrowsHeart()
{
	miscItemSharedPtr h(new miscItem("Old Crow's Heart", HEART_TILE, TCODColor::lightCrimson,
		"Within the Old Crow's breast sat this small, black heart. It beats softly in your palm."));
	h->setKeyTag("old_crows_heart");
	return h;
}

miscItemSharedPtr heart_FishBornGoddessesHeart()
{
	miscItemSharedPtr h(new miscItem("Fishborn Goddess's Heart", HEART_TILE, TCODColor::darkSea,
		"Within the breast of the Fish-born Goddess sat this scaly green heart, dripping with seawater."));
	h->setKeyTag("fishborn_heart");
	return h;
}

miscItemSharedPtr heart_OrsylsShriveledHeart()
{
	miscItemSharedPtr h(new miscItem("Orsyl's Shriveled Heart", HEART_TILE, TCODColor::purple,
		"Ruined heart of High Cleric Orsyl, who stood against the Void for as long as he could. It pulses with the awful light \
of the Void."));
	h->setKeyTag("orsyls_heart");
	return h;
}

miscItemSharedPtr heart_VortensShriveledHeart()
{
	miscItemSharedPtr h(new miscItem("Vorten's Shriveled Heart", HEART_TILE, TCODColor::darkGreen,
		"Duke Vorten welcomed the Void, and was cast out of the Winter Court as a consequence. He remained at the gates while the \
Void gnawed at him, begging to be allowed back in, but the gates never opened."));
	h->setKeyTag("vortens_heart");
	return h;
}

miscItemSharedPtr heart_VenomousSpiderHeart()
{
	miscItemSharedPtr h(new miscItem("Venomous Spider Heart", HEART_TILE, TCODColor::lime,
		"This venom-drenched heart lay deep within Ghorth, the monstrous lord of spiders."));
	h->setKeyTag("venomous_spider_heart");
	return h;
}

miscItemSharedPtr heart_LadyTvertsHeart()
{
	miscItemSharedPtr h(new miscItem("Lady Tvert's Heart", HEART_TILE, TCODColor::lightBlue,
		"Heart plucked from the breast of Lady Tvert, whose pact with the White Fog Herald brought ruin to her house."));
	h->setKeyTag("lady_tverts_heart");
	return h;
}

miscItemSharedPtr heart_DeadSparrowsHeart()
{
	miscItemSharedPtr h(new miscItem("Dead Sparrow's Heart", HEART_TILE, TCODColor::fuchsia,
		"The ethereal heart buried in the breast of the Dead Sparrow, whose search for arcane power \
brought him too close to the mysteries of the gods."));
	h->setKeyTag("dead_sparrows_heart");
	return h;
}

miscItemSharedPtr heart_HeartOfCoren()
{
	miscItemSharedPtr h(new miscItem("Heart of Coren", HEART_TILE, TCODColor::darkCrimson,
		"The beating heart of Coren the Conqueror, who once laid waste to the lowlands. It perpetually oozes blood, \
but never seems to run dry."));
	h->setKeyTag("corens_heart");
	return h;
}

miscItemSharedPtr heart_KhallesWretchedHeart()
{
	miscItemSharedPtr h(new miscItem("Khalle's Wretched Heart", HEART_TILE, TCODColor::darkYellow,
		"Heart filled with shards of bone. This is all that remains of Khalle, whom the gods never answered."));
	h->setKeyTag("khalles_heart");
	return h;
}

miscItemSharedPtr heart_IetrasBurningHeart()
{
	miscItemSharedPtr h(new miscItem("Ietra's Burning Heart", HEART_TILE, TCODColor::flame,
		"Still-burning heart of Ietra the Seer, who found the truth of the immortals at the heart of the void, \
and resolved that it would be known to none."));
	h->setKeyTag("ietras_heart");
	return h;
}

miscItemSharedPtr heart_PaleHeart()
{
	miscItemSharedPtr h(new miscItem("Pale Heart", HEART_TILE, TCODColor::lightestBlue,
		"Half-unreal heart of the Pale Shade that dwelt in the wastes of a lost kingdom. Perhaps its tears were what \
kept the wastes from drifting out of existence entirely."));
	h->setKeyTag("pale_heart");
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
	return s;
}

miscItemSharedPtr spellbook_MinasProfanedWritings()
{
	miscItemSharedPtr s(new miscItem("Minas's Profaned Writings", SPELL_TILE, TCODColor::purple,
		"This filthy scroll is marked with profane runes, scrawled in blood."));
	s->setKeyTag("minas_profaned_writings");
	return s;
}

miscItemSharedPtr spellbook_FrostbittenWritings()
{
	miscItemSharedPtr s(new miscItem("Frostbitten Writings", SPELL_TILE, TCODColor::cyan,
		"These pages are brittle and cold, marked with runes and sketches of frostbitten shades. Perhaps a trained wizard could \
make some sense of them."));
	s->setKeyTag("frostbitten_writings");
	return s;
}

miscItemSharedPtr spellbook_SingedWritings()
{
	miscItemSharedPtr s(new miscItem("Singed Writings", SPELL_TILE, TCODColor::magenta,
		"Pages singed by lightning bolts. No surprise that Gottric would be intrigued by the Sparrows; their \
knowledge of arcane mysteries was well known."));
	s->setKeyTag("singed_writings");
	return s;
}

miscItemSharedPtr spellbook_StartouchedWritings()
{
	miscItemSharedPtr s(new miscItem("Star-touched Writings", SPELL_TILE, TCODColor::lightFuchsia,
		"These page seems faintly unreal. Moshka's handwriting is unmistakable, and grows increasingly fervent as \
the writings proceed, until at least he seems wholly unhinged."));
	s->setKeyTag("startouched_writings");
	return s;
}

miscItemSharedPtr spellbook_OrsylsTomeOfPrayer()
{
	miscItemSharedPtr s(new miscItem("Orsyl's Tome of Prayer", SPELL_TILE, TCODColor::lightPurple,
		"In his final days, Orsyl's holy prayers were profaned by his contact with the Void."));
	s->setKeyTag("orsyls_tome_of_prayer");
	return s;
}

miscItemSharedPtr spellbook_DivineMoonsparkTome()
{
	miscItemSharedPtr s(new miscItem("Divine Moonspark Tome", SPELL_TILE, TCODColor::lighterYellow,
		"This tome, encrusted with mud, contains the divine knowledge of the lost saint Yutria."));
	s->setKeyTag("divine_moonspark_tome");
	return s;
}

miscItemSharedPtr spellbook_DivineTomeOfTheEmissary()
{
	miscItemSharedPtr s(new miscItem("Divine Tome of the Emissary", SPELL_TILE, TCODColor::lightFlame,
		"The writings of the Emissaries are filled with edicts of cleansing and proclamations of divine justice, \
as befit their role in the Valley."));
	s->setKeyTag("divine_tome_of_the_emissary");
	return s;
}

miscItemSharedPtr spellbook_DivineNightmareTome()
{
	miscItemSharedPtr s(new miscItem("Divine Nightmare Tome", SPELL_TILE, TCODColor::magenta,
		"Before she was devoured by the Void, Iedra, last wizard of the Winter Court, sought a truth beyond the \
stars. The First Sparrow rejected her offer to teach him the prayers that led her to this holy knowledge, however, \
and she was cast out to wither away."));
	s->setKeyTag("divine_nightmare_tome");
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

miscItemSharedPtr runestone_KinslayersRunestone()
{
	miscItemSharedPtr r(new miscItem("Kinslayer's Runestone", RUNESTONE_TILE, TCODColor::red,
		"The Kinslayer's name was lost to time, but his misdeeds were never forgotten. They say his \
wickedness was sealed into this gore-soaked stone."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Kinslayer's Runestone", "Slayer's", TCODColor::red, SCALE_PHYSICAL));
	return r;
}

miscItemSharedPtr runestone_StarweaversRunestone()
{
	miscItemSharedPtr r(new miscItem("Starweaver's Runestone", RUNESTONE_TILE, TCODColor::magenta,
		"The Sparrow's aim was to someday reach the stars, and so his followers delved deep into the arcane secrets of \
the runestones. It was said that all rune-writings eventually laid one bare to the gaze of the gods."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Starweaver's Runestone", "Weaving", TCODColor::magenta, SCALE_MAGIC));
	return r;
}

miscItemSharedPtr runestone_ThundrousRunestone()
{
	miscItemSharedPtr r(new miscItem("Thundrous Runestone", RUNESTONE_TILE, TCODColor::lightPurple,
		"Eventually, the inquiries of the Sparrows was bound to reach the eye of the gods. Who could say whether they \
would look kindly upon such searching? Always, when the gods were threatened with the uncovering of their holy mysteries, \
they responded with holy lightning."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Thundrous Runestone", "Thundrous", TCODColor::lightPurple, SCALE_ELECTRIC));
	return r;
}

miscItemSharedPtr runestone_BloodDrenchedRunestone()
{
	miscItemSharedPtr r(new miscItem("Blood-drenched Runestone", RUNESTONE_TILE, TCODColor::crimson,
		"Runestone infused with gore from the heart of Coren, the Conqueror, whose blood was said to contain immense power."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Blood Drenched Runestone", "Bloody", TCODColor::crimson, SCALE_BLEED));
	return r;
}

miscItemSharedPtr runestone_CorrodingRunestone()
{
	miscItemSharedPtr r(new miscItem("Corroding Runestone", RUNESTONE_TILE, TCODColor::green,
		"Runestone that exudes corrosive acid, bringing with it a potent stench."));
	r->isRunestone = true;
	r->setRune(new weaponRune("Corroding Runestone", "Corrosive", TCODColor::green, SCALE_ACID));
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

miscItemSharedPtr bones_CorensBones()
{
	miscItemSharedPtr b(new miscItem("Coren's Bones", KEY_TILE, TCODColor::lightestYellow,
		"All that remains of Coren, who prayed to the old gods for glory and victory, and who once laid waste to the\
 lowlands. After the gods had slaked their thirst, his bones were laid to rest in the muck."));
	b->setKeyTag("corens_bones");
	return b;
}

miscItemSharedPtr bones_IetrasBones()
{
	miscItemSharedPtr b(new miscItem("Ietra's Bones", KEY_TILE, TCODColor::lightestYellow,
		"All that remains of Ietra, who sought truth among the fallen stars. According to certain storytellers, \
she told Moshka the true history of the immortals, and then fell to lie eternally in the void."));
	b->setKeyTag("ietras_bones");
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
