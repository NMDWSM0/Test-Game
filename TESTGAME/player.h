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
	//���Ҫ����ǰ�����
	SetPosition(0.0f, 0.0f, 0.1f);
	SetGravityType(Physics::GRAVITATIONAL);
}


//����������д�Ų���һ��
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