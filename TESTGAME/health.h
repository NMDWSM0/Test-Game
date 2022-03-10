#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include "component.h"

class Health :public Component
{
	float current_health;
	float max_health;
	bool invincible = false;

public:
	Health(Entity* const inst, float max);

	void SetMaxHealth(float max, bool withpercent = true);

	float GetMaxHealth()
	{
		return max_health;
	}

	void SetCurrentHealth(float current, DATA data = DATA{});

	float GetCurrenthealth()
	{
		return current_health;
	}

	float GetPercent()
	{
		return current_health / max_health;
	}

	float DoDelta(float delta, DATA data = DATA{});

	bool IsDead()
	{
		return current_health <= 0.0f;
	}

	void SetInvincible(bool inv = true);

	bool IsInvincible()
	{
		return invincible;
	}
};

/**********************************************************/

Health::Health(Entity* const inst, float max):
	Component(inst), max_health(max)
{
	assert(inst != nullptr);
	current_health = max_health;
	inst->AddTag("_health");
}

void Health::SetMaxHealth(float max, bool withpercent)
{
	if (IsDead())
		return;
	float per = 1.0f;
	if (withpercent)
		per = GetPercent();
	max_health = max;
	inst->PushEvent("maxhealthdelta", DATA{ max_health, current_health }); // max, current
	SetCurrentHealth(per * max);
}

// data: attacker(afflicter)
void Health::SetCurrentHealth(float current, DATA data)
{
	if (IsDead())
		return;
	if (current > max_health)
		current = max_health;
	float ori_health = current_health;
	current_health = current;
	inst->PushEvent("healthdelta", DATA{ current_health - ori_health, current_health }); //delta, current
	if (IsDead())
	{
		inst->PushEvent("death", data); // data: attacker(afflicter)
	}
}

// data: attacker(afflicter)
float Health::DoDelta(float delta, DATA data)
{
	if (IsDead())
		return 0.0f;
	if (IsInvincible() && delta < 0.0f)
		return 0.0f;

	float ori_health = current_health;
	SetCurrentHealth(ori_health + delta, data);
	float real_delta = current_health - ori_health;
	return real_delta;
}

void Health::SetInvincible(bool inv)
{
	if (IsDead())
		return;
	invincible = inv;
}

#endif // !HEALTH_H
