#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
	float speed = 8.0f;
public:
	Player(const std::string& filename, const std::string& texturename = "");

	void Up();
	void Down();
	void Left();
	void Right();
	void PauseLROnGround();
};

Player::Player(const std::string& filename, const std::string& texturename):
	Entity(filename, texturename)
{
	//玩家要放在前面绘制
	SetPosition(0.0f, 0.0f, 0.1f);
	SetGravityType(Physics::GRAVITATIONAL);
}


//先暂且这样写着测试一下
void Player::Up()
{
	//position.y += deltatime * speed;
	if (OnGround())
		physics->GiveVelocity(glm::vec2(0.0f, 1.5 * speed));
}

void Player::Down()
{
	//position.y -= deltatime * speed;
	//??
}

void Player::Left()
{
	//position.x -= deltatime * speed;
	physics->SetVelocity(glm::vec2(-speed, physics->velocity.y));
}

void Player::Right()
{
	//position.x += deltatime * speed;
	physics->SetVelocity(glm::vec2(speed, physics->velocity.y));
}

void Player::PauseLROnGround()
{
	if (OnGround())
		physics->SetVelocity(glm::vec2(0.0f, physics->velocity.y));
}

#endif