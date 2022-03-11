#include "entity.h"
#include "externfuncs.h"

Entity::Entity() :
	meshes(nullptr), textures(nullptr), materials(nullptr), physics(nullptr), combat(nullptr), health(nullptr)
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

	physics = new Physics(GRAVITYTYPE::ANTIGRAVITY, COLLISIONGROUP::WORLD, glm::vec3(1.0f));

	combat = nullptr;
	health = nullptr;
}

void Entity::Remove()
{
	RemoveFromWorld(this);
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
	this->scale = glm::vec3(scale);
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::abs(this->scale));
	if (physics != nullptr)
		physics->boundingbox = glm::vec3(scaling * glm::vec4(oribounding, 1.0f));
}

void Entity::SetScale(float scale_x, float scale_y, float scale_z)
{
	this->scale = glm::vec3(scale_x, scale_y, scale_z);
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::abs(scale));
	if (physics != nullptr)
		physics->boundingbox = glm::vec3(scaling * glm::vec4(oribounding, 1.0f));
}

void Entity::SetRotation(float rot)
{
	rotation = rot;
}

/**********************************************************************/

bool Entity::CanCollide(const Entity& ent, COLLIDEDIRECTION dir) const
{
	float deltay;
	switch (physics->collisiongroup)
	{
	case COLLISIONGROUP::WORLD:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING)
			return true;
		return false;
		break;

	case COLLISIONGROUP::A_WORLD:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::A_WORLD)
			return true;
		return false;
		break;

	//这里写的都是主动碰撞别人的时候的属性，A_WORLD组都不会动（或者强行运动），对player直接返回true也未尝不可

	case COLLISIONGROUP::ARROW:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::ARROW)
			return true;
		return false;
		break;

	case COLLISIONGROUP::MOVABLE:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING)
			return true;
		return false;
		break;

	case COLLISIONGROUP::PLAYER:
		if (ent.physics->collisiongroup == COLLISIONGROUP::WORLD)
		{
			return true;
		}
		if (ent.physics->collisiongroup == COLLISIONGROUP::A_WORLD || ent.physics->collisiongroup == COLLISIONGROUP::ARROW)
		{
			deltay = position.y - physics->boundingbox.y / 2.0f - (ent.position.y + ent.physics->boundingbox.y / 2.0f);
			if (deltay > 0.0f)
				//在上面
				return true;
			if (deltay > -0.03f * fabs(physics->velocity.y))
			{
				if (physics->GetVelocity().y >= 0.0f)
					return false;
				return true;
			}
			return false;
		}
		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::PLAYER && ent.physics->collisiongroup != COLLISIONGROUP::INV_PLAYER)
			return true;
		return false;
		break;

	case COLLISIONGROUP::INV_PLAYER:
		if (ent.physics->collisiongroup == COLLISIONGROUP::WORLD)
		{
			return true;
		}
		if (ent.physics->collisiongroup == COLLISIONGROUP::A_WORLD || ent.physics->collisiongroup == COLLISIONGROUP::ARROW)
		{
			deltay = position.y - physics->boundingbox.y / 2.0f - (ent.position.y + ent.physics->boundingbox.y / 2.0f);
			if (deltay > 0.0f)
				//在上面
				return true;
			if (deltay > -0.03f * fabs(physics->velocity.y))
			{
				if (physics->GetVelocity().y >= 0.0f)
					return false;
				return true;
			}
			return false;
		}
		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::PLAYER && ent.physics->collisiongroup != COLLISIONGROUP::ENEMIES && ent.physics->collisiongroup != COLLISIONGROUP::THR_ENEMIES && ent.physics->collisiongroup != COLLISIONGROUP::INV_PLAYER)
			return true;
		return false;
		break;

	case COLLISIONGROUP::ENTITIES:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::ENTITIES)
			return true;
		return false;
		break;

	case COLLISIONGROUP::ENEMIES:

		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::ENEMIES && ent.physics->collisiongroup != COLLISIONGROUP::INV_PLAYER && ent.physics->collisiongroup != COLLISIONGROUP::THR_ENEMIES)
			return true;
		return false;
		break;

	case COLLISIONGROUP::THR_ENEMIES:

		if (ent.physics->collisiongroup == COLLISIONGROUP::WORLD)
		{
			return true;
		}
		if (ent.physics->collisiongroup == COLLISIONGROUP::A_WORLD)
		{
			deltay = position.y - physics->boundingbox.y / 2.0f - (ent.position.y + ent.physics->boundingbox.y / 2.0f);
			if (deltay > 0.0f)
				//在上面
				return true;
			if (deltay > -0.03f * fabs(physics->velocity.y))
			{
				if (physics->GetVelocity().y >= 0.0f)
					return false;
				return true;
			}
			return false;
		}
		if (ent.physics->collisiongroup != COLLISIONGROUP::NOTHING && ent.physics->collisiongroup != COLLISIONGROUP::THR_ENEMIES && ent.physics->collisiongroup != COLLISIONGROUP::ENEMIES && ent.physics->collisiongroup != COLLISIONGROUP::INV_PLAYER)
			return true;
		return false;
		break;

	case COLLISIONGROUP::NOTHING:

		return false;
		break;

	}
	return false;
}

//碰撞默认函数，子类可以自行覆盖方法
COLLIDEDIRECTION Entity::CollideWith(const Entity& ent) const
{
	if (physics == nullptr || ent.physics == nullptr || &ent == this)
		return COLLIDEDIRECTION::NONE;
	float deltax = fabs(position.x - ent.position.x);
	float deltay = fabs(position.y - ent.position.y);

	float boundx = (ent.physics->boundingbox.x + physics->boundingbox.x) / 2.0f;
	float boundy = (ent.physics->boundingbox.y + physics->boundingbox.y) / 2.0f;

	bool c_x = false, c_y = false;
	//下面两个参数设的越小，越容易碰撞(设定为负数时即便还未差一点也会被判定为碰撞，这种情况一般用来识别攻击到敌人)
	if (boundx - deltax > 0.01f && boundy - deltay > 0.0f && CanCollide(ent, COLLIDEDIRECTION::VERTICAL))
		c_y = true;
	if (boundy - deltay > 0.01f && boundx - deltax > 0.0f && CanCollide(ent, COLLIDEDIRECTION::HORIZONTAL))
		c_x = true;

	if (!c_x && !c_y)
		return COLLIDEDIRECTION::NONE;
	else if (c_x && !c_y)
		return COLLIDEDIRECTION::HORIZONTAL;
	else if (!c_x && c_y)
		return COLLIDEDIRECTION::VERTICAL;
	else
		return COLLIDEDIRECTION::ALLDIR;
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
		if (!e->HasTag("blockers"))
			continue;
		float deltay = position.y - physics->boundingbox.y / 2.0f - (e->position.y + e->physics->boundingbox.y / 2.0f);
		if (deltay > -0.1f && deltay < 0.01f)
			return true;
		/*if ((position.y - e->position.y > 0 && position.y - e->position.y < 1.01f) && fabs(position.x - e->position.x) + 0.01f < (e->physics->boundingbox.x + physics->boundingbox.x) / 2.0f)
			return true;*/
	}
	return false;
}

//此为基本流程，子类可以自行覆盖方法
void Entity::UpdatePosition(float deltaT)
{
	if (physics == nullptr)
		return;
	glm::vec3 oripos = position;
	glm::vec3 targetpos = position + glm::vec3(deltaT * physics->GetVelocity().x, deltaT * physics->GetVelocity().y, 0.0f);
	
	SetPosition(targetpos);

	std::vector<COLLIDEDIRECTION> dir;
	std::vector<Entity*> collideobj = CollidedObject(*this, dir);
	
	if (collideobj.size() > 0)
	{
		bool hasc_x = false, hasc_y = false;
		for (auto t : dir)
		{
			if (t == COLLIDEDIRECTION::HORIZONTAL)
			{
				physics->SetHorizontalV(0.0f);
				hasc_x = true;
			}
			if (t == COLLIDEDIRECTION::VERTICAL)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_y = true;
			}
			if (t == COLLIDEDIRECTION::ALLDIR)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_x = true;
				hasc_y = true;
			}
		}
		if (hasc_x && !hasc_y)
			SetPosition(oripos.x, targetpos.y, oripos.z);
		else if (hasc_y && !hasc_x)
			SetPosition(targetpos.x, oripos.y, oripos.z);
		else
			SetPosition(oripos);

		PushEvent("collide", DATA{ collideobj, dir });
	}

	if (physics->gravtype == GRAVITYTYPE::GRAVITATIONAL && OnGround())
	{
		physics->onground = true;
	}
	else
		physics->onground = false;

	physics->UpdateVelocity(deltaT);
}

void Entity::Draw(Shader& shader)
{
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotating = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 translating = glm::translate(glm::mat4(1.0f), position);

	glm::mat4 model = translating * rotating * scaling;

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
	if (meshes)
	{
		meshes->Terminate();
		//delete[] meshes;
	}
	/*if (textures)
		delete[] textures;
	if (materials)
		delete[] materials;
	if (physics)
		delete[] physics;*/
}