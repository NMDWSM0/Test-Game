#pragma once
#define JUMPBLACK_H

#include "monster.h"

class JumpBlack : public Monster
{
public:
	enum FUNCNAME {
		DEATH,
		REMOVE,
		ONCOLLIDE,

		//以上来源于怪物基类，必须复制
	};

	float speed = 4.0f;

	float jumpspeed = 12.0f;

public:
	JumpBlack();

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	void OnCollide(DATA data);
};