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
	bool movable;

public:
	Entity();

	Entity(const std::string& filename, const std::string& texturename = "");

	~Entity();

	//physics
	void SetGravityType(Physics::GRAVITYTYPE type)
	{
		physics->gravtype = type;
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

	void SetMovable(bool movable) { this->movable = movable; }

	void UpdatePosition(float deltaT);

	//shade
	/********************************/
	void Draw(Shader& shader);

};

/*******************************************************/

std::vector<Entity*> CollidedObject(const Entity& ent, std::vector<Physics::COLLIDEDIRECTION>& dir);
std::vector<Entity*> NearObject(const Entity& ent);

/*******************************************************/

Entity::Entity() :
	meshes(nullptr), textures(nullptr), materials(nullptr), physics(nullptr)
{
	position = glm::vec3(0.0f);
}

Entity::Entity(const std::string& filename, const std::string& texturename)
{
	position = glm::vec3(0.0f);

	std::ifstream file(filename);
	if (!file.is_open())
	{
		meshes = nullptr;
		textures = nullptr;
		materials = nullptr;
		physics = nullptr;
	}

	std::ifstream texfile(texturename);
	if (texfile.is_open())
	{
		textures = new Texture(texturename.c_str());
	}
	else
	{
		textures = nullptr;
	}
	texfile.close();

	std::vector<glm::vec3> Vvertices;
	std::vector<glm::vec3> Vnormals;
	std::vector<glm::vec2> VtexCoords;

	std::string curline;
	while (std::getline(file, curline))
	{
		if (curline[0] == 'v')
		{
			std::string pos = curline.substr(2);
			float x, y, z;
			sscanf(pos.c_str(), "%f/%f/%f", &x, &y, &z);
			Vvertices.push_back(glm::vec3(x, y, z));
			continue;
		}
		if (curline[0] == 'n')
		{
			//??
			continue;
		}
		if (curline[0] == 't')
		{
			std::string tex = curline.substr(2);
			float x, y;
			sscanf(tex.c_str(), "%f/%f", &x, &y);
			VtexCoords.push_back(glm::vec2(x, y));
			continue;
		}
		if (curline[0] == 'm')
		{
			std::string mat = curline.substr(2);
			float kax, kay, kaz, kdx, kdy, kdz, ksx, ksy, ksz;
			sscanf(mat.c_str(), "%f/%f/%f/%f/%f/%f/%f/%f/%f", &kax, &kay, &kaz, &kdx, &kdy, &kdz, &ksx, &ksy, &ksz);
			materials = new Material(glm::vec3(kax, kay, kaz), glm::vec3(kdx, kdy, kdz), glm::vec3(ksx, ksy, ksz));
		}
	}

	meshes = new Meshes(Vvertices, materials, Vnormals, VtexCoords);
	file.close();

	physics = new Physics(Physics::ANTIGRAVITY, Physics::ENTITIES, glm::vec3(1.0f));
}

glm::vec3 Entity::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	return position;
}

glm::vec3 Entity::SetPosition(glm::vec3 pos)
{
	position = pos;
	return position;
}

void Entity::SetScale(float scale)
{
	this->scale = scale; 
}

Physics::COLLIDEDIRECTION Entity::CollideWith(const Entity& ent) const
{
	if (physics == nullptr || ent.physics == nullptr || &ent == this)
		return Physics::NONE;
	float deltax = fabs(position.x - ent.position.x);
	float deltay = fabs(position.y - ent.position.y);

	float boundx = (ent.physics->boundingbox.x + physics->boundingbox.x) / 2.0f;
	float boundy = (ent.physics->boundingbox.y + physics->boundingbox.y) / 2.0f;

	if (boundx - deltax > 0.01f && boundy - deltay > 0.0f )
		return Physics::VERTICAL;
	if (boundy - deltay > 0.01f && boundx - deltax > 0.0f)
		return Physics::HORIZONTAL;
	return Physics::NONE;
}

bool Entity::Near(const Entity& ent) const
{
	if (physics == nullptr || ent.physics == nullptr || &ent == this)
		return false;
	float deltax = fabs(position.x - ent.position.x);
	float deltay = fabs(position.y - ent.position.y);

	float boundx = (ent.physics->boundingbox.x + physics->boundingbox.x) / 2.0f;
	float boundy = (ent.physics->boundingbox.y + physics->boundingbox.y) / 2.0f;

	if (deltax < boundx + 0.01f && deltay < boundy + 0.01f)
		return true;
	return false;
}

bool Entity::OnGround() const
{
	std::vector<Entity*> nearobj = NearObject(*this);
	for (auto e : nearobj)
	{
		if ((position.y - e->position.y > 0 && position.y - e->position.y < 1.01f) && fabs(position.x - e->position.x) < (e->physics->boundingbox.x + physics->boundingbox.x)/2.0f)
			return true;
	}
	return false;
}

void Entity::UpdatePosition(float deltaT)
{
	if (physics == nullptr)
		return;
	glm::vec3 oripos = position;
	glm::vec3 targetpos = position + glm::vec3(deltaT * physics->GetVelocity().x, deltaT * physics->GetVelocity().y, 0.0f);
		//printf("%f, %f, %f\n", physics->GetVelocity().x, physics->GetVelocity().y, deltaT);
	SetPosition(targetpos);

	std::vector<Physics::COLLIDEDIRECTION> dir;
	std::vector<Entity*> collideobj = CollidedObject(*this, dir);
	/*if (collideobj != nullptr)
	{
		SetPosition((oripos + targetpos) / 2.0f);
		physics->SetVelocity(glm::vec2(0.0f));
	}*/
	if (collideobj.size() > 0)
	{
		for (auto t : dir)
		{
			if (t == Physics::HORIZONTAL)
				physics->SetHorizontalV(0.0f);
			if (t == Physics::VERTICAL)
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
		}
		SetPosition(oripos);
	}

	if (physics->gravtype == Physics::GRAVITATIONAL && OnGround())
	{
		physics->onground = true;
	}
	else
		physics->onground = false;

	if (!physics->onground)
		physics->UpdateVelocity(deltaT);
}

void Entity::Draw(Shader& shader)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

	shader.setMat("uModel", model);
	if (textures != nullptr)
	{
		shader.setBool("uHasTex", true);
		textures->Activate(GL_TEXTURE0);
	}
	else
	{
		shader.setBool("uHasTex", false);
	}
	shader.setInt("uTexture", 0);

	meshes->Draw();
}

Entity::~Entity()
{
	delete[] meshes;
	delete[] textures;
	delete[] materials;
	delete[] physics;
}

#endif