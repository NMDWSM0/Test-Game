#pragma once
//#ifndef SAMLLDOT_H
#define SAMLLDOT_H

#include "monster.h"

class SmallDot : public Monster
{
public:
	enum FUNCNAME {
		DEATH,

		//������Դ�ڹ�����࣬���븴��
	};

public:
	SmallDot();

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/
};

/**********************************************************************/

SmallDot::SmallDot() :
	Monster("assets/prefabs/smalldot.txt", "assets/images/smalldot.png")
{

	AddTag("smalldot");
}

/*************************************************/
//���ֻ��ຯ��ID��ԭ�����������ⷢ���������û���
void SmallDot::EventTask(unsigned int id, DATA data)
{
	Monster::EventTask(id, data);
	switch (id)
	{
	
	}
}

void SmallDot::TimeTask(unsigned int id)
{
	Monster::TimeTask(id);
	switch (id)
	{

	}
}

void SmallDot::PeriodicTask(unsigned int id, float time)
{
	Monster::PeriodicTask(id, time);
	switch (id)
	{

	}
}

/*************************************************/

//#endif // !SAMLLDOT_H
