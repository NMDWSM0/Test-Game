#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "externfuncs.h"

class Player : public Entity
{
	float speed = 8.0f;

	float atkcd = 0.8f;
	float cur_t = 0.81f;

	bool right = true;

public:
	Player(const std::string& filename, const std::string& texturename = "");

	void UpdateKeyCD(float deltaT) 
	{
		cur_t += deltaT;
	}

	void Up();
	void Down();
	void Left();
	void Right();
	void PauseLR();

	//
	void ShootArrow();
};

Player::Player(const std::string& filename, const std::string& texturename):
	Entity(filename, texturename)
{
	position.z = 0.1f;
	SetGravityType(Physics::GRAVITATIONAL);
	SetCollisionGroup(Physics::PLAYER);
}


//先暂且这样写着测试一下
void Player::Up()
{
	//position.y += deltatime * speed;
	if (OnGround())
		physics->SetVerticalV(1.5 * speed);
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

void Player::ShootArrow()
{
	if (cur_t < atkcd)
		return;
	cur_t = 0.0f;

	auto& arrow = SpawnEntity(10);
	if (right)
	{
		arrow.SetPosition(position + glm::vec3(0.6f, 0.5f + 0.15f * random(), 0.0f));
		arrow.GetPhysics()->GiveVelocity(glm::vec2(10.0f, 0.0f));
	}
	else
	{
		arrow.SetPosition(position + glm::vec3(-0.6f, 0.5f + 0.15f * random(), 0.0f));
		arrow.GetPhysics()->GiveVelocity(glm::vec2(-10.0f, 0.0f));
	}
}

#endif