
#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "counter.h"
#include "spell.h"

class consumable : public item
{
public:

	//Constructors
	consumable() : item() {}
	consumable(std::string name, int tileCode, TCODColor color, std::string description);
	~consumable();

	//Quantity adjustments
	virtual const bool isStackable() { return true; }
	virtual const void add(int n = 1) { amount.increaseMaxValue(n); }
	void lose(int n = 1) { amount.decrease(n); }
	virtual int getAmountLeft() { return amount.getValue(); }
	virtual int getMaxAmount() { return amount.getMaxValue(); }
	virtual void const restoreToMax() { amount.restore(); }
	virtual const void setAmount(int amount) { this->amount.setTo(amount); }

	//Flava
	virtual std::string getMenuName();

	//Effects
	bool isRangedAttackItem() { return rangedAttack != nullptr; }
	effectVector getEffects() { return effectsApplied; }
	std::vector<int> getPotencies() { return effectPotencies; }
	spellSharedPtr getRangedAttack() { return rangedAttack; }
	weaponBuffSharedPtr getWeaponBuff() { return buff; }

	//Setting effects
	void addEffect(effect eff, int pot = 0) { effectsApplied.push_back(eff); effectPotencies.push_back(pot); }
	void setRangedAttack(spellSharedPtr at) { rangedAttack = at; }
	void setWeaponBuff(weaponBuff* b) { buff = weaponBuffSharedPtr(b); }

	//Flags
	bool consumeOnUse = true; //Doesn't have to get USED UP!
	bool oneUseOnly = false; //If true, we lose it PERMANENTLY when it's used
	bool addsPermanentBuff = false;

private:

	//Quantity!
	counter amount;

	//Effects and potency
	effectVector effectsApplied;
	std::vector<int> effectPotencies;

	//Ranged attack is treated as a spell, for ranged-attack consumables
	spellSharedPtr rangedAttack;

	//Weapon buff, for consumables that do that
	weaponBuffSharedPtr buff;

};


typedef std::shared_ptr<consumable> consumableSharedPtr;
typedef std::vector<consumableSharedPtr> consumableVector;

/*
	Consumable objects
*/

consumableSharedPtr consumable_StarwaterDraught();
consumableSharedPtr consumable_InvigoratingTea();
consumableSharedPtr consumable_PalegrassHerb();
consumableSharedPtr consumable_PilgrimsFingerbone();

consumableSharedPtr consumable_TinyRedFlower();
consumableSharedPtr consumable_TinyGreenFlower();
consumableSharedPtr consumable_PutridFlower();
consumableSharedPtr consumable_BloodDrinkersEyes();
consumableSharedPtr consumable_IntoxicatingWine();
consumableSharedPtr consumable_WitchwaterFlask();
consumableSharedPtr consumable_BlackHoney();

consumableSharedPtr consumable_Quickgrass();
consumableSharedPtr consumable_AcidBloodFlask();
consumableSharedPtr consumable_WitchsRoot();

consumableSharedPtr consumable_PutridBrew();
consumableSharedPtr consumable_GodsbloodBrew();
consumableSharedPtr consumable_BlackTarLiquor();
consumableSharedPtr consumable_PurifiedStarwater();

consumableSharedPtr ranged_ThrowingKnives();
consumableSharedPtr ranged_HeavyJavelin();
consumableSharedPtr ranged_LaceratingKnives();
consumableSharedPtr ranged_PoisonThrowingKnives();
consumableSharedPtr ranged_CorrodingJar();
consumableSharedPtr ranged_VoidEssenceJar();
consumableSharedPtr ranged_PyromancersFlask();
consumableSharedPtr ranged_WitchsJar();
consumableSharedPtr ranged_LightingJavelin();
consumableSharedPtr ranged_FrostKnives();
consumableSharedPtr ranged_WarpingJavelin();

consumableSharedPtr oil_PyromancersOil();
consumableSharedPtr oil_CorrosiveOil();
consumableSharedPtr oil_FrozenOil();
consumableSharedPtr oil_HolyWater();
consumableSharedPtr oil_CursedWater();
consumableSharedPtr oil_HangmansBlood();
consumableSharedPtr oil_WitchsOoze();
consumableSharedPtr oil_LightningOil();
consumableSharedPtr oil_DeepRedOil();
consumableSharedPtr oil_RotbloodOil();

consumableSharedPtr gem_DullGreyGemstone();
consumableSharedPtr gem_SmallShiningGemstone();
consumableSharedPtr gem_NobleGlowingGemstone();
consumableSharedPtr gem_KingsGiantGemstone();
consumableSharedPtr gem_GodlessGemstone();

consumableSharedPtr bell_VoidwalkersDancingBell();
consumableSharedPtr bell_VoidwalkersReturningBell();
consumableSharedPtr horn_HeraldsWhiteHorn();
consumableSharedPtr misc_BrandOfTheEmissary();

#endif
