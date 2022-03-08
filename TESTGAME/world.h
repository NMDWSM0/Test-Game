#pragma once
#ifndef WORLD_H
#define WORLD_H
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include "entity.h"
#include "player.h"

class World
{
	std::vector<Entity*> entities;

public:
	World(const std::string& levelname);

	std::vector<Entity*>& GetEntities();

	Entity& SpawnEntity(unsigned int id);

	void Draw(Shader& shader);

	void Clear();

	void UpdateEntities(float deltaT);
};

#endif