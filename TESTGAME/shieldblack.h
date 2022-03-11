#pragma once
#define SHIELDBLACK_H

#include "monster.h"

class ShieldBlack : public Monster
{
public:
	enum FUNCNAME {
		DEATH,
		REMOVE,
		ONCOLLIDE,

		//������Դ�ڹ�����࣬���븴��
	};

	float speed = 3.0f;

public:
	ShieldBlack();

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	void OnCollide(DATA data);
	
};