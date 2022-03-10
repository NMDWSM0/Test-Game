#include "player.h"
#include "arrow.h"

Player::Player(const std::string& filename, const std::string& texturename) :
	Entity(filename, texturename), jump_cd(0.1), atk_cd(0.65)
{
	position.z = 0.1f;
	SetGravityType(GRAVITYTYPE::GRAVITATIONAL);
	SetCollisionGroup(COLLISIONGROUP::PLAYER);
	SetBounding(glm::vec3(1.1f, 1.3f, 0.0f));

	//组件
	health = new Health(this, 30.0f);
	combat = new Combat(this, 10.0f);

	ListenForEvent("attacked", ONATTACKED);

	AddTag("player");
}

/*****************************************************************/

void Player::Up()
{
	//position.y += deltatime * speed;
	if (CanJump())
	{
		if (jump_cd.CanCast())
		{
			physics->SetVerticalV(1.7 * speed);
			jump_level++;
		}
	}
}

void Player::Down()
{
	//position.y -= deltatime * speed;
	//??
}

void Player::Left()
{
	//position.x -= deltatime * speed;
	right = false;
	physics->SetHorizontalV(-speed);
}

void Player::Right()
{
	//position.x += deltatime * speed;
	right = true;
	physics->SetHorizontalV(speed);
}

void Player::PauseLR()
{
	physics->SetHorizontalV(0);
}

bool Player::CanJump()
{
	if (!OnGround())
		return false;
	if (jump_level > 0)
		return false;

	return true;
}

/****************************************************************/

void Player::EventTask(unsigned int id, DATA data)
{
	switch (id)
	{
	case ONATTACKED:
		OnAttacked(data);
		break;
	}
}

void Player::TimeTask(unsigned int id)
{
	switch (id)
	{
	case STOPSTUN:
		StopStun();
		break;
	case STOPINVINCIBLE:
		StopInvincible();
		break;
	}
}

void Player::PeriodicTask(unsigned int id, float time)
{
	switch (id)
	{
	case STUNEFFECT:
		StunEffect(time);
		break;
	case TWINT:
		Twint();
		break;
	}
}

/***********************************************************************/

COLLIDEDIRECTION Player::CollideWith(const Entity& ent) const
{
	if (physics == nullptr || ent.GetPhysics() == nullptr || &ent == this)
		return COLLIDEDIRECTION::NONE;
	float deltax = fabs(position.x - ent.position.x);
	float deltay = fabs(position.y - ent.position.y);

	float boundx = (ent.GetPhysics()->boundingbox.x + physics->boundingbox.x) / 2.0f;
	float boundy = (ent.GetPhysics()->boundingbox.y + physics->boundingbox.y) / 2.0f;

	bool c_x = false, c_y = false;
	//下面两个参数设的越小，越容易碰撞(设定为负数时即便还未差一点也会被判定为碰撞，这种情况一般用来识别攻击到敌人)
	//player来说这两个参数设定的大，主要是怕被墙卡住
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

void Player::UpdatePosition(float deltaT)
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
			auto iter = collideobj.begin();
			if (t == COLLIDEDIRECTION::HORIZONTAL)
			{
				physics->SetHorizontalV(0.0f);
				hasc_x = true;
			}
			if (t == COLLIDEDIRECTION::VERTICAL)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_y = true;
				if ((*iter)->position.y < position.y)
				{
					jump_level = 0;
				}
			}
			if (t == COLLIDEDIRECTION::ALLDIR)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_x = true;
				hasc_y = true;
				if ((*iter)->position.y < position.y)
				{
					jump_level = 0;
				}
			}
			++iter;
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

	if (!physics->onground)
		physics->UpdateVelocity(deltaT);
}

void Player::UpdateScaling(float deltaT)
{
	if (right)
		SetScale(1.0f, 1.0f, 1.0f);
	else
		SetScale(-1.0f, 1.0f, 1.0f);
}

void Player::Draw(Shader& shader)
{
	if (!twint_on)
		return;

	if (stunned)
		shader.setBool("uUseMult", true);
	else
		shader.setBool("uUseMult", false);
	shader.setVec("uMultColor", stunned_color);

	Entity::Draw(shader);

	shader.setBool("uUseMult", false);
}

/****************************************************************/
//
void Player::ShootArrow()
{
	if (!atk_cd.CanCast())
		return;

	Entity& ent = SpawnEntity(99);
	Arrow& arrow = (Arrow&)ent;
	if (right)
	{
		arrow.SetPosition(position + glm::vec3(0.2f, -0.1f + 0.15f * random(), 0.0f));
		arrow.GetPhysics()->GiveVelocity(glm::vec2(15.0f, 0.0f));
	}
	else
	{
		arrow.SetPosition(position + glm::vec3(-0.2f, -0.1f + 0.15f * random(), 0.0f));
		arrow.GetPhysics()->GiveVelocity(glm::vec2(-15.0f, 0.0f));
		arrow.SetScale(-1.0f, 1.0f, 1.0f);
	}
	arrow.SetAttacker(this);
}

//
void Player::Stun()
{
	if (stunned || health->IsInvincible())
		return;
	stunned = true;
	DoPeriodicTask(0.05f, STUNEFFECT, 1.0f);
	DoTaskInTime(1.0f, STOPSTUN);
}

void Player::StopStun()
{
	stunned = false;
	stunned_rot = 0.0f;
	SetRotation(0.0f);
}

void Player::StunEffect(float time)
{
	if (!stunned)
	{
		StopStun();
		return;
	}

	if (stunned_color.a < 0.1f)
		stunned_color.a = 0.3f;
	else
		stunned_color.a = 0.0f;
	
	stunned_rot = sinf(time * 2 * PI) * 0.06 * PI;
	SetRotation(stunned_rot);
}

//
void Player::OnAttacked(DATA data)
{
	SetCollisionGroup(COLLISIONGROUP::INV_PLAYER);
	health->SetInvincible(true);

	twint_on = false;
	DoPeriodicTask(0.2f, TWINT, 2.4f);

	DoTaskInTime(2.5f, STOPINVINCIBLE);
}

void Player::StopInvincible()
{
	SetCollisionGroup(COLLISIONGROUP::PLAYER);
	health->SetInvincible(false);

	twint_on = true;
}

void Player::Twint()
{
	twint_on = !twint_on;
}