#pragma once
#include "world.h"
#include "entities.h"

extern Player* ThePlayer;
extern World* TheWorld;

World::World(const std::string& levelname)
{
	std::ifstream file(levelname);
	if (!file.is_open())
	{
		throw "World Load Filed";
	}

	std::string curline;
	while (std::getline(file, curline))
	{
		if (curline[0] == 'e')
		{
			unsigned int id;
			float x, y, z;
			std::string pos = curline.substr(2);
			sscanf(pos.c_str(), "%d/%f/%f/%f", &id, &x, &y, &z);

			SpawnEntity(id).SetPosition(x, y, z);

			continue;
		}
		if (curline[0] == 'p')
		{
			float x, y, z;
			std::string pos = curline.substr(2);
			sscanf(pos.c_str(), "%f/%f/%f", &x, &y, &z);
			ThePlayer->SetPosition(x, y, 0.1F);
			continue;
		}
	}
}

Entity& World::SpawnEntity(unsigned int id)
{
	Entity* ent;

	switch (id)
	{
	case 1:
		ent = new RedBlock();
		break;

	}

	entities.push_back(ent);
	return *ent;
}

std::vector<Entity*>& World::GetEntities()
{
	return entities;
}

void World::Draw(Shader& shader)
{
	for (auto e : entities)
	{
		e->Draw(shader);
	}
	ThePlayer->Draw(shader);
}

void World::Clear()
{
	for (auto e : entities)
	{
		delete[] e;
	}
}

void World::UpdateEntities(float deltaT)
{
	for (auto e : entities)
	{
		//if 有 brains 按照AI行动 更新velocity
		
		//更新物理
		if (e->HasPhysics())
		{
			e->UpdatePosition(deltaT);
		}
	}
	//玩家不需要在这里处理输入捏
	ThePlayer->UpdatePosition(deltaT);
}

/*********************************************/

std::vector<Entity*> CollidedObject(const Entity& ent, std::vector<Physics::COLLIDEDIRECTION>& dir)
{
	std::vector<Entity*> objs;
	dir.clear();
	for (auto e : TheWorld->GetEntities())
	{
		Physics::COLLIDEDIRECTION type = e->CollideWith(ent);
		if (type != Physics::NONE)
		{
			objs.push_back(e);
			dir.push_back(type);
		}
	}
	return objs;
}

std::vector<Entity*> NearObject(const Entity& ent)
{
	std::vector<Entity*> objs;
	for (auto e : TheWorld->GetEntities())
	{
		if (e->Near(ent))
			objs.push_back(e);
	}
	return objs;
}