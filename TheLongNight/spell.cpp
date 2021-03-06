#include "spell.h"


//Standard constructor
spell::spell(std::string name, int tileCode, TCODColor color, std::string description) :
	item(name, tileCode, color, ITEM_SPELL, description)
{
}

//Quick-and-dirty constructor for creating ranged-weapon-attack spells
spell::spell(std::string name, TCODColor color, int attackRange, int physDamage, effect otherEffect, int otherEffectPotency) :
	attackRange(attackRange), aType(ATTACK_RANGE)
{
	if (physDamage > 0)
		addEffect(APPLY_PHYSICAL_DAMAGE, physDamage);
	if (otherEffect != NULL_EFFECT)
		addEffect(otherEffect, otherEffectPotency);
	this->vigourCost = 0;
	this->name = name;
	this->color = color;
}


//Quick-and-dirty constructor for creating any spell with a single effect.
spell::spell(std::string name, TCODColor color, attackType aType, int range, effect mainEff, int potency) :
	aType(aType), attackRange(range)
{
	this->name = name;
	this->color = color;
	addEffect(mainEff, potency);
}

//Same as above, but with two effects.
spell::spell(std::string name, TCODColor color, attackType aType, int range, effect eff1, int pot1, effect eff2, int pot2) :
	aType(aType), attackRange(range)
{
	this->name = name;
	this->color = color;
	addEffect(eff1, pot1);
	addEffect(eff2, pot2);
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
Returns whether we have the given effect.
*/
bool spell::hasEffect(effect eff)
{
	for (auto e : effectsApplied) {
		if (e == eff)
			return true;
	}
	return false;
}



/*
Spell objects
*/

//Weapon arts

spellSharedPtr attack_Splintering()
{
	spellSharedPtr sp(new spell("Splintering", SWORD_TILE, TCODColor::crimson,
		"Deals 20 bleed damage to target."));
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(APPLY_BLEED_DAMAGE, 20);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_CrowShriek()
{
	spellSharedPtr sp(new spell("Crow Shriek", SWORD_TILE, TCODColor::crimson,
		"Enchanted crowfeathers coat the blade, increasing bleed damage dealt."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setStatusEffectBuff(EFFECT_BLEED, 15);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_Quickstep()
{
	spellSharedPtr sp(new spell("Quickstep", SWORD_TILE, TCODColor::yellow,
		"Quickly leap one space."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->setVigourCost(1);
	sp->addEffect(GAIN_FREE_MOVES, 2);
	return sp;
}

spellSharedPtr attack_StrongAttack()
{
	spellSharedPtr sp(new spell("Strong Attack", SWORD_TILE, TCODColor::darkRed,
		"Next attack deals double damage."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_ATTACK, 100);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_Knockback()
{
	spellSharedPtr sp(new spell("Knockback", SWORD_TILE, TCODColor::lightBlue,
		"Next attack knocks target back."));
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(KNOCKBACK_TARGET, 2);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_MightyCleave()
{
	spellSharedPtr sp(new spell("Mighty Cleave", SWORD_TILE, TCODColor::grey,
		"Attack all adjacent targets."));
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_Spearfishing()
{
	spellSharedPtr sp(new spell("Spearfishing", SWORD_TILE, TCODColor::lime,
		"Ranged attack yanks target closer."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->setVigourCost(1);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->addEffect(PULL_CLOSER, 1);
	return sp;
}

spellSharedPtr attack_Fillet()
{
	spellSharedPtr sp(new spell("Fillet", DAGGER_TILE, TCODColor::crimson,
		"Make multiple attacks at once."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->setVigourCost(1);
	sp->addEffect(GAIN_MULTIPLE_ATTACKS, 3);
	return sp;
}

spellSharedPtr attack_DivineSmite()
{
	spellSharedPtr sp(new spell("Divine Smite", HAMMER_TILE, TCODColor::darkYellow,
		"Smite target with holy fury."));
	sp->setAttackType(ATTACK_MELEE);
	sp->setVigourCost(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 100);
	sp->addEffect(APPLY_BLINDING, 1);
	return sp;
}

spellSharedPtr attack_BloodFeast()
{
	spellSharedPtr sp(new spell("Blood Feast", SPELL_TILE, TCODColor::crimson,
		"Inflict bleed on oneself. Next attack deals +25 damage."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->addEffect(SCALE_NEXT_ATTACK, 25);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_HolySummons()
{
	spellSharedPtr sp(new spell("Holy Summons", SPELL_TILE, TCODColor::darkYellow,
		"Pull target closer and deal 25 holy damage."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 25);
	sp->addEffect(PULL_CLOSER, 2);
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr attack_VoidSlash()
{
	spellSharedPtr sp(new spell("Void Slash", DAGGER_TILE, TCODColor::lightPurple,
		"Damage target and fling away."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(2);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->addEffect(KNOCKBACK_TARGET, 1);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_VoidSurge()
{
	spellSharedPtr sp(new spell("Void Surge", SWORD_TILE, TCODColor::lightFuchsia,
		"Weapon surges with profane power, but damages you."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_PROFANE, 50);
	sp->setVigourCost(3);
	sp->setDamageToCaster(20);
	return sp;
}

spellSharedPtr attack_FlameSwathe()
{
	spellSharedPtr sp(new spell("Flame Swathe", SPELL_TILE, TCODColor::darkFlame,
		"Adjacent targets take fire damage."));
	sp->setAttackType(ATTACK_AOE, 2);
	sp->addEffect(APPLY_FIRE_DAMAGE, 25);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_DripVenom()
{
	spellSharedPtr sp(new spell("Drip Venom", DAGGER_TILE, TCODColor::lime,
		"Blade drips with thick acid."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_ACID, 25);
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr attack_SuckOutLife()
{
	spellSharedPtr sp(new spell("Suck Out Life", DAGGER_TILE, TCODColor::lightCrimson,
		"The agony dealt by the nail pours life into you."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(RESTORE_HEALTH, 10);
	sp->usesDivinePower = true;
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_SpiderStrike()
{
	spellSharedPtr sp(new spell("Spider Strike", WEB_TILE, TCODColor::lightestGrey,
		"Ensnares target in a sticky web."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(APPLY_ENTANGLING, 1);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_DeepCut()
{
	spellSharedPtr sp(new spell("Deep Cut", DAGGER_TILE, TCODColor::orange,
		"Deal 100 damage to target and 25 damage to self."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 100);
	sp->addEffect(HURT_CASTER, 25);
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr attack_Lash()
{
	spellSharedPtr sp(new spell("Lash", WHIP_TILE, TCODColor::lightRed,
		"Attack from up to 3 spaces away, dealing extra bleed damage."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_EtherealStrike()
{
	spellSharedPtr sp(new spell("Ethereal Strike", SWORD_TILE, TCODColor::lightMagenta,
		"Special attack deals damage that ignores all armour."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(APPLY_UNTYPED_DAMAGE, 50);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_Frenzy()
{
	spellSharedPtr sp(new spell("Frenzy", SWORD_TILE, TCODColor::crimson,
		"Next attack deals double damage."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_ATTACK, 100);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_PercivelsFire()
{
	spellSharedPtr sp(new spell("Percivel's Fire", SWORD_TILE, TCODColor::flame,
		"Blast your foe with holy fire."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(APPLY_FIRE_DAMAGE, 25);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 25);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_KnifeCast()
{
	spellSharedPtr sp(new spell("Knife Cast", DAGGER_TILE, TCODColor::flame,
		"Cast the knife as a ranged attack; it instantly returns to your hand."));
	sp->setAttackType(ATTACK_RANGE, 4);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_LightningSlam()
{
	spellSharedPtr sp(new spell("Lightning Slam", HAMMER_TILE, TCODColor::lightPurple,
		"Slam target with a melee attack that launches them away."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(KNOCKBACK_TARGET, 3);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_BillowingFlames()
{
	spellSharedPtr sp(new spell("Billowing Flames", SPEAR_TILE, TCODColor::darkFlame,
		"Imbues the scythe with billowing fire."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_FIRE, 25);
	sp->setVigourCost(5);
	return sp;
}

spellSharedPtr attack_Penetrate()
{
	spellSharedPtr sp(new spell("Penetrate", SPEAR_TILE, TCODColor::lightBlue,
		"Strike a target up to 3 spaces away with the long pike, dealing additional damage."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(SCALE_NEXT_ATTACK, 2);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_EtherealRay()
{
	spellSharedPtr sp(new spell("Ethereal Ray", SPELL_TILE, TCODColor::lightMagenta,
		"Fires a beam of magical energy."));
	sp->setAttackType(ATTACK_RANGE, 5);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 25);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_PaleSlash()
{
	spellSharedPtr sp(new spell("Pale Slash", SPEAR_TILE, TCODColor::lightestBlue,
		"Attack deals cold, acid and electric damage."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(APPLY_COLD_DAMAGE, 25);
	sp->addEffect(APPLY_ACID_DAMAGE, 25);
	sp->addEffect(APPLY_ELECTRIC_DAMAGE, 25);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_PoisonHook()
{
	spellSharedPtr sp(new spell("Poison Hook", DAGGER_TILE, TCODColor::lightAmber,
		"Attack deals poison and bleed damage."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	sp->addEffect(APPLY_POISON_DAMAGE, 20);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_FlameBreath()
{
	spellSharedPtr sp(new spell("Flame Breath", DAGGER_TILE, TCODColor::darkFlame,
		"Blow a stream of fire."));
	sp->setAttackType(ATTACK_RANGE, 5);
	sp->addEffect(APPLY_FIRE_DAMAGE, 50);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr attack_BlackwatchBattlecry()
{
	spellSharedPtr sp(new spell("Blackwatch Battlecry", SWORD_TILE, TCODColor::purple,
		"The roar of the Blackwatch provides a surge of power."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_ATTACK, 200);
	sp->addEffect(ADD_HEALTH_TRICKLE, 20);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_RatCleave()
{
	spellSharedPtr sp(new spell("Rat Cleave", SWORD_TILE, TCODColor::sepia,
		"Attack all adjacent targets and hurl them away."));
	sp->setAttackType(ATTACK_AOE, 1);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->addEffect(KNOCKBACK_TARGET, 3);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr attack_BlightingInfusion()
{
	spellSharedPtr sp(new spell("Blighted Rage", SWORD_TILE, TCODColor::amber,
		"The ancient power of the old blade strikes all nearby targets."));
	sp->setAttackType(ATTACK_AOE, 4);
	sp->addEffect(CASTER_MELEE_ATTACK, 1);
	sp->setVigourCost(4);
	return sp;
}

spellSharedPtr attack_Deathstrike()
{
	spellSharedPtr sp(new spell("Deathstrike", SWORD_TILE, TCODColor::red,
		"Infuses your next attack with the raw power of death itself."));
	sp->setAttackType(ATTACK_MELEE);
	sp->addEffect(APPLY_UNTYPED_DAMAGE, 1000);
	sp->setVigourCost(10);
	return sp;
}

spellSharedPtr ability_EtherealSurge()
{
	spellSharedPtr sp(new spell("Ethereal Surge", SPELL_TILE, TCODColor::magenta,
		"Your next two spells are cast instantly."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(INSTANT_SPELL_CAST, 2);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_TransitorySurge()
{
	spellSharedPtr sp(new spell("Transitory Surge", SPELL_TILE, TCODColor::fuchsia,
		"Grants a temporary surge of magical power."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 200);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_Metamagic()
{
	spellSharedPtr sp(new spell("Metamagic", SPELL_TILE, TCODColor::white,
		"Next spell has reduced power but costs (3) less."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, -20);
	sp->addEffect(NEXT_SPELL_COST_ADJUST, -3);
	sp->setVigourCost(0);
	return sp;
}

spellSharedPtr ability_GottricsPartingCall()
{
	spellSharedPtr sp(new spell("Gottric's Parting Call", SPELL_TILE, TCODColor::purple,
		"Next spell gains 1000% power."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 1000);
	sp->setVigourCost(10);
	return sp;
}

spellSharedPtr ability_StrengthOfMind()
{
	spellSharedPtr sp(new spell("Strength of Mind", SPELL_TILE, TCODColor::orange,
		"Temporarily boosts both prayers and spells."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 20);
	sp->addEffect(SCALE_NEXT_PRAYER, 20);
	sp->setVigourCost(1);
	return sp;
}

//Wand/chime powers

spellSharedPtr ability_PowerSurge()
{
	spellSharedPtr sp(new spell("Power Surge", SWORD_TILE, TCODColor::magenta,
		"Your next spell gains +150% power."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_SPELL, 150);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_AcidInfusion()
{
	spellSharedPtr sp(new spell("Acid Infusion", SWORD_TILE, TCODColor::darkLime,
		"Next attack spell deals acid damage. This is especially effective using acid-based spells."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SPELL_ACID_INFUSION, 25);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_DivineFavour()
{
	spellSharedPtr sp(new spell("Divine Favour", SWORD_TILE, TCODColor::darkYellow,
		"Slowly restore some health."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(ADD_HEALTH_TRICKLE, 15);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_WyrdChannel()
{
	spellSharedPtr sp(new spell("Wyrd Channel", SWORD_TILE, TCODColor::lightSepia,
		"Next prayer has +50% power."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(SCALE_NEXT_PRAYER, 50);
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr ability_Rotcall()
{
	spellSharedPtr sp(new spell("Rotcall", SPELL_TILE, TCODColor::darkAmber,
		"Speak a fetid chant to coat your weapon with poison."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setStatusEffectBuff(EFFECT_POISON, 10);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr ability_MemoryOfLight()
{
	spellSharedPtr sp(new spell("Memory of Light", SPELL_TILE, TCODColor::lightGreen,
		"Memories of prior days restore a hint of life."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(RESTORE_HEALTH, 25);
	sp->addEffect(ADD_HEALTH_TRICKLE, 25);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr ability_FogCloud()
{
	spellSharedPtr sp(new spell("Fog Cloud", SPELL_TILE, TCODColor::white,
		"Create a cloud of fog."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(CREATE_FOG, 5);
	return sp;
}




//Shield powers


spellSharedPtr ability_ShieldBlock()
{
	spellSharedPtr sp(new spell("Shield Block", SHIELD_TILE, TCODColor::sepia,
		"Extra defence the next time you take damage."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TEMP_GAIN_DEFENCE, 50);
	return sp;
}

spellSharedPtr ability_ElementalShieldBlock()
{
	spellSharedPtr sp(new spell("Elemental Shield Block", SHIELD_TILE, TCODColor::white,
		"Extra defence against acid, cold, fire and electric the next time you take damage."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TEMP_GAIN_ELEMENTAL_DEFENCE, 50);
	return sp;
}

spellSharedPtr ability_DivineGuardian()
{
	spellSharedPtr sp(new spell("Divine Guardian", SHIELD_TILE, TCODColor::yellow,
		"Call upon the protectors of the city to regain some health."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(ADD_HEALTH_TRICKLE, 50);
	return sp;
}

spellSharedPtr ability_VoidAura()
{
	spellSharedPtr sp(new spell("Void Aura", SHIELD_TILE, TCODColor::purple,
		"Call upon the Void to sap life from adjacent enemies."));
	sp->setAttackType(ATTACK_AOE, 1);
	sp->addEffect(APPLY_UNTYPED_DAMAGE, 25);
	return sp;
}

spellSharedPtr ability_FlameBreath()
{
	spellSharedPtr sp(new spell("Flame Breath", SHIELD_TILE, TCODColor::flame,
		"Call upon the power of dragons to cast flame at your enemies."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->addEffect(APPLY_FIRE_DAMAGE, 50);
	return sp;
}

spellSharedPtr ability_MagesDisquiet()
{
	spellSharedPtr sp(new spell("Mage's Disquiet", SHIELD_TILE, TCODColor::magenta,
		"Silence all magic in your vicinity."));
	sp->setAttackType(ATTACK_AOE, 8);
	sp->addEffect(SILENCE, 10);
	return sp;
}

spellSharedPtr ability_KhallesCryOfDespair()
{
	spellSharedPtr sp(new spell("Khalle's Cry of Despair", SHIELD_TILE, TCODColor::darkYellow,
		"The cry of Khalle is said to confer lasting pain, but perhaps the gods look kindly upon it."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_MAX_HEALTH, -20);
	sp->addEffect(INCREASE_PRAYER_POWER, 50);
	sp->addPermanentBuff = true;
	return sp;
}

spellSharedPtr ability_Materialize()
{
	spellSharedPtr sp(new spell("Materialize", SHIELD_TILE, TCODColor::pink,
		"The shield briefly becomes real, conferring excellent physical defence."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TEMP_GAIN_DEFENCE, 100);
	return sp;
}

spellSharedPtr ability_Corrosion()
{
	spellSharedPtr sp(new spell("Envenom", SHIELD_TILE, TCODColor::lightGreen,
		"Touch your weapon to the shield to coat it with corrosive acid."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_ACID, 30);
	return sp;
}


//Spells

spellSharedPtr spell_MagicMissile()
{
	spellSharedPtr sp(new spell("Magic Missile", SPELL_TILE, TCODColor::magenta,
		"Attack deals magic damage. Magic blasting is the simplest kind of magic there is, and perhaps the most useful. \
(You need to have a wand equipped to use spells. Each wand can hold a number of spells equal to its Spellstore value. Press [q] \
to cast your selected spell.)"));
	sp->setAttackType(ATTACK_RANGE);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 25);
	sp->setAttackRange(5);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_ArcaneRadiance()
{
	spellSharedPtr sp(new spell("Arcane Burst", SPELL_TILE, TCODColor::darkMagenta,
		"A blast of magical energy strikes everything within 3 spaces. (Spells become more powerful the higher your Spell Power \
is. You can increase Spell Power by putting points into Arcana.)"));
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 25);
	sp->setAttackRange(3);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_ArcaneBlade()
{
	spellSharedPtr sp(new spell("Arcane Blade", SPELL_TILE, TCODColor::lightMagenta,
		"Your weapon deals arcane damage. This is an old magick of Sunken Atalundra, which popularized the arcane arts before \
they sunk beneath the waves."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_MAGIC, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_GottricsArcaneProtection()
{
	spellSharedPtr sp(new spell("Gottric's Arcane Protection", SPELL_TILE, TCODColor::lighterMagenta,
		"Confers protection from magic damage. Gottric used this spell to protect himself as he delved ever deeper into the \
mysteries of Sunken Atalundra."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_MAGIC_RESIST, 20);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->addPermanentBuff = true;
	return sp;
}

spellSharedPtr spell_Frostbolt()
{
	spellSharedPtr sp(new spell("Frostbolt", SPELL_TILE, TCODColor::cyan,
		"Bolt deals cold damage. The mages of Sunken Atalundra attempted to use ice magic to protect their city from the rising \
waves, though of course they did not succeed. (Being Frozen reduces physical defence for a short time.)"));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(5);
	sp->addEffect(APPLY_COLD_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	sp->isColdSpell = true;
	return sp;
}

spellSharedPtr spell_AcidSpit()
{
	spellSharedPtr sp(new spell("Acid Spit", SPELL_TILE, TCODColor::lightGreen,
		"This spell draws upon the anger of the earth itself towards its despoilers. It was discovered by the Wyrd, who \
fruitlessly stood against the annihilation of their homeland. (Acid damage can cause damage over time.)"));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(5);
	sp->addEffect(APPLY_ACID_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	sp->isAcidSpell = true;
	return sp;
}

spellSharedPtr spell_AcidBlade()
{
	spellSharedPtr sp(new spell("Acid Blade", SPELL_TILE, TCODColor::lime,
		"Coat one's blade in corrosive acid to disintegrate the armour of your enemies. The Wyrd discovered this spell and taught \
it to the angelic visitors who rose from the earth."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_ACID, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_AcidBurst()
{
	spellSharedPtr sp(new spell("Acid Burst", SPELL_TILE, TCODColor::darkLime,
		"Spew a powerful blast of acid to disintegrate armour and burn enemies."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(2);
	sp->addEffect(APPLY_ACID_DAMAGE, 50);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isAcidSpell = true;
	return sp;
}

spellSharedPtr spell_ProfanedBlade()
{
	spellSharedPtr sp(new spell("Profaned Blade", SPELL_TILE, TCODColor::darkPurple,
		"A blade touched by this spell briefly ceases to exist, replaced by a sucking portal that draws flesh into the Void. \
(Profane damage is more effective against Blessed creatures.)"));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_PROFANE, 15);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr spell_VoidJaunt()
{
	spellSharedPtr sp(new spell("Void Jaunt", SPELL_TILE, TCODColor::purple,
		"Total immersion in the Void is known to cause madness, disintegration, or a particularly unpleasant combination of the \
two. Luckily, this spell is only temporary."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(BECOME_INVISIBLE, 3);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr spell_DevouringVoidCloud()
{
	spellSharedPtr sp(new spell("Devouring Void Cloud", SPELL_TILE, TCODColor::darkViolet,
		"Opening pathways to the Void is a dangerous act that only the mad would entertain."));
	sp->setAttackType(ATTACK_AOE, 10);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 50);
	sp->setVigourCost(5);
	sp->usesSpellPower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr spell_FrostBlast()
{
	spellSharedPtr sp(new spell("Frost Blast", SPELL_TILE, TCODColor::cyan,
		"Spell discovered by the Winter Court, whose reign of ice chilled the surrounding lands."));
	sp->setAttackType(ATTACK_AOE, 3);
	sp->addEffect(APPLY_COLD_DAMAGE, 50);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isColdSpell = true;
	return sp;
}

spellSharedPtr spell_FrozenBlade()
{
	spellSharedPtr sp(new spell("Frozen Blade", SPELL_TILE, TCODColor::darkCyan,
		"The Winter Court coated their blades in ice and froze their enemies to death."));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_COLD, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isColdSpell = true;
	return sp;
}

spellSharedPtr spell_Chillbite()
{
	spellSharedPtr sp(new spell("Chillbite", SPELL_TILE, TCODColor::lightCyan,
		"An illusory jaw of ice closes around your foe, freezing them solid."));
	sp->setAttackType(ATTACK_RANGE, 6);
	sp->addEffect(APPLY_COLD_DAMAGE, 20);
	sp->addEffect(APPLY_ENTANGLING, 3);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	sp->isColdSpell = true;
	return sp;
}

spellSharedPtr spell_LightningStrike()
{
	spellSharedPtr sp(new spell("Lightning Strike", SPELL_TILE, TCODColor::lightPurple,
		"The old gods visited a just punishment upon the First Sparrow, but their cleansing fire only strengthened \
his resolve. According to certain storytellers, in later days, he learned to channel their power for himself, \
until he ascended to near-godhood."));
	sp->setAttackType(ATTACK_RANGE, 10);
	sp->addEffect(APPLY_ELECTRIC_DAMAGE, 25);
	sp->usesSpellPower = true;
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr spell_ElectricBlade()
{
	spellSharedPtr sp(new spell("Electric Blade", SPELL_TILE, TCODColor::lightPurple,
		"The weapons of the Sparrow Knights once flashed with arcane lightning, drawn from the earth \
by arcane power near to godhood. (Being Electrified reduces damage dealt by 10% for a short period.)"));
	sp->setAttackType(ATTACK_BUFF_WEAPON);
	sp->setBuffApplied(DAMAGE_ELECTRIC, 15);
	sp->usesSpellPower = true;
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr spell_ArcLightning()
{
	spellSharedPtr sp(new spell("Arc Lightning", SPELL_TILE, TCODColor::lightPurple,
		"Given enough time, even the most profound secrets underlying the universe can be discovered. At least, such \
was the belief of the First Sparrow, who delved deep into matters forbidden by the gods."));
	sp->setAttackType(ATTACK_AOE, 5);
	sp->addEffect(APPLY_ELECTRIC_DAMAGE, 50);
	sp->usesSpellPower = true;
	sp->setVigourCost(4);
	return sp;
}

spellSharedPtr spell_AtalundraArcaneSpear()
{
	spellSharedPtr sp(new spell("Atalundra Arcane Spear", SPELL_TILE, TCODColor::lightestMagenta,
		"Fire a surging beam of arcane energy. A spell discovered by the sages of Atalundra, who perished for their trouble."));
	sp->setAttackType(ATTACK_RANGE, 12);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 100);
	sp->addEffect(APPLY_ELECTRIC_DAMAGE, 50);
	sp->usesSpellPower = true;
	sp->alwaysHitTarget = true;
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr spell_AtalundraDeathSurge()
{
	spellSharedPtr sp(new spell("Atalundra Death Surge", SPELL_TILE, TCODColor::lightMagenta,
		"The final days of Atalundra were characterized by frantic research, as the scholars of the city searched for the \
knowledge that they were certain would be their salvation. In the end, all they found was this, inscribed on the wall of the \
sunken cathedral."));
	sp->setAttackType(ATTACK_AOE, 5);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 100);
	sp->inverseHealthScaling = true;
	sp->usesSpellPower = true;
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr spell_AtalundraSoulBlade()
{
	spellSharedPtr sp(new spell("Atalundra Soul Blade", SPELL_TILE, TCODColor::lighterMagenta,
		"As their city was consigned to the waves, the scholars of Atalundra despaired of ever concluding their great work. \
All that remained were the arcane words of this spell, which summon the spectre of a knight with a shining blade, half-visible."));
	sp->setAttackType(ATTACK_RANGE, 2);
	sp->addEffect(APPLY_MAGIC_DAMAGE, 500);
	sp->usesSpellPower = true;
	sp->alwaysHitTarget = true;
	sp->setVigourCost(5);
	return sp;
}

spellSharedPtr spell_SpitFire()
{
	spellSharedPtr sp(new spell("Spit Fire", SPELL_TILE, TCODColor::flame,
		"Discharge a stream of fire."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_FIRE_DAMAGE, 20);
	sp->setVigourCost(1);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_StormLash()
{
	spellSharedPtr sp(new spell("Storm Lash", SPELL_TILE, TCODColor::lightPurple,
		"This spell, born of the churning heart of the void, summons a thunderous whip that throws enemies back."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->addEffect(APPLY_ELECTRIC_DAMAGE, 25);
	sp->addEffect(KNOCKBACK_TARGET, 5);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_Firestorm()
{
	spellSharedPtr sp(new spell("Firestorm", SPELL_TILE, TCODColor::flame,
		"Spell of Ietra, who saw the flame at the heart of the void. She claimed that the truth of the immortals lay \
within that fire, but her tomb was sealed away from the world so that the knowledge would be forgotten."));
	sp->setAttackType(ATTACK_AOE, 5);
	sp->addEffect(APPLY_FIRE_DAMAGE, 50);
	sp->setVigourCost(3);
	sp->usesSpellPower = true;
	return sp;
}

spellSharedPtr spell_LightOfFarin()
{
	spellSharedPtr sp(new spell("Light of Farin", SPELL_TILE, TCODColor::lighterFlame,
		"Before he was consumed, Farin learned that even the dead carry a flame within them, and that the power \
of this flame can be harnessed."));
	sp->setAttackType(ATTACK_RANGE, 10);
	sp->addEffect(DEATHFIRE_INFUSION, 25);
	sp->setVigourCost(2);
	sp->usesSpellPower = true;
	return sp;
}



//Prayers



spellSharedPtr prayer_Restoration()
{
	spellSharedPtr sp(new spell("Restoration", PRAYER_TILE, TCODColor::darkYellow,
		"Call upon the favour of the old gods to restore life essence. But the old gods are fickle, and help only those who \
strike their fancy. (Prayers are more effective the higher your Divine Power is. Put points into Devotion to increase your divine \
power.)"));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(RESTORE_HEALTH, 25);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_WyrdChantOfStrength()
{
	spellSharedPtr sp(new spell("Wyrd Chant of Strength", PRAYER_TILE, TCODColor::darkGreen,
		"Increases defence by 10%. During the final days of the Wyrd, this chant filled the forest, though it did no good. \
(Buffs last until you die or rest.)"));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_DEFENCE, 10);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	sp->addPermanentBuff = true;
	return sp;
}

spellSharedPtr prayer_RayOfLight()
{
	spellSharedPtr sp(new spell("Ray of Light", PRAYER_TILE, TCODColor::lightYellow,
		"Fires a ray of blessed energy. (Blessed damage is more effective against Profane creatures.)"));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 25);
	sp->setVigourCost(1);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_BlessedRadiance()
{
	spellSharedPtr sp(new spell("Blessed Radiance", PRAYER_TILE, TCODColor::lighterYellow,
		"Blind nearby creatures with holy radiance."));
	sp->setAttackType(ATTACK_AOE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 20);
	sp->addEffect(APPLY_BLINDING, 2);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_RemovePoison()
{
	spellSharedPtr sp(new spell("Remove Poison", PRAYER_TILE, TCODColor::green,
		"Cleanses the body of poison. (Poison builds up in the blood, and once it takes hold, slowly causes damage until you \
rest or are killed.)"));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(REMOVE_POISON, 1);
	sp->setVigourCost(1);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_ProfaneRadiance()
{
	spellSharedPtr sp(new spell("Profane Radiance", PRAYER_TILE, TCODColor::purple,
		"Spell deals profane damage in a blast around you."));
	sp->setAttackType(ATTACK_AOE, 2);
	sp->setVigourCost(2);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->addEffect(APPLY_BLINDING, 2);
	sp->usesDivinePower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr prayer_Intoxify()
{
	spellSharedPtr sp(new spell("Intoxify", PRAYER_TILE, TCODColor::lime,
		"Blasts target with poison."));
	sp->setAttackType(ATTACK_RANGE, 4);
	sp->setVigourCost(1);
	sp->addEffect(APPLY_POISON_DAMAGE, 10);
	sp->usesDivinePower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr prayer_AstralPrison()
{
	spellSharedPtr sp(new spell("Astral Prison", PRAYER_TILE, TCODColor::lightFuchsia,
		"Target is transported to an astral prison. This spell is only effective against Profane targets."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->setVigourCost(10);
	sp->addEffect(ASTRAL_IMPRISONMENT, 1);
	sp->usesDivinePower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr prayer_ProfaneRay()
{
	spellSharedPtr sp(new spell("Profane Ray", PRAYER_TILE, TCODColor::lightPurple,
		"Fire a ray of profane energy, drawing upon the forbidden power of the Void."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->setVigourCost(1);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 25);
	sp->usesDivinePower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr prayer_Deathlink()
{
	spellSharedPtr sp(new spell("Deathlink", PRAYER_TILE, TCODColor::blue,
		"Students of the Arcane in Sunken Atalundra learned that the gods were the true source of arcane power. \
With this prayer, they begged the gods to send them life beyond death."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(DEATHLINK, 10);
	sp->setVigourCost(3);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_ProfoundStill()
{
	spellSharedPtr sp(new spell("Profound Still", PRAYER_TILE, TCODColor::lightBlue,
		"The clerics of Atalundra begged the gods to spare their kingdom, and were met only with profound silence."));
	sp->setAttackType(ATTACK_AOE, 5);
	sp->addEffect(SILENCE, 20);
	sp->setVigourCost(3);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_SinkBeneath()
{
	spellSharedPtr sp(new spell("Sink Beneath", PRAYER_TILE, TCODColor::cyan,
		"Plunge below the waves and re-emerge somewhere unknown."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TELEPORT_VIA_WATER, 5);
	sp->usesDivinePower = true;
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr prayer_OrsylsCall()
{
	spellSharedPtr sp(new spell("Orsyl's Call", PRAYER_TILE, TCODColor::purple,
		"Calling upon the old gods is a dangerous prospect, as Orsyl learned in his final days."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 20);
	sp->addEffect(SCALE_NEXT_PRAYER, 100);
	sp->setVigourCost(1);
	sp->usesDivinePower = true;
	sp->isProfane = true;
	return sp;
}

spellSharedPtr prayer_SpidersPrayer()
{
	spellSharedPtr sp(new spell("Spider's Prayer", PRAYER_TILE, TCODColor::white,
		"According to certain storytellers, the old gods gave power to the spiders to keep humanity from overrunning the earth."));
	sp->setAttackType(ATTACK_RANGE, 10);
	sp->addEffect(DROP_WEB, 1);
	sp->usesDivinePower = true;
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr prayer_TouchOfDoom()
{
	spellSharedPtr sp(new spell("Touch of Doom", PRAYER_TILE, TCODColor::lightYellow,
		"Khalle's devotion allowed her to glimpse the divine light of Heaven. Her followers claimed that \
the true shape of the old gods drove her mad, but they never discerned why."));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(GAIN_DEFENCE, -20);
	sp->addEffect(GAIN_ELECTRIC_RESIST, -20);
	sp->addEffect(GAIN_FIRE_RESIST, -20);
	sp->addEffect(GAIN_COLD_RESIST, -20);
	sp->addEffect(GAIN_ACID_RESIST, -20);
	sp->addPermanentBuff = true;
	sp->usesDivinePower = true;
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr prayer_DrawOutTheBlood()
{
	spellSharedPtr sp(new spell("Draw Out the Blood", PRAYER_TILE, TCODColor::crimson,
		"Certain adherents of the Void acquired a great interest in blood. Orsyl was convinced that a secret power lay in \
the blood of the living, but he never found it."));
	sp->setAttackType(ATTACK_RANGE, 6);
	sp->addEffect(HURT_BLEEDER, 50);
	sp->usesDivinePower = true;
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr prayer_DivineRetribution()
{
	spellSharedPtr sp(new spell("Divine Retribution", PRAYER_TILE, TCODColor::lightBlue,
		"Saint Yutria was surrounded by a divine aura wherever she went, such that the nightmare could never touch her."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addPermanentBuff = true;
	sp->addEffect(GAIN_DIVINE_RETRIBUTION, 10);
	sp->setVigourCost(4);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_YutriasDivineSpark()
{
	spellSharedPtr sp(new spell("Yutria's Divine Spark", PRAYER_TILE, TCODColor::lightestBlue,
		"Evildoers did not dare tread upon the footprints of Saint Yutria, for those who did were swiftly struck with divine fire."));
	sp->setAttackType(ATTACK_RANGE, 8);
	sp->addEffect(APPLY_BLESSED_DAMAGE, 20);
	sp->addEffect(APPLY_FIRE_DAMAGE, 20);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_YutriasProtection()
{
	spellSharedPtr sp(new spell("Yutria's Protection", PRAYER_TILE, TCODColor::lightBlue,
		"Grant oneself the elemental protection offered to Yutria, who walked upon death without fear."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_ACID_RESIST, 10);
	sp->addEffect(GAIN_COLD_RESIST, 10);
	sp->addEffect(GAIN_FIRE_RESIST, 10);
	sp->addEffect(GAIN_ELECTRIC_RESIST, 10);
	sp->setVigourCost(4);
	sp->usesDivinePower = true;
	sp->addPermanentBuff = true;
	return sp;
}

spellSharedPtr prayer_DivineRestoration()
{
	spellSharedPtr sp(new spell("Divine Restoration", PRAYER_TILE, TCODColor::flame,
		"Slowly restores a large amount of health."));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(ADD_HEALTH_TRICKLE, 100);
	sp->setVigourCost(2);
	sp->usesDivinePower = true;
	return sp;
}

spellSharedPtr prayer_DivineJudgement()
{
	spellSharedPtr sp(new spell("Divine Judgement", PRAYER_TILE, TCODColor::lightFlame,
		"Reduces damage dealt by enemies."));
	sp->setAttackType(ATTACK_AOE, 3);
	sp->addEffect(APPLY_DAMAGE_PENALTY, 20);
	sp->usesDivinePower = true;
	sp->addPermanentBuff = true;
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr prayer_DivineForgiveness()
{
	spellSharedPtr sp(new spell("Divine Forgiveness", PRAYER_TILE, TCODColor::darkFlame,
		"Cleanse oneself of all ailments."));
	sp->setAttackType(ATTACK_BUFF_SELF, 1);
	sp->addEffect(REMOVE_BLEED, 1);
	sp->addEffect(REMOVE_POISON, 1);
	sp->addEffect(REMOVE_PLAGUE, 1);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr prayer_NightmarePrayer()
{
	spellSharedPtr sp(new spell("Nightmare Prayer", PRAYER_TILE, TCODColor::orange,
		"Perhaps the old gods did hear the prayers of Ietra, who - surrounded by enemies - called upon their aid, \
and found her enemies fleeing; but their attention did not serve her well in the end."));
	sp->setAttackType(ATTACK_AOE, 10);
	sp->addEffect(INSTILL_FEAR, 10);
	sp->usesDivinePower = true;
	sp->setVigourCost(3);
	return sp;
}

spellSharedPtr prayer_WordOfUnmaking()
{
	spellSharedPtr sp(new spell("Word of Unmaking", PRAYER_TILE, TCODColor::darkOrange,
		"Ietra began to see the true shape of things, buried within the arcane mysteries, and asked the gods to \
unmake her being; with which they happily complied."));
	sp->setAttackType(ATTACK_RANGE, 10);
	sp->addEffect(APPLY_UNTYPED_DAMAGE, 100);
	sp->addEffect(HURT_CASTER, 50);
	sp->usesDivinePower = true;
	sp->setVigourCost(1);
	return sp;
}

spellSharedPtr prayer_GazeIntoEmptiness()
{
	spellSharedPtr sp(new spell("Gaze Into Emptiness", PRAYER_TILE, TCODColor::orange,
		"Ietra's glimpse of the Void placed a permanent blindness in her heart."));
	sp->setAttackType(ATTACK_RANGE, 12);
	sp->addEffect(APPLY_BLINDING, 10);
	sp->usesDivinePower = true;
	sp->setVigourCost(2);
	return sp;
}




//Monster abilities


spellSharedPtr ability_Charge()
{
	spellSharedPtr sp(new spell("Charge", SPELL_TILE, TCODColor::white));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(GAIN_FREE_MOVES, 3);
	sp->usesSpellPower = false;
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_Burst()
{
	spellSharedPtr sp(new spell("Burst", SPELL_TILE, TCODColor::darkLime));
	sp->setAttackType(ATTACK_AOE);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->setAttackRange(2);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_CrowArrow()
{
	spellSharedPtr sp(new spell("Crow Arrow", SPELL_TILE, TCODColor::lightGrey));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(8);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 10);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_FeedingFrenzy()
{
	spellSharedPtr sp(new spell("Feeding Frenzy", SPELL_TILE, TCODColor::lime));
	sp->setAttackType(ATTACK_AOE);
	sp->setAttackRange(3);
	sp->addEffect(GAIN_FREE_MOVES, 1);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_DropOoze()
{
	spellSharedPtr sp(new spell("Drop Ooze", SPELL_TILE, TCODColor::sepia));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(DROP_OOZE, 1);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_ShadowWalk()
{
	spellSharedPtr sp(new spell("Shadow Walk", SPELL_TILE, TCODColor::purple));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(BECOME_INVISIBLE, 2);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_WinterBlast()
{
	spellSharedPtr sp(new spell("Winter Blast", SPELL_TILE, TCODColor::cyan));
	sp->setAttackType(ATTACK_AOE, 3);
	sp->addEffect(APPLY_COLD_DAMAGE, 25);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_WreatheInFlame()
{
	spellSharedPtr sp(new spell("Wreathe In Flame", SPELL_TILE, TCODColor::flame));
	sp->setAttackType(ATTACK_AOE, 3);
	sp->addEffect(APPLY_FIRE_DAMAGE, 25);
	return sp;
}

spellSharedPtr ability_DragBelow()
{
	spellSharedPtr sp(new spell("Drag Below", SPELL_TILE, TCODColor::sea));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(APPLY_ENTANGLING, 4);
	return sp;
}

spellSharedPtr ability_SpitWeb()
{
	spellSharedPtr sp(new spell("Spit Web", SPELL_TILE, TCODColor::white));
	sp->setAttackType(ATTACK_RANGE, 8);
	sp->addEffect(APPLY_ENTANGLING, 5);
	return sp;
}

spellSharedPtr attack_RavenousHunger()
{
	spellSharedPtr sp(new spell("Ravenous Hunger", SPELL_TILE, TCODColor::red,
		"Attack heals you and deals bleed damage to target."));
	sp->setAttackType(ATTACK_RANGE, 3);
	sp->addEffect(APPLY_BLEED_DAMAGE, 25);
	sp->addEffect(HEAL_CASTER, 25);
	return sp;
}

spellSharedPtr ability_ShredSkin()
{
	spellSharedPtr sp(new spell("Shred Skin", SPELL_TILE, TCODColor::red));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(HURT_CASTER, 15);
	sp->addEffect(APPLY_DAMAGE_PENALTY, -25);
	return sp;
}

spellSharedPtr ability_Gnash()
{
	spellSharedPtr sp(new spell("Gnash", SPELL_TILE, TCODColor::lightestRed));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(HEAL_CASTER, 200);
	sp->addEffect(APPLY_ENTANGLING, 1);
	sp->addEffect(APPLY_BLEED_DAMAGE, 10);
	return sp;
}

spellSharedPtr ability_DevouringGaze()
{
	spellSharedPtr sp(new spell("Devouring Gaze", SPELL_TILE, TCODColor::purple));
	sp->setAttackType(ATTACK_RANGE, 1);
	sp->addEffect(APPLY_BLINDING, 1);
	sp->addEffect(APPLY_POISON_DAMAGE, 10);
	return sp;
}

spellSharedPtr ability_Deathwatch()
{
	spellSharedPtr sp(new spell("Death Stink", SPELL_TILE, TCODColor::green));
	sp->setAttackType(ATTACK_RANGE, 20);
	sp->addEffect(APPLY_UNTYPED_DAMAGE, 50);
	sp->isStenchBased = true;
	return sp;
}

spellSharedPtr ability_TentacleSwarm()
{
	spellSharedPtr sp(new spell("Tentacle Swarm", SPELL_TILE, TCODColor::green,
		"Entangles target, preventing them from moving."));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(3);
	sp->addEffect(APPLY_ENTANGLING, 3);
	sp->setVigourCost(2);
	return sp;
}

spellSharedPtr ability_Throw()
{
	spellSharedPtr sp(new spell("Throw", SPELL_TILE, TCODColor::green));
	sp->setAttackType(ATTACK_RANGE);
	sp->setAttackRange(1);
	sp->addEffect(KNOCKBACK_TARGET, 5);
	sp->addEffect(APPLY_PHYSICAL_DAMAGE, 25);
	sp->useAlternateAnimation = true;
	return sp;
}

spellSharedPtr ability_VoidStep()
{
	spellSharedPtr sp(new spell("Void Step", SPELL_TILE, TCODColor::purple));
	sp->setAttackType(ATTACK_BUFF_SELF);
	sp->addEffect(TELEPORT, 5);
	return sp;
}

spellSharedPtr ability_WarpingJavelin()
{
	spellSharedPtr sp(new spell("Warping Javelin", SPELL_TILE, TCODColor::lighterBlue));
	sp->setAttackType(ATTACK_RANGE, 8);
	sp->addEffect(APPLY_PROFANE_DAMAGE, 100);
	sp->alwaysHitTarget = true;
	return sp;
}
