#pragma once
#ifndef JUMPBLAKBRAIN_H
#define JUMPBLAKBRAIN_H

#include "brain.h"
#include "jumpblack.h"

class JumpBlackBrain :public Brain
{
	JumpBlack* inst;
public:
	JumpBlackBrain(JumpBlack* inst) :
		Brain(inst), inst(inst)
	{

	}

	void UpdateActions();

	//判断函数, 优先级由高到低（或可以同时进行）
	bool JumpableAtFront()
	{
		glm::vec2 front = inst->IsRight() ? glm::vec2(1.0f, 0.0f) : glm::vec2(-1.0f, 0.0f);
		auto blocks = FindEntities(inst->position.x, inst->position.y, inst->position.z, 2.0f, { "blockers" });
		for (auto b : blocks)
		{
			if (!inst->CanCollide(*b))
				continue;

			glm::vec2 tob = glm::vec2(b->position - inst->position);
			float deltax = glm::dot(tob, front);
			if (deltax > 0.0f && deltax < 0.3f && fabs(b->position.y - inst->position.y) < 0.5f)
				return false;
		}
		return true;
	}

	//执行函数
	void GoFront()
	{
		inst->GetPhysics()->SetHorizontalV(inst->IsRight() ? inst->speed : -inst->speed);
		if (inst->OnGround())
			inst->GetPhysics()->SetVerticalV(inst->jumpspeed);
	}

	void TurnBack()
	{
		inst->SetRight(!inst->IsRight());
		inst->GetPhysics()->SetHorizontalV(inst->IsRight() ? inst->speed : -inst->speed);
	}
};

void JumpBlackBrain::UpdateActions()
{
	if (!enable)
		return;

	if (JumpableAtFront())
	{
		GoFront();
	}
	else
		TurnBack();
}

#endif // !JUMPBLAKBRAIN_H