#pragma once
#ifndef COMBAT_H
#define COMBAT_H

#include "component.h"
#include "player.h"
#include "monster.h"

typedef bool (*CanAttackFunc)(Entity*, Entity*, DATA);

class Combat :public Component
{
	float atk;
	SourceModifierList atk_modifiers;

	CanAttackFunc canattackfunc;

	CanAttackFunc canbeattackedfunc;

	//float def;
	//float res;
	//float bonus;
public:
	Combat(Entity* const inst, float attack); 

	void SetCanAttack(CanAttackFunc func);

	void SetCanBeAttacked(CanAttackFunc func);

	bool DoAttack(Entity* target, float instancemult = 1.0f, DATA data = {});

	float CalcDamage(Entity* target, float instancemult);

	bool GetAttacked(Entity* attacker, float dmg);

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
	Component(inst), atk(attack), canattackfunc(nullptr), canbeattackedfunc(nullptr)
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

void Combat::SetCanAttack(CanAttackFunc func)
{
	canattackfunc = func;
}

void Combat::SetCanBeAttacked(CanAttackFunc func)
{
	canbeattackedfunc = func;
}

//data: bool isarrowattack, vec3 arrowpos
bool Combat::DoAttack(Entity* target, float instancemult, DATA data)
{
	if (target->combat == nullptr)
		return false;
	if (target->health == nullptr || target->health->IsDead())
		return false;
	
	// attacker, target, data
	if (canattackfunc != nullptr && !(*canattackfunc)(inst, target, data))
		return false;
	// attacker, target, data
	if (target->combat->canbeattackedfunc != nullptr && !(*target->combat->canbeattackedfunc)(inst, target, data))
		return false;

	float damage = CalcDamage(target, instancemult);
	
	return target->combat->GetAttacked(inst, damage);
}

float Combat::CalcDamage(Entity* target, float instancemult)
{
	float base = GetATK();

	float damage = base * instancemult;
	return damage;
}

bool Combat::GetAttacked(Entity* attacker, float dmg)
{
	if (inst->health == nullptr || inst->health->IsDead())
		return false;

	bool blocked = false;
	float damageresolved = 0.0f;
	if (inst->health->IsInvincible())
		blocked = true;
	else
	{
		damageresolved = -inst->health->DoDelta(-dmg, DATA{ attacker });
	}

	if (!blocked)
	{
		attacker->PushEvent("hitother", DATA{ inst, dmg, damageresolved });  //target, damage, damageresolved
		inst->PushEvent("attacked", DATA{ attacker, dmg, damageresolved });  //attacker, damage, damageresolved
		return true;
	}
	else
	{
		attacker->PushEvent("misstarget", DATA{ inst, dmg });
		return false;
	}
}

#endif // !COMBAT_H
