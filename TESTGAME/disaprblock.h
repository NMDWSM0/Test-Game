#pragma once
#ifndef DISAPRBLOCK_H
#define DISAPRBLOCK_H

#include "entity.h"
#include "externfuncs.h"

class DisaprBlock : public Entity
{
	enum FUNCNAME
	{
		SHOW,
		HIDE,

		ONCOLLIDEWITHT,
	};

	float show_time = 1.5f;
	float hide_time = 2.5f;

	bool shown = true;

public:
	DisaprBlock() :
		Entity("assets/prefabs/disaprblock.txt", "assets/images/disaprblock.png")
	{
		AddTag("disappearable");
		AddTag("disaprblock");

		ListenForEvent("collidewitht", ONCOLLIDEWITHT);

		Show();
	}

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	void Draw(Shader& shader);

	/**************************************/

	void Show();

	void Hide();

	void OnCollideWithT();
};

/****************************************************************/

void DisaprBlock::EventTask(unsigned int id, DATA data)
{
	switch (id)
	{
	case ONCOLLIDEWITHT:
		OnCollideWithT();
	}
}

void DisaprBlock::TimeTask(unsigned int id)
{
	switch (id)
	{
	case SHOW:
		Show();
		break;
	case HIDE:
		Hide();
		break;
	}
}

void DisaprBlock::PeriodicTask(unsigned int id, float time)
{
	switch (id)
	{

	}
}

/***********************************************************************/

void DisaprBlock::Draw(Shader& shader)
{
	if (!shown)
		return;

	Entity::Draw(shader);
}

/****************************************************************/

void DisaprBlock::Show()
{
	shown = true;
	
	AddTag("blockers");
	AddTag("canonwall");

	SetCollisionGroup(COLLISIONGROUP::A_WORLD);
}

void DisaprBlock::Hide()
{
	shown = false;

	RemoveTag("blockers");
	RemoveTag("canonwall");

	SetCollisionGroup(COLLISIONGROUP::NOTHING);

	DoTaskInTime(hide_time, SHOW);
}

void DisaprBlock::OnCollideWithT()
{
	DoTaskInTime(show_time, HIDE);
}

#endif // !DISAPRBLOCK_H
