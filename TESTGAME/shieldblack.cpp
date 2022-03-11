#include "shieldblack.h"
#include "shieldblackbrain.h"

bool CanBeAttacked(Entity* attacker, Entity* target, DATA data)
{
	if (!target->HasTag("shieldbalck"))
		return true;

	ShieldBlack* inst = (ShieldBlack*)target;
	bool isarrowattack = std::any_cast<bool>(data[0]);
	if (isarrowattack)
	{
		glm::vec3 arrowpos = std::any_cast<glm::vec3>(data[1]);
		glm::vec3 to = arrowpos - inst->position;
		glm::vec3 front = inst->IsRight() ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(-1.0f, 0.0f, 0.0f);
		if (glm::dot(to, front) > 0.0f)
			return false;
	}
	return true;
}

ShieldBlack::ShieldBlack() :
	Monster("assets/prefabs/shieldblack.txt", "assets/images/shieldblack.png")
{
	brain = new ShieldBlackBrain(this);
	SetScale(1.4f);
	combat->SetCanBeAttacked(&CanBeAttacked);

	AddTag("shieldbalck");
}

/*************************************************/
//保持基类函数ID的原因就在这里，避免发生函数调用混乱
void ShieldBlack::EventTask(unsigned int id, DATA data)
{
	Monster::EventTask(id, data);
	switch (id)
	{

	}
}

void ShieldBlack::TimeTask(unsigned int id)
{
	Monster::TimeTask(id);
	switch (id)
	{

	}
}

void ShieldBlack::PeriodicTask(unsigned int id, float time)
{
	Monster::PeriodicTask(id, time);
	switch (id)
	{

	}
}

/**************************************************************/

void ShieldBlack::OnCollide(DATA data)
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