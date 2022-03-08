#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#define _CRT_SECURE_NO_WARNINGS
#include "object.h"
#include "meshes.h"
#include "material.h"
#include "textures.h"
#include "physics.h"

class Entity : public Object
{
protected:
	Meshes* meshes;
	Texture* textures;
	Material* materials;
	Physics* physics;

public:
	glm::vec3 position;
	float scale;

public:
	Entity();

	Entity(const std::string& filename, const std::string& texturename = "");

	~Entity();

	//
	void Remove();

	//physics
	Physics* GetPhysics()
	{
		return physics;
	}

	void SetGravityType(Physics::GRAVITYTYPE type)
	{
		physics->gravtype = type;
	}

	void SetBounding(glm::vec3 box)
	{
		if (physics != nullptr)
			physics->boundingbox = box;
	}

	//pos
	/********************************/
	glm::vec3 SetPosition(float x, float y, float z);
	glm::vec3 SetPosition(glm::vec3 pos);

	void SetScale(float scale);

	bool HasPhysics() const { return physics != nullptr; }

	Physics::COLLIDEDIRECTION CollideWith(const Entity& ent) const;

	bool Near(const Entity& ent) const;

	bool OnGround() const;

	void SetCollisionGroup(Physics::COLLISIONGROUP group) { physics->collisiongroup = group; }

	void UpdatePosition(float deltaT);

	//shade
	/********************************/
	void Draw(Shader& shader);

};

#endif