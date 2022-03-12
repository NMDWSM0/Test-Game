#pragma once
#ifndef SPRING_H
#define SPRING_H

#include "entity.h"
class Spring : public Entity
{
public:
	Spring() :
		Entity("assets/prefabs/spring.txt", "assets/images/spring.png")
	{
		AddTag("blockers");
		AddTag("canonwall");
		AddTag("spring");

		SetCollisionGroup(COLLISIONGROUP::A_WORLD);
	}
};

#endif // !SPRING_H
