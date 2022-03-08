#include "entity.h"
#include "externfuncs.h"

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

	physics = new Physics(Physics::ANTIGRAVITY, Physics::WORLD, glm::vec3(1.0f));
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

	bool c_x = false, c_y = false;
	if (boundx - deltax > 0.1f && boundy - deltay > 0.0f)
		c_y = true;
	if (boundy - deltay > 0.1f && boundx - deltax > 0.0f)
		c_x = true;
	if (!c_x && !c_y)
		return Physics::NONE;
	else if (c_x && !c_y)
		return Physics::HORIZONTAL;
	else if (!c_x && c_y)
		return Physics::VERTICAL;
	else
		return Physics::ALLDIR;
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
		if ((position.y - e->position.y > 0 && position.y - e->position.y < 1.01f) && fabs(position.x - e->position.x) + 0.01f < (e->physics->boundingbox.x + physics->boundingbox.x) / 2.0f)
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
		bool hasc_x = false, hasc_y = false;
		for (auto t : dir)
		{
			if (t == Physics::HORIZONTAL)
			{
				physics->SetHorizontalV(0.0f);
				hasc_x = true;
			}
			if (t == Physics::VERTICAL)
			{
				physics->SetVelocity(glm::vec2(0.0f, 0.0f));
				hasc_y = true;
			}
			if (t == Physics::ALLDIR)
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

		PushEvent("collide");
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
	if (meshes)
		delete[] meshes;
	if (textures)
		delete[] textures;
	if (materials)
		delete[] materials;
	if (physics)
		delete[] physics;
}