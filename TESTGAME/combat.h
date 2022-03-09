#pragma once
#ifndef COMBAT_H
#define COMBAT_H

#include "component.h"
#include "player.h"
#include "monster.h"

class Combat :public Component
{
	float atk;
	SourceModifierList atk_modifiers;

	//float def;
	//float res;
	//float bonus;
public:
	Combat(Entity* const inst, float attack);

	void DoAttack(Entity* target, float instancemult = 1.0f);

	float CalcDamage(Entity* target, float instancemult);

	void GetAttacked(Entity* attacker, float dmg);

	void SetBaseATK(float v);

	void AddATKModifiers(std::string source, float value, std::string key = "");

	void RemoveATKModifiers(std::string source, std::string key = "");

	float GetBaseATK()
	{
		return atk;
	}

	float GetATK()
	{
		return atk * atk_modifiers.CalcModifiers();
	}
};

/********************************************************/

Combat::Combat(Entity* const inst, float attack):
	Component(inst), atk(attack)
{
	assert(inst != nullptr);
	inst->AddTag("_combat");
}

void Combat::SetBaseATK(float v)
{
	atk = v;
}

void Combat::AddATKModifiers(std::string source, float value, std::string key)
{
	atk_modifiers.AddModifier(source, value, key);
}

void Combat::RemoveATKModifiers(std::string source, std::string key)
{
	atk_modifiers.RemoveModifier(source, key);
}

void Combat::DoAttack(Entity* target, float instancemult)
{
	if (target->combat == nullptr)
		return;
	if (target->health == nullptr || target->health->IsDead())
		return;

	float damage = CalcDamage(target, instancemult);

	target->combat->GetAttacked(inst, damage);
}

float Combat::CalcDamage(Entity* target, float instancemult)
{
	float base = GetATK();

	float damage = base * instancemult;
	return damage;
}

void Combat::GetAttacked(Entity* attacker, float dmg)
{
	if (inst->health == nullptr || inst->health->IsDead())
		return;

	bool blocked = false;
	float damageresolved = 0.0f;
	if (inst->health->IsInvincible())
		blocked = true;
	else
	{
		damageresolved = - inst->health->DoDelta(-dmg);
	}

	if (!blocked)
	{
		attacker->PushEvent("onhitother", DATA{ inst, dmg, damageresolved });  //target, damage, damageresolved
		inst->PushEvent("onattacked", DATA{ attacker, dmg, damageresolved });  //attacker, damage, damageresolved
	}
	else
	{
		attacker->PushEvent("misstarget", DATA{ inst, dmg });
	}
}

#endif // !COMBAT_H
