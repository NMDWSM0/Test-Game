#include "flyear.h"
#include "flyearbrain.h"

FlyEar::FlyEar() :
	Monster("assets/prefabs/flyear.txt", "assets/images/flyear.png")
{
	brain = new FlyEarBrain(this);
	SetGravityType(GRAVITYTYPE::ANTIGRAVITY);
	SetBounding(glm::vec3(1.0f, 1.0f, 1.0f));

	AddTag("flyear");
}

/*************************************************/
//���ֻ��ຯ��ID��ԭ�����������ⷢ���������û���
void FlyEar::EventTask(unsigned int id, DATA data)
{
	Monster::EventTask(id, data);
	switch (id)
	{

	}
}

void FlyEar::TimeTask(unsigned int id)
{
	Monster::TimeTask(id);
	switch (id)
	{

	}
}

void FlyEar::PeriodicTask(unsigned int id, float time)
{
	Monster::PeriodicTask(id, time);
	switch (id)
	{

	}
}

/**************************************************************/

void FlyEar::OnCollide(DATA data)
{
	Monster::OnCollide(data);

	if (data.size() == 0)
		return;

	bool shouldturn = false;
	auto colliders = std::any_cast<std::vector<Entity*>>(data[0]);
	auto dirs = std::any_cast<std::vector<COLLIDEDIRECTION>>(data[1]);

	auto col = colliders.begin();
	auto dir = dirs.begin();
	for (; col != colliders.end() && dir != dirs.end(); ++col, ++dir)
	{
		if ((*col) && (*col)->HasTag("blockers") && (*dir) == COLLIDEDIRECTION::HORIZONTAL)
		{
			shouldturn = true;
		}
	}

	if (shouldturn)
	{
		SetRight(!IsRight());
		GetPhysics()->SetHorizontalV(IsRight() ? speed : -speed);
	}
}