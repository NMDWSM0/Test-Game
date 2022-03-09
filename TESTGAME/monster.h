#pragma once
#ifndef MONSTER_H
#define SAMLLDOT_H

#include "entity.h"
#include "combat.h"
#include "health.h"
#include "externfuncs.h"

class Monster :public Entity
{
public:
	enum FUNCNAME {
		DEATH,
	};

public:
	Monster(const std::string& filename, const std::string& texturename = "");

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	void Death();
};

#endif // !MONSTER_H
