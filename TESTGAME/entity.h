#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#define _CRT_SECURE_NO_WARNINGS
#include "object.h"
#include "meshes.h"
#include "material.h"
#include "textures.h"
#include "physics.h"
//#include "components.h"

class Combat;
class Health;

class Entity : public Object
{
protected:
	Meshes* meshes;
	Texture* textures;
	Material* materials;
	Physics* physics;

public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 oribounding = glm::vec3(1.0f);
	float rotation = 0.0f;

public:
	//组件，此处全为null，只有子类进行初始化之后才有用
	Combat* combat;
	Health* health;
	//对于C++一个很不爽的点在于，变量必须要被定义...不然连返回null的机会都没有，直接炸掉了
	//如果是列表形式的数据结构，可以判断变量有没有被定义

public:
	Entity();

	Entity(const std::string& filename, const std::string& texturename = "");

	~Entity();

	//
	virtual void Remove();

	//physics
	Physics* GetPhysics() const
	{
		return physics;
	}

	void SetGravityType(GRAVITYTYPE type)
	{
		physics->gravtype = type;
	}

	void SetBounding(glm::vec3 box)
	{
		oribounding = box;
		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::abs(scale));
		if (physics != nullptr)
			physics->boundingbox = glm::vec3(scaling * glm::vec4(oribounding, 1.0f));
	}

	bool HasPhysics() const 
	{ 
		return physics != nullptr; 
	}

	bool CanCollide(const Entity& ent, COLLIDEDIRECTION dir = COLLIDEDIRECTION::ALLDIR) const;

	virtual COLLIDEDIRECTION CollideWith(const Entity& ent) const;

	//transform
	/********************************/
	glm::vec3 SetPosition(float x, float y, float z);
	glm::vec3 SetPosition(glm::vec3 pos);

	void SetScale(float scale);
	void SetScale(float scale_x, float scale_y, float scale_z);

	void SetRotation(float rot);

	bool Near(const Entity& ent) const;

	bool OnGround() const;

	void SetCollisionGroup(COLLISIONGROUP group) 
	{
		physics->collisiongroup = group; 
	}

	virtual void UpdatePosition(float deltaT);

	//shade
	/********************************/
	void Draw(Shader& shader);

};

#endif