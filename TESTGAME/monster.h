#pragma once
#ifndef MONSTER_H
#define SAMLLDOT_H

#include "entity.h"
#include "combat.h"
#include "health.h"
#include "externfuncs.h"

class Brain;

class Monster :public Entity
{
protected:
	bool right = true;

	Brain* brain;

public:
	enum FUNCNAME {
		DEATH,
		REMOVE,
		ONCOLLIDE,
	};

public:
	Monster(const std::string& filename, const std::string& texturename = "");

	bool IsRight() const
	{
		return right;
	}

	void SetRight(bool r)
	{
		right = r;
	}

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	COLLIDEDIRECTION CollideWith(const Entity& ent) const;

	void UpdatePosition(float deltaT);

	void UpdateScaling(float deltaT);

	void Update(float deltaT);

	/**************************************/

	virtual void Death(DATA data = DATA{});

	virtual void OnCollide(DATA data);
};

#endif // !MONSTER_H
