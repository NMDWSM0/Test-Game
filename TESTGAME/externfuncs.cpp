#include "world.h"
#include "entities.h"
#include "externfuncs.h"
#include "GAMEMODE.h"

extern GAMEMODE gameon;

extern Player* ThePlayer;
extern World* TheWorld;

std::vector<Entity*> CollidedObject(const Entity& ent, std::vector<COLLIDEDIRECTION>& dir)
{
	std::vector<Entity*> objs;
	dir.clear();
	for (auto e : TheWorld->GetEntities())
	{
		if (e == &ent)
			continue;
		COLLIDEDIRECTION type = ent.CollideWith(*e);
		if (type != COLLIDEDIRECTION::NONE)
		{
			objs.push_back(e);
			dir.push_back(type);
		}
	}
	//单独处理player
	COLLIDEDIRECTION type = ent.CollideWith(*ThePlayer);
	if (type != COLLIDEDIRECTION::NONE)
	{
		objs.push_back(ThePlayer);
		dir.push_back(type);
	}
	return objs;
}

std::vector<Entity*> NearObject(const Entity& ent)
{
	std::vector<Entity*> objs;
	for (auto e : TheWorld->GetEntities())
	{
		if (ent.Near(*e))
			objs.push_back(e);
	}
	//单独处理player
	if (ent.Near(*ThePlayer))
		objs.push_back(ThePlayer);
	return objs;
}

void RemoveFromWorld(Entity* ent)
{
	for (auto iter = TheWorld->GetEntities().begin(); iter != TheWorld->GetEntities().end();)
	{
		if (*iter == ent)
		{
			iter = TheWorld->GetEntities().erase(iter);
			/*if (ent)
				delete[] ent;*/
		}
		else
			++iter;
	}
}

Entity& SpawnEntity(unsigned int id)
{
	return TheWorld->SpawnEntity(id);
}

std::vector<Entity*> FindEntities(float x, float y, float z, float radius, const std::vector<std::string>& MUSTtags, const std::vector<std::string>& MUSTNOTtags, const std::vector<std::string>& MUSTOFONEtags)
{
	std::vector<Entity*> objs;
	glm::vec4 worldsize = TheWorld->GetWorldSize();  // x_min, y_min, x_max, y_max
	float x_size = worldsize.z - worldsize.x;
	float y_size = worldsize.w - worldsize.y;
	for (auto e : TheWorld->GetEntities())
	{
		bool isnear = false;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (glm::length(e->position + glm::vec3(i * x_size, j * y_size, 0.0f) - glm::vec3(x, y, z)) < radius)
				{
					isnear = true;
					break;
				}
			}
		}
		if (!isnear)
			continue;

		for (auto t : MUSTtags)
		{
			if (!e->HasTag(t))
				goto back;
		}

		for (auto t : MUSTNOTtags)
		{
			if (e->HasTag(t))
				goto back;
		}

		if (MUSTOFONEtags.size() >= 1)
		{
			for (auto t : MUSTOFONEtags)
			{
				if (e->HasTag(t))
				{
					objs.push_back(e);
					goto back;
				}
			}
		}
		else
			objs.push_back(e);

	back:;
	}
	//player
	bool isnear = false;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (glm::length(ThePlayer->position + glm::vec3(i * x_size, j * y_size, 0.0f) - glm::vec3(x, y, z)) < radius)
			{
				isnear = true;
				break;
			}
		}
	}
	if (!isnear)
		return objs;
	for (auto t : MUSTtags)
		if (!ThePlayer->HasTag(t))
			return objs;
	for (auto t : MUSTNOTtags)
		if (ThePlayer->HasTag(t))
			return objs;
	if (MUSTOFONEtags.size() >= 1)
		for (auto t : MUSTOFONEtags)
			if (ThePlayer->HasTag(t))
				objs.push_back(ThePlayer);
	else
		objs.push_back(ThePlayer);

	return objs;
}

Entity* FindCloestEntityWithTag(float x, float y, float z, std::string tag)
{
	std::vector<Entity*> ents = FindEntities(x, y, z, TheWorld->GetWorldExpansion(), std::vector<std::string>{tag});

	glm::vec4 worldsize = TheWorld->GetWorldSize();  // x_min, y_min, x_max, y_max
	float x_size = worldsize.z - worldsize.x;
	float y_size = worldsize.w - worldsize.y;

	Entity* cloest = nullptr;
	float min_length = INFINITY;
	for (auto e : ents)
	{
		float dist = glm::length(glm::vec3(x, y, z) - e->position);
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				dist = fminf(dist, glm::length(glm::vec3(x, y, z) + glm::vec3(i * x_size, j * y_size, 0.0f) - e->position));
			}
		}

		if (dist < min_length)
		{
			cloest = e;
			min_length = dist;
		}
	}

	return cloest;
}

void PlayerDeathQuit()
{
	gameon = OFF;
}