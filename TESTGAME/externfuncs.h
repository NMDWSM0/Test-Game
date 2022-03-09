#pragma once
#ifndef EXTERNFUNCS_H
#define EXTERNFUNCS_H
#include "entity.h"

float random()
{
	return (rand() % INT_MAX) * 1.0F / INT_MAX;
}

std::vector<Entity*> CollidedObject(const Entity& ent, std::vector<COLLIDEDIRECTION>& dir);
std::vector<Entity*> NearObject(const Entity& ent);
void RemoveFromWorld(Entity* ent);
Entity& SpawnEntity(unsigned int id);
//std::vector<Entity*> FindEntities(float x, float y, float z, float radius, std::vector<const std::string&> MUSTtags, std::vector<const std::string&> MUSTNOTtags, std::vector<const std::string&> MUSTOFONEtags);

#endif // !EXTERNFUNCS_H
