#include "spell.h"


//Standard constructor
spell::spell(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_SPELL, description)
{
}

//Quick-and-dirty constructor for creating ranged-weapon-attack spells
spell::spell(int attackRange, int physDamage, effect otherEffect, int otherEffectPotency) :
	attackRange(attackRange), aType(ATTACK_RANGE)
{
	if (physDamage > 0)
		addEffect(APPLY_PHYSICAL_DAMAGE, physDamage);
	if (otherEffect != NULL_EFFECT)
		addEffect(otherEffect, otherEffectPotency);
	this->vigourCost = 0;
}

spell::~spell()
{
}

/*
	SETTING
*/

void spell::setAttackType(attackType aType, int attackRange)
{
	this->aType = aType;
	this->attackRange = attackRange;
}

void spell::addEffect(effect eff, int potency)
{
	effectsApplied.push_back(eff);
	effectPotencies.push_back(potency);
}



/*
Spell objects
*/

//Weapon arts

spell * attack_Splintering()
{
	spell* sp = new spell("Splintering", SWORD_TILE, TCODColor::crimson,
		"Deals 20 bleed damage to target.");
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(APPLY_BLEED_DAMAGE, 20);
	sp->setVigourCost(1);
	return sp;
}

spell * attack_Quickstep()
{
	spell* sp = new spell("Quickstep", SWORD_TILE, TCODColor::yellow,
		"Quickly leap one space.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->setVigourCost(1);
	sp->addEffect(GAIN_FREE_MOVES, 2);
	return sp;
}

spell * attack_StrongAttack()
{
	spell* sp = new spell("Strong Attack", SWORD_TILE, TCODColor::darkRed,
		"Next attack deals double damage.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_ATTACK, 100);
	sp->setVigourCost(1);
	return sp;
}

spell * attack_Knockback()
{
	spell* sp = new spell("Knockback", SWORD_TILE, TCODColor::lightBlue,
		"Next attack knocks target back.");
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(KNOCKBACK_TARGET, 2);
	sp->setVigourCost(1);
	return sp;
}

spell * attack_MightyCleave()
{
	spell* sp = new spell("Mighty Cleave", SWORD_TILE, TCODColor::grey,
		"Attack all adjacent targets.");
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->setVigourCost(2);
	return sp;
}

spell * attack_Spearfishing()
{
	spell* sp = new spell("Spearfishing", SWORD_TILE, TCODColor::lime,
		"Ranged attack yanks target closer.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->setVigourCost(1);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->addEffect(PULL_CLOSER, 1);
	return sp;
}

spell * attack_Fillet()
{
	spell* sp = new spell("Fillet", DAGGER_TILE, TCODColor::crimson,
		"Make multiple attacks at once.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->setVigourCost(1);
	sp->addEffect(GAIN_MULTIPLE_ATTACKS, 3);
	return sp;
}

spell * attack_DivineSmite()
{
	spell* sp = new spell("Divine Smite", HAMMER_TILE, TCODColor::darkYellow,
		"Smite target with holy fury.");
	sp->setAttackType(ATTACK_MELEE);
	sp->setVigourCost(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 100);
	sp->addEffect(APPLY_BLINDING, 1);
	return sp;
}

spell * attack_BloodFeast()
{
	spell* sp = new spell("Blood Feast", SPELL_TILE, TCODColor::crimson,
		"Inflict bleed on oneself. Next attack deals +15 damage.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->addEffect(SCALE_NEXT_ATTACK, 15);
	sp->setVigourCost(1);
	return sp;
}

spell * attack_HolySummons()
{
	spell* sp = new spell("Holy Summons", SPELL_TILE, TCODColor::darkYellow,
		"Pull target closer and deal 25 holy damage.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 25);
	sp->addEffect(PULL_CLOSER, 2);
	sp->setVigourCost(3);
	return sp;
}

spell * attack_VoidSlash()
{
	spell* sp = new spell("Void Slash", DAGGER_TILE, TCODColor::lightPurple,
		"Damage target and fling away.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(2);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->addEffect(KNOCKBACK_TARGET, 1);
	sp->setVigourCost(2);
	return sp;
}

spell * attack_VoidSurge()
{
	spell* sp = new spell("Void Surge", SWORD_TILE, TCODColor::lightFuchsia,
		"Weapon surges with profane power, but damages you.");
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_PROFANE, 50);
	sp->setVigourCost(3);
	sp->setDamageToCaster(20);
	return sp;
}

//Wand/chime powers

spell * ability_PowerSurge()
{
	spell* sp = new spell("Power Surge", SWORD_TILE, TCODColor::magenta,
		"Your next spell gains +100% power.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 100);
	sp->setVigourCost(1);
	return sp;
}

spell * ability_AcidInfusion()
{
	spell* sp = new spell("Acid Infusion", SWORD_TILE, TCODColor::darkLime,
		"Next attack spell deals acid damage. This is especially effective using acid-based spells.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SPELL_ACID_INFUSION, 25);
	sp->setVigourCost(1);
	return sp;
}

spell * ability_DivineFavour()
{
	spell* sp = new spell("Divine Favour", SWORD_TILE, TCODColor::darkYellow,
		"Slowly restore some health.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(ADD_HEALTH_TRICKLE, 15);
	sp->setVigourCost(1);
	return sp;
}

spell * ability_WyrdChannel()
{
	spell* sp = new spell("Wyrd Channel", SWORD_TILE, TCODColor::lightSepia,
		"Next prayer has 150% power.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_PRAYER, 50);
	sp->setVigourCost(1);
	return sp;
}


//Spells

spell * spell_MagicMissile()
{
	spell* sp = new spell("Magic Missile", SPELL_TILE, TCODColor::magenta,
		"Attack deals magic damage. Magic blasting is the simplest kind of magic there is, and perhaps the most useful.");
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 25);
	sp->setAttackRange(5);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	return sp;
}

spell * spell_ArcaneRadiance()
{
	spell* sp = new spell("Arcane Radiance", SPELL_TILE, TCODColor::darkMagenta,
		"A blast of magical energy strikes everything within 3 spaces. In Sunken Atalundra, when arcane power was harmless, \
mages used this as a party trick.");
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(3);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spell * spell_ArcaneBlade()
{
	spell* sp = new spell("Arcane Blade", SPELL_TILE, TCODColor::lightMagenta,
		"Your weapon deals arcane damage. This is an old magick of Sunken Atalundra, which popularized the arcane arts before \
they sunk beneath the waves.");
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_MAGIC, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spell * spell_GottricsArcaneProtection()
{
	spell* sp = new spell("Gottric's Arcane Protection", SPELL_TILE, TCODColor::lighterMagenta,
		"Confers protection from magic damage. Gottric used this spell to protect himself as he delved ever deeper into the \
mysteries of Sunken Atalundra.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_MAGIC_RESIST, 20);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->addPermanentBuff = true;
	return sp;
}

spell * spell_Frostbolt()
{
	spell* sp = new spell("Frostbolt", SPELL_TILE, TCODColor::cyan,
		"Bolt deals cold damage. The mages of Sunken Atalundra attempted to use ice magic to protect their city from the rising \
waves, though of course they did not succeed.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(5);
	sp->addEffect(APPLY_COLD_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	sp->isColdSpell = true;
	return sp;
}

spell * spell_AcidSpit()
{
	spell* sp = new spell("Acid Spit", SPELL_TILE, TCODColor::lightGreen,
		"This spell draws upon the anger of the earth itself towards its despoilers. It was discovered by the Wyrd, who \
fruitlessly stood against the annihilation of their homeland.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(5);
	sp->addEffect(APPLY_ACID_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	sp->isAcidSpell = true;
	return sp;
}

spell * spell_AcidBlade()
{
	spell* sp = new spell("Acid Blade", SPELL_TILE, TCODColor::lime,
		"Coat one's blade in corrosive acid to disintegrate the armour of your enemies. The Wyrd discovered this spell and taught \
it to the angelic visitors who rose from the earth.");
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_ACID, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spell * spell_AcidBurst()
{
	spell* sp = new spell("Acid Burst", SPELL_TILE, TCODColor::darkLime,
		"Spew a powerful blast of acid to disintegrate armour and burn enemies.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(2);
	sp->addEffect(APPLY_ACID_DAMAGE, 50);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isAcidSpell = true;
	return sp;
}

spell * spell_ProfanedBlade()
{
	spell* sp = new spell("Profaned Blade", SPELL_TILE, TCODColor::darkPurple,
		"A blade touched by this spell briefly ceases to exist, replaced by a sucking portal that draws flesh into the Void.");
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_PROFANE, 15);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spell * spell_SpitFire()
{
	spell* sp = new spell("Spit Fire", SPELL_TILE, TCODColor::flame,
		"Discharge a stream of fire.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_FIRE_DAMAGE, 20);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	return sp;
}



//Prayers



spell * prayer_Restoration()
{
	spell* sp = new spell("Restoration", SPELL_TILE, TCODColor::darkYellow,
		"Call upon the favour of the old gods to restore life essence. But the old gods are fickle, and help only those who \
strike their fancy.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(RESTORE_HEALTH, 25);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_WyrdChantOfStrength()
{
	spell* sp = new spell("Wyrd Chant of Strength", SPELL_TILE, TCODColor::darkGreen,
		"Increases defence by 10%. During the final days of the Wyrd, this chant filled the forest, though it did no good.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_DEFENCE, 10);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	sp->addPermanentBuff = true;
	return sp;
}

spell * prayer_RayOfLight()
{
	spell* sp = new spell("Ray of Light", SPELL_TILE, TCODColor::lightYellow,
		"Fires a ray of blessed energy.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_BlessedRadiance()
{
	spell* sp = new spell("Blessed Radiance", SPELL_TILE, TCODColor::lighterYellow,
		"Blind nearby creatures with holy radiance.");
	sp->setAttackType(ATTACK_AOE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 20);
	sp->addEffect(APPLY_BLINDING, 2);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_RemovePoison()
{
	spell* sp = new spell("Remove Poison", SPELL_TILE, TCODColor::green,
		"Cleanses the body of poison.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(REMOVE_POISON, 1);
	sp->setVigourCost(1);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_ProfaneRadiance()
{
	spell* sp = new spell("Profane Radiance", SPELL_TILE, TCODColor::purple,
		"Spell deals profane damage in a blast around you.");
	sp->setAttackType(ATTACK_AOE);
	sp->setAttackRange(2);
	sp->setVigourCost(2);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->addEffect(APPLY_BLINDING, 2);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_ProfaneRay()
{
	spell* sp = new spell("Profane Ray", SPELL_TILE, TCODColor::lightPurple,
		"Fire a ray of profane energy, drawing upon the forbidden power of the Void.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->setVigourCost(1);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->usesDivinePower = true;
	return sp;
}

spell * prayer_SinkBeneath()
{
	spell* sp = new spell("Sink Beneath", SPELL_TILE, TCODColor::cyan,
		"Plunge below the waves and re-emerge somewhere unknown.");
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TELEPORT_VIA_WATER, 5);
	sp->usesDivinePower = true;
	sp->setVigourCost(1);
	return sp;
}

spell * prayer_DrawOutTheBlood()
{
	spell* sp = new spell("Draw Out the Blood", SPELL_TILE, TCODColor::crimson,
		"Certain adherents of the Void acquired a great interest in blood. Orsyl was convinced that a secret power lay in \
the blood of the living, but he never found it.");
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(HURT_BLEEDER, 50);
	sp->usesDivinePower = true;
	sp->setVigourCost(2);
	return sp;
}




//Monster abilities


spell * ability_Charge()
{
	spell* sp = new spell("Charge", SPELL_TILE, TCODColor::white);
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_FREE_MOVES, 3);
	sp->usesSpellPower = false;
	return sp;
}

spell* ability_Burst()
{
	spell* sp = new spell("Burst", SPELL_TILE, TCODColor::darkLime);
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(2);
	return sp;
}

spell * ability_CrowArrow()
{
	spell* sp = new spell("Crow Arrow", SPELL_TILE, TCODColor::lightGrey);
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 10);
	return sp;
}

spell * ability_FeedingFrenzy()
{
	spell* sp = new spell("Feeding Frenzy", SPELL_TILE, TCODColor::lime);
	sp->setAttackType(ATTACK_AOE);
	sp->setAttackRange(3);
	sp->addEffect(GAIN_FREE_MOVES, 1);
	return sp;
}

spell * ability_DropOoze()
{
	spell* sp = new spell("Drop Ooze", SPELL_TILE, TCODColor::sepia);
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(DROP_OOZE, 1);
	return sp;
}

spell * ability_ShadowWalk()
{
	spell* sp = new spell("Shadow Walk", SPELL_TILE, TCODColor::purple);
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(BECOME_INVISIBLE, 2);
	return sp;
}

spell * ability_TentacleSwarm()
{
	spell* sp = new spell("Tentacle Swarm", SPELL_TILE, TCODColor::green,
		"Entangles target, preventing them from moving.");
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_ENTANGLING, 3);
	sp->setVigourCost(2);
	return sp;
}

spell * ability_Throw()
{
	spell* sp = new spell("Throw", SPELL_TILE, TCODColor::green);
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(1);
	sp->addEffect(KNOCKBACK_TARGET, 5);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	return sp;
}

spell * ability_VoidStep()
{
	spell* sp = new spell("Void Step", SPELL_TILE, TCODColor::purple);
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TELEPORT, 5);
	return sp;
}
