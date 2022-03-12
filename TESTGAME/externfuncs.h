#pragma once
#ifndef EXTERNFUNCS_H
#define EXTERNFUNCS_H
#include "entity.h"
#include <time.h>

float random()
{
	srand((unsigned)time(nullptr));
	return (rand() % 360) * 1.0F / 360;
}

std::vector<Entity*> CollidedObject(const Entity& ent, std::vector<COLLIDEDIRECTION>& dir);
std::vector<Entity*> NearObject(const Entity& ent);
void RemoveFromWorld(Entity* ent);
Entity& SpawnEntity(unsigned int id);
std::vector<Entity*> FindEntities(float x, float y, float z, float radius, const std::vector<std::string>& MUSTtags, const std::vector<std::string>& MUSTNOTtags = std::vector<std::string>{}, const std::vector<std::string>& MUSTOFONEtags = std::vector<std::string>{});
Entity* FindCloestEntityWithTag(float x, float y, float z, std::string tag);

void PlayerDeathQuit();
void LevelComplete();

#endif // !EXTERNFUNCS_H
