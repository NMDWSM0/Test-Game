#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#define _g 2 * 9.8f
#define MAX_VELOCITY 30.0f
#include "glm/glm.hpp"

class Physics
{
public:
	enum GRAVITYTYPE {
		GRAVITATIONAL,
		ANTIGRAVITY,
	};

	enum COLLISIONGROUP {
		WORLD,
		MOVABLE,
		PLAYER,
		ENTITIES,
		NOTHING
	};

	enum COLLIDEDIRECTION {
		ALLDIR,
		HORIZONTAL,
		VERTICAL,
		NONE,
	};

	GRAVITYTYPE gravtype = ANTIGRAVITY;
	COLLISIONGROUP collisiongroup = WORLD;

	glm::vec3 boundingbox;

	glm::vec2 velocity = glm::vec2(0.0f);

	bool onground = false;

private:
	void CutVelocity();

public:
	Physics(GRAVITYTYPE t, COLLISIONGROUP c, glm::vec3 bound);

	glm::vec2 SetVelocity(glm::vec2 v);

	glm::vec2 SetHorizontalV(float x);

	glm::vec2 SetVerticalV(float y);

	glm::vec2 GiveVelocity(glm::vec2 v);

	glm::vec2 GetVelocity() { return velocity; }

	void UpdateVelocity(float deltaT);
};

Physics::Physics(GRAVITYTYPE t, COLLISIONGROUP c, glm::vec3 bound) :
	gravtype(t), collisiongroup(c), boundingbox(bound)
{

}

void Physics::CutVelocity()
{
	float dist = velocity.x * velocity.x + velocity.y * velocity.y;
	if (dist > MAX_VELOCITY * MAX_VELOCITY)
	{
		velocity *= MAX_VELOCITY / sqrtf(dist);
	}
}

glm::vec2 Physics::SetVelocity(glm::vec2 v)
{
	velocity = v;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::SetHorizontalV(float x)
{
	velocity.x = x;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::SetVerticalV(float y)
{
	velocity.y = y;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::GiveVelocity(glm::vec2 v)
{
	velocity += v;
	CutVelocity();
	return velocity;
}

void Physics::UpdateVelocity(float deltaT)
{
	if (gravtype == GRAVITATIONAL && fabs(velocity.y) < MAX_VELOCITY)
		velocity.y -= deltaT * _g;
}

#endif // !PHYSICS_H
