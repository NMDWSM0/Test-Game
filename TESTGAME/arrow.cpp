#include "arrow.h"

Arrow::Arrow() :
	Entity("assets/prefabs/arrow.txt", "assets/images/arrow.png")
{
	SetCollisionGroup(COLLISIONGROUP::ARROW);
	SetGravityType(GRAVITYTYPE::QUAD_GRAV);
	SetBounding(glm::vec3(1.4f, 0.25f, 0.0f));

	ListenForEvent("collide", ONCOLLIDE, "Remove");
	DoPeriodicTask(0.01f, UPDATEROTATION, INFINITY, "flywithrot");
	DoTaskInTime(1.0f, COLLIDEWITHPLAYER);

	AddTag("arrow");
}

void Arrow::EventTask(unsigned int id, DATA data)
{
	switch (id)
	{
	case ONCOLLIDE:
		OnCollide(data);
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
		DoPeriodicTask(0.1f, TWINT, 0.9f);
		break;
	case RMV_LISTENER_Twint_Remove:
		RemoveEventListener("collide", "Remove");
		RemoveEventListener("collide", "Twint");
		break;
	case COLLIDEWITHPLAYER:
		CollideWithPlayer();
		break;
	}
}

void Arrow::PeriodicTask(unsigned int id, float time)
{
	switch (id)
	{
	case TWINT:
		Twint();
		break;
	case UPDATEROTATION:
		UpdateRotation();
		break;
	}
}

COLLIDEDIRECTION Arrow::CollideWith(const Entity& ent) const
{
	if (ent.HasTag("player") && !collidewithplayer)
		return COLLIDEDIRECTION::NONE;
	return Entity::CollideWith(ent);
}

/******************************************************************/

void Arrow::Draw(Shader& shader)
{
	if (lighted)
		Entity::Draw(shader);
}

//data: 第一个参数是指向碰撞者的指针
void Arrow::OnCollide(DATA data)
{
	if (data.size() == 0)
	{
		Remove();
		return;
	}

	Entity* collider = std::any_cast<Entity*>(data[0]);
	if (collider->HasTag("canonwall"))
	{
		DoTaskInTime(5, REMOVE);
		DoTaskInTime(3.5, STARTTWINT);
		DoTaskInTime(0.01f, RMV_LISTENER_Twint_Remove);
		physics->SetVerticalV(0.0f);
		SetGravityType(GRAVITYTYPE::ANTIGRAVITY);
		CancelPeriodicTask("flywithrot");
		SetRotation(0.0f);
		return;
	}

	if (collider->HasTag("player"))
	{
		Player* player = (Player*)collider;
		player->Stun();
		Remove();
		return;
	}

	if (collider->HasTag("monster"))
	{
		//
		if (attacker != nullptr && attacker->combat != nullptr)
			attacker->combat->DoAttack(collider, 1.5f);
		Remove();
		return;
	}

	Remove();
}

void Arrow::UpdateRotation()
{
	glm::vec2 vel = physics->GetVelocity();
	float angle = glm::atan(vel.y / vel.x);
	SetRotation(angle);
}

void Arrow::CollideWithPlayer()
{
	collidewithplayer = true;
}