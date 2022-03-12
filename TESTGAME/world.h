#pragma once
#ifndef WORLD_H
#define WORLD_H
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include "entity.h"
#include "player.h"

class World : public Object
{
	bool hasloaded;

	float x_min, x_max, y_min, y_max;

	std::vector<Entity*> entities;

public:
	World();

	World(const std::string& levelname);

	void LoadWorld(const std::string& levelname);

	void QuitWorld();

	bool HasLoaded() const { return hasloaded; }

	/********************************************/

	void SetWorldSize(float x1, float y1, float x2, float y2);

	float GetWorldExpansion();

	glm::vec4 GetWorldSize();

	std::vector<Entity*>& GetEntities();

	Entity& SpawnEntity(unsigned int id);

	bool NoMonsterInWorld();

	void Draw(Shader& shader);

	void Clear();

	void UpdateEntities(float deltaT);

	void SetWorldReverse();
};

#endif