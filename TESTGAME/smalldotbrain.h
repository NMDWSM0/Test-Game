#pragma once
#ifndef SMALLDOTBRAIN_H
#define SMALLDOTBRAIN_H

#include "brain.h"
#include "smalldot.h"

class SmallDotBrain :public Brain
{
	SmallDot* inst;
public:
	SmallDotBrain(SmallDot* inst) :
		Brain(inst), inst(inst)
	{
		
	}

	void UpdateActions();

	//判断函数, 优先级由高到低（或可以同时进行）
	bool WalkableAtFront()
	{
		glm::vec2 front = inst->IsRight() ? glm::vec2(1.0f, 0.0f) : glm::vec2(-1.0f, 0.0f);
		auto blocks = FindEntities(inst->position.x, inst->position.y, inst->position.z, 2.0f, { "blockers" });
		bool hasfloor = false;
		for (auto b : blocks)
		{
			glm::vec2 tob = glm::vec2(b->position - inst->position);
			float deltax = glm::dot(tob, front);
			if (b->position.y + b->GetPhysics()->boundingbox.y * 0.5f < inst->position.y && deltax > 0.0f)
				hasfloor = true;

			if (deltax > 0.0f && deltax < 0.3f && fabs(b->position.y - inst->position.y) < 0.5f)
				return false;
		}
		if (hasfloor)
			return true;
		return false;
	}

	//执行函数
	void GoFront()
	{
		inst->GetPhysics()->SetHorizontalV(inst->IsRight() ? inst->speed : -inst->speed);
	}

	void TurnBack()
	{
		inst->SetRight(!inst->IsRight());
		inst->GetPhysics()->SetHorizontalV(inst->IsRight() ? inst->speed : -inst->speed);
	}
};

void SmallDotBrain::UpdateActions()
{
	if (!enable)
		return;

	if (WalkableAtFront())
	{
		GoFront();
	}
	else
		TurnBack();
}

#endif // !SMALLDOTBRAIN_H
