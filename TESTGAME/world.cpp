#pragma once
#include "world.h"
#include "entities.h"

extern Player* ThePlayer;
extern World* TheWorld;

World::World():
	hasloaded(false)
{

}

World::World(const std::string& levelname)
{
	LoadWorld(levelname);
}

void World::LoadWorld(const std::string& levelname)
{
	std::ifstream file(levelname);
	if (!file.is_open())
	{
		throw "World Load Filed";
	}

	std::string curline;
	bool hassize = false;
	while (std::getline(file, curline))
	{
		if (curline[0] == 's')
		{
			float x1, y1, x2, y2;
			std::string size = curline.substr(2);
			sscanf(size.c_str(), "%f/%f/%f/%f", &x1, &y1, &x2, &y2);

			assert(x1 < x2&& y1 < y2);

			SetWorldSize(x1, y1, x2, y2);
			hassize = true;
		}

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
	assert(hassize);
	hasloaded = true;
}

void World::QuitWorld()
{
	Clear();
	hasloaded = false;
}


/**********************************************************************/



// x_min, y_min, x_max, y_max
void World::SetWorldSize(float x1, float y1, float x2, float y2)
{
	x_min = x1;
	x_max = x2;
	y_min = y1;
	y_max = y2;
}

// x_min, y_min, x_max, y_max
glm::vec4 World::GetWorldSize()
{
	return glm::vec4(x_min, y_min, x_max, y_max);
}

float World::GetWorldExpansion()
{
	return glm::length(glm::vec2(x_max - x_min, y_max - y_min));
}

Entity& World::SpawnEntity(unsigned int id)
{
	Entity* ent;

	switch (id)
	{
	//blocks
	case 1:
		ent = new RedBlock();
		break;
	case 2:
		ent = new RedBlock_A();
		break;
	case 3:
		ent = new DisaprBlock();
		break;
	case 4:
		ent = new Spring();
		break;

	//arrow
	case 99:
		ent = new Arrow();
		break;

	//monsters
	case 100:
		ent = new SmallDot();
		break;
	case 101:
		ent = new FlyEar();
		break;
	case 102:
		ent = new JumpBlack();
		break;
	case 103:
		ent = new ShieldBlack();
		break;
	}

	entities.push_back(ent);
	return *ent;
}

std::vector<Entity*>& World::GetEntities()
{
	return entities;
}

bool World::NoMonsterInWorld()
{
	for (auto ent : entities)
	{
		if (ent->HasTag("monster"))
			return false;
	}
	return true;
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
	/*for (auto e : entities)
	{
		delete[] e;
	}*/
	entities.clear();
}

void World::UpdateEntities(float deltaT)
{
	if (NoMonsterInWorld())
		LevelComplete();

	for (auto e : entities)
	{
		e->Update(deltaT);
	}
	//玩家不需要在这里处理输入捏
	ThePlayer->Update(deltaT);
	
	SetWorldReverse();
}

void World::SetWorldReverse()
{
	for (auto e : entities)
	{
		if (e->HasTag("FX"))
			continue;
		if (e->position.x < x_min)
			e->position.x += (x_max - x_min);
		else if (e->position.x > x_max)
			e->position.x -= (x_max - x_min);

		if (e->position.y < y_min)
			e->position.y += (y_max - y_min);
		else if (e->position.y > y_max)
			e->position.y -= (y_max - y_min);
	}
	auto e = ThePlayer;
	if (e->position.x < x_min)
		e->position.x += (x_max - x_min);
	else if (e->position.x > x_max)
		e->position.x -= (x_max - x_min);

	if (e->position.y < y_min)
		e->position.y += (y_max - y_min);
	else if (e->position.y > y_max)
		e->position.y -= (y_max - y_min);
}