#pragma once
#ifndef ABILITYCD_H
#define ABILITYCD_H

#include "sourcemodifierlist.h"

class Ability_CD
{
	const float cd = 1.0f ;
	float cur_t = 0.0f;

	SourceModifierList cd_modifier;

public:
	Ability_CD(float cd):
		cd(cd), cd_modifier(SourceModifierList(1.0f, MODITYPE::MULTIPLY))
	{

	}

	//每帧都刷新的
	void Update(float deltaT)
	{
		cur_t += deltaT * cd_modifier.CalcModifiers();
	}

	//手动调用的，不受冷却回复速度的影响
	void Timedelta(float deltaT)
	{
		cur_t += deltaT;
	}

	bool CanCast()
	{
		if (cur_t >= cd)
		{
			cur_t = 0.0f;
			return true;
		}
		return false;
	}

	void AddCDModifier(std::string source, float value, std::string key = "")
	{
		cd_modifier.AddModifier(source, value, key);
	}

	void RemoveCDModifier(std::string source, std::string key = "")
	{
		cd_modifier.RemoveModifier(source, key);
	}

};

#endif // !ABILITYCD_H