#pragma once
#ifndef ARROW_H
#define ARROW_H

#include "entity.h"
#include "player.h"

class Arrow : public Entity
{
	Player* attacker;

	enum FUNCNAME
	{
		REMOVE = 0,
		ONCOLLIDE = 1,
		TWINT = 2,
		UPDATEROTATION = 3,
		STARTTWINT = 4,
		COLLIDEWITHPLAYER = 5,

		RMV_LISTENER_Twint_Remove = 99,
	};

	bool lighted = true;

	bool collidewithplayer = false;

	void Twint() 
	{
		lighted = !lighted;
	}

protected:
	/*************************************/
	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);
	/*************************************/
	void UpdateRotation();

	COLLIDEDIRECTION CollideWith(const Entity& ent) const;
	/*************************************/
	void OnCollide(DATA data);

	void CollideWithPlayer();

public:
	Arrow();

	void SetAttacker(Player* player)
	{
		attacker = player;
	}

	/*******************************************/
	void Draw(Shader& shader);
};

#endif // !ARROW_H
