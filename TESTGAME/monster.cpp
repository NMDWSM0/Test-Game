#include "monster.h"

Monster::Monster(const std::string& filename, const std::string& texturename):
	Entity(filename, texturename)
{
	//以下参数可能不同怪物不同，这里只给出默认设置
	SetGravityType(GRAVITYTYPE::GRAVITATIONAL);
	SetCollisionGroup(COLLISIONGROUP::ENEMIES);
	SetBounding(glm::vec3(1.0f, 1.0f, 0.0f));

	//组件
	health = new Health(this, 10.0f);
	combat = new Combat(this, 10.0f);

    ListenForEvent("death", DEATH);

	AddTag("monster");
}

/****************************************************************/

void Monster::EventTask(unsigned int id, DATA data)
{
	switch (id)
	{
	case DEATH:
		Death();
	}
}

void Monster::TimeTask(unsigned int id)
{
	switch (id)
	{
	
	}
}

void Monster::PeriodicTask(unsigned int id, float time)
{
	switch (id)
	{
	
	}
}

/*************************************************************/

void Monster::Death()
{
	Remove();
}