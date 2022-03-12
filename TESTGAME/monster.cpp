#include "monster.h"
#include "brain.h"

Monster::Monster(const std::string& filename, const std::string& texturename) :
	Entity(filename, texturename), brain(nullptr)
{
	//以下参数可能不同怪物不同，这里只给出默认设置
	SetGravityType(GRAVITYTYPE::GRAVITATIONAL);
	SetCollisionGroup(COLLISIONGROUP::ENEMIES);
	SetBounding(glm::vec3(0.9f, 0.9f, 0.9f));
	SetScale(1.05f, 1.05f, 1.05f);

	right = (rand() % 2);

	//组件
	health = new Health(this, 10.0f);
	combat = new Combat(this, 10.0f);

    ListenForEvent("death", DEATH);
	ListenForEvent("collide", ONCOLLIDE);

	AddTag("monster");
}

/****************************************************************/

void Monster::EventTask(unsigned int id, DATA data)
{
	switch (id)
	{
	case DEATH:
		Death(data);
		break;
	case ONCOLLIDE:
		OnCollide(data);
		break;
	}
}

void Monster::TimeTask(unsigned int id)
{
	switch (id)
	{
	case REMOVE:
		Remove();
		break;
	}
}

void Monster::PeriodicTask(unsigned int id, float time)
{
	switch (id)
	{
	
	}
}

/*************************************************************/

COLLIDEDIRECTION Monster::CollideWith(const Entity& ent) const
{
	if (physics == nullptr || ent.GetPhysics() == nullptr || &ent == this)
		return COLLIDEDIRECTION::NONE;
	float deltax = fabs(position.x - ent.position.x);
	float deltay = fabs(position.y - ent.position.y);

	float boundx = (ent.GetPhysics()->boundingbox.x + physics->boundingbox.x) / 2.0f;
	float boundy = (ent.GetPhysics()->boundingbox.y + physics->boundingbox.y) / 2.0f;

	// && (boundx - deltax > 0.1f && boundy - deltay > 0.0f || boundy - deltay > 0.1f && boundx - deltax > 0.0f)

	bool c_x = false, c_y = false;

	//玩家的碰撞设置在比较远的位置就要求停止了，因此怪的碰撞要设置的更加夸张一点才行
	//相应的，把怪的原始碰撞调小而贴图调大，使之看起来合理
	if (ent.GetPhysics()->collisiongroup == COLLISIONGROUP::PLAYER)
	{
		if (boundx - deltax > -0.03f && boundy - deltay > 0.0f && CanCollide(ent, COLLIDEDIRECTION::VERTICAL))
			c_y = true;
		if (boundy - deltay > -0.03f && boundx - deltax > 0.0f && CanCollide(ent, COLLIDEDIRECTION::HORIZONTAL))
			c_x = true;
	}

	if (boundx - deltax > 0.1f && boundy - deltay > 0.0f && CanCollide(ent, COLLIDEDIRECTION::VERTICAL))
		c_y = true;
	if (boundy - deltay > 0.1f && boundx - deltax > 0.0f && CanCollide(ent, COLLIDEDIRECTION::HORIZONTAL))
		c_x = true;

	if (!c_x && !c_y)
		return COLLIDEDIRECTION::NONE;
	else if (c_x && !c_y)
		return COLLIDEDIRECTION::HORIZONTAL;
	else if (!c_x && c_y)
		return COLLIDEDIRECTION::VERTICAL;
	else
		return COLLIDEDIRECTION::ALLDIR;
}

void Monster::UpdatePosition(float deltaT)
{
	if (physics == nullptr)
		return;
	glm::vec3 oripos = position;
	glm::vec3 targetpos = position + glm::vec3(deltaT * physics->GetVelocity().x, deltaT * physics->GetVelocity().y, 0.0f);
	SetPosition(targetpos);

	std::vector<COLLIDEDIRECTION> dir;
	std::vector<Entity*> collideobj = CollidedObject(*this, dir);

	if (collideobj.size() > 0)
	{
		bool hasc_x = false, hasc_y = false;
		for (auto t : dir)
		{
			if (t == COLLIDEDIRECTION::HORIZONTAL)
			{
				physics->SetHorizontalV(0.0f);
				hasc_x = true;
			}
			if (t == COLLIDEDIRECTION::VERTICAL)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_y = true;
			}
			if (t == COLLIDEDIRECTION::ALLDIR)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_x = true;
				hasc_y = true;
			}
		}
		if (hasc_x && !hasc_y)
			SetPosition(oripos.x, targetpos.y, oripos.z);
		else if (hasc_y && !hasc_x)
			SetPosition(targetpos.x, oripos.y, oripos.z);
		else
			SetPosition(oripos);

		PushEvent("collide", DATA{ collideobj, dir });
	}

	if (physics->gravtype == GRAVITYTYPE::GRAVITATIONAL && OnGround())
	{
		physics->onground = true;
	}
	else
		physics->onground = false;

	physics->UpdateVelocity(deltaT);
}

void Monster::UpdateScaling(float deltaT)
{
	if (!right)
		SetScale(-1.0f, 1.0f, 1.0f);
	else
		SetScale(1.0f, 1.0f, 1.0f);
}

void Monster::Update(float deltaT)
{
	TimeTick(deltaT);
	
	if (HasPhysics())
	{
		UpdatePosition(deltaT);
		UpdateRotation(deltaT);
		UpdateScaling(deltaT);
	}

	if (brain != nullptr)
		brain->UpdateActions();

	if (!health->IsDead())
		SendEventToUnder({ "blockers", "disaprblock" }, "collidewitht");
}

/*************************************************************/

// data: attacker(afflicter)
void Monster::Death(DATA data)
{
	if (brain != nullptr)
		brain->Stop();

	SetCollisionGroup(COLLISIONGROUP::NOTHING);
	AddTag("FX");
	SetGravityType(GRAVITYTYPE::GRAVITATIONAL);

	SetPosition(position.x, position.y, 0.05f);

	if (data.size() >= 0)
	{
		Entity* attacker = std::any_cast<Entity*>(data[0]);
		if (attacker && attacker->HasPhysics() && attacker->position.x > position.x)
		{
			right = true;
			SetScale(1.0f, 1.0f, 1.0f);
		}
		else
		{
			right = false;
			SetScale(-1.0f, 1.0f, 1.0f);
		}
	}

	float theta = random() * PI * 0.5f;
	if (right)
		theta += PI * 0.5f;
	physics->GiveVelocity(4.0f * glm::vec2(cosf(theta), sinf(theta)));

	DoTaskInTime(2.0f, REMOVE);
}

//data: 第一个参数是指向碰撞者的指针
void Monster::OnCollide(DATA data)
{
	if (data.size() == 0)
		return;

	//在这里只考虑与玩家碰撞，被打的情况不需要在这里考虑捏
	auto colliders = std::any_cast<std::vector<Entity*>>(data[0]);
	for (auto collider : colliders)
	{
		if (collider && collider->HasTag("player"))
		{
			Player* player = (Player*)collider;
			combat->DoAttack(player, 1.01f);
		}
	}
}