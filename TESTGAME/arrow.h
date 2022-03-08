#pragma once
#ifndef ARROW_H
#define ARROW_H

#include "entity.h"

class Arrow : public Entity
{
	enum FUNCNAME
	{
		REMOVE = 0,
		REMOVE_IN_5_SEC = 1,
		TWINT = 2,
		SRART_TWINT_IN_3_5_SEC = 3,
		STARTTWINT = 4,

		RMV_LISTENER_Twint_Remove = 99,
	};

	bool lighted = true;

	void Twint() 
	{
		lighted = !lighted;
	}

protected:
	void TimeTask(unsigned int id);

	void EventTask(unsigned int id);

	void PeriodicTask(unsigned int id);

public:
	Arrow() :
		Entity("assets/prefabs/arrow.txt", "assets/images/arrow.png")
	{
		SetCollisionGroup(Physics::NOTHING);
		SetBounding(glm::vec3(1.0f, 0.25f, 0.0f));

		ListenForEvent("collide", REMOVE_IN_5_SEC, "Remove");
		ListenForEvent("collide", SRART_TWINT_IN_3_5_SEC, "Twint");
	}

	void Draw(Shader& shader);
};

void Arrow::EventTask(unsigned int id)
{
	switch (id)
	{
	case REMOVE_IN_5_SEC:
		DoTaskInTime(5, REMOVE);
		DoTaskInTime(0.01f, RMV_LISTENER_Twint_Remove);
		break;
	case SRART_TWINT_IN_3_5_SEC:
		DoTaskInTime(3.5, STARTTWINT);
		break;
	}
}

void Arrow::TimeTask(unsigned int id)
{
	switch (id)
	{
	case REMOVE:
		Remove();
		break;
	case STARTTWINT:
		DoPeridiocTask(0.1f, TWINT, 0.9f);
		break;
	case RMV_LISTENER_Twint_Remove:
		RemoveEventListener("collide", "Remove");
		RemoveEventListener("collide", "Twint");
		break;
	}
}

void Arrow::PeriodicTask(unsigned int id)
{
	switch (id)
	{
	case TWINT:
		Twint();
		break;
	}
}

void Arrow::Draw(Shader& shader)
{
	if (lighted)
		Entity::Draw(shader);
}

#endif // !ARROW_H
