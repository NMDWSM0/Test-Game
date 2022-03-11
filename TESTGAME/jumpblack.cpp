#include "jumpblack.h"
#include "jumpblackbrain.h"

JumpBlack::JumpBlack() :
	Monster("assets/prefabs/jumpblack.txt", "assets/images/jumpblack.png")
{
	SetCollisionGroup(COLLISIONGROUP::THR_ENEMIES);

	brain = new JumpBlackBrain(this);
	AddTag("jumpblack");
}

/*************************************************/
//���ֻ��ຯ��ID��ԭ�����������ⷢ���������û���
void JumpBlack::EventTask(unsigned int id, DATA data)
{
	Monster::EventTask(id, data);
	switch (id)
	{

	}
}

void JumpBlack::TimeTask(unsigned int id)
{
	Monster::TimeTask(id);
	switch (id)
	{

	}
}

void JumpBlack::PeriodicTask(unsigned int id, float time)
{
	Monster::PeriodicTask(id, time);
	switch (id)
	{

	}
}

/**************************************************************/

void JumpBlack::OnCollide(DATA data)
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
		if ((*col) && !(*col)->HasTag("player") && (*dir) == COLLIDEDIRECTION::HORIZONTAL)
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