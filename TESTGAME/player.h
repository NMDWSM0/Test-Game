#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "externfuncs.h"
#include "abilitycd.h"
#include "combat.h"
#include "health.h"

class Player : public Entity
{
public:
	enum FUNCNAME {
		STOPSTUN,
		STUNEFFECT,
	};

private:

	float speed = 7.0f;

	Ability_CD jump_cd, atk_cd;

	//
	bool right = true;

	//
	unsigned int jump_level = 0;

	//
	bool stunned = false;
	glm::vec4 stunned_color = glm::vec4(1.0f, 1.0f, 0.1f, 0.0f);
	float stunned_rot = 0.0f;

public:
	Player(const std::string& filename, const std::string& texturename = "");

	void UpdateKeyCD(float deltaT) 
	{
		if (CanJump())
			jump_cd.Update(deltaT);

		atk_cd.Update(deltaT);
	}

	void Up();
	void Down();
	void Left();
	void Right();
	void PauseLR();

	/**************************************/

	void TimeTask(unsigned int id);

	void EventTask(unsigned int id, DATA data = DATA{});

	void PeriodicTask(unsigned int id, float time);

	/**************************************/

	void Draw(Shader& shader);

	/**************************************/

	//
	void UpdatePosition(float deltaT);

	bool CanJump();

	//
	void ShootArrow();

	void Stun();
	void StopStun();
	void StunEffect(float time);
	bool IsStunned() const { return stunned; }
};

#endif