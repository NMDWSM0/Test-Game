#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "shader.h"
#include <vector>
#include "meshes.h"
#include "material.h"
#include "textures.h"

class ChildBG
{
public:
	Meshes* meshes;
	Texture* textures, *textures2;
	Material* materials;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	float rotation = 0.0f;

	ChildBG(const std::string& filename, const std::string& texturename, const std::string& texturename2 = "")
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			meshes = nullptr;
			textures = nullptr;
			textures2 = nullptr;
			materials = nullptr;
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

		std::ifstream texfile2(texturename2);
		if (texfile2.is_open())
		{
			textures2 = new Texture(texturename2.c_str());
		}
		else
		{
			textures2 = nullptr;
		}
		texfile2.close();

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
	}

	void Draw(Shader& shader)
	{
		glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotating = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 translating = glm::translate(glm::mat4(1.0f), position);

		glm::mat4 model = translating * rotating * scaling;

		shader.setMat("uModel", model);
		if (textures != nullptr)
		{
			shader.setBool("uHasTex", true);
			textures->Activate(GL_TEXTURE0);
			if (textures2 != nullptr)
			{
				shader.setBool("uHasTex2", true);
				textures2->Activate(GL_TEXTURE1);
			}
			else
			{
				shader.setBool("uHasTex2", false);
			}
		}
		else
		{
			shader.setBool("uHasTex", false);
			shader.setBool("uHasTex2", false);
		}
		shader.setInt("uTexture", 0);
		shader.setInt("uTexture2", 1);

		meshes->Draw();
	}
};

class BackGround
{
public:
	ChildBG* bg0;
	ChildBG* bg1;

	Camera* selfcam;

	glm::vec2 offset1, offset2;

	BackGround()
	{
		bg0 = new ChildBG("assets/background/bg0.txt", "assets/background/stage05b.png");
		bg1 = new ChildBG("assets/background/bg1.txt", "assets/background/stage06a.png", "assets/background/stage06b.png");
		selfcam = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void Update(float deltaT)
	{
		selfcam->Pitch = -25.0f;
		selfcam->UpdateCamera();

		bg0->position = glm::vec3(0.0f, 0.0f, -50.0f);

		bg1->position = glm::vec3(5.0f, -10.0f, -25.0f);
		bg1->scale = glm::vec3(2.0f);
		bg1->rotation = 30.0f;

		float speed1 = 0.025f, speed2 = 0.032f;
		offset1.x += speed1 * deltaT; offset1.x -= floorf(offset1.x);
		offset1.y += speed1 * deltaT * 0.3f; offset1.y -= floorf(offset1.y);
		offset2.x += speed2 * deltaT; offset2.x -= floorf(offset2.x);
		offset2.y += speed2 * deltaT * 0.3f; offset2.y -= floorf(offset2.y);
	}

	void Draw(Shader& shader)
	{
		shader.use();

		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 view2 = selfcam->GetViewMatrix();
		
		glm::vec2 halfsize = TheCamera->GetHalfViewSize();
		glm::mat4 projection1 = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y, 1.0f, 55.0f);
		glm::mat4 projection2 = glm::perspective(glm::radians(55.0f), 4.0f/3.0f, 0.1f, 50.0f);

		shader.setMat("uView", view1);
		shader.setMat("uProjection", projection1);
		shader.setBool("uUseEdgeCut", false);
		shader.setVec2("uTexOffset1", glm::vec2(0.0f));
		shader.setVec2("uTexOffset2", glm::vec2(0.0f));
		bg0->Draw(shader);

		shader.setMat("uView", view2);
		shader.setMat("uProjection", projection2);
		shader.setBool("uUseEdgeCut", true);
		shader.setVec2("uTexOffset1", -offset1);
		shader.setVec2("uTexOffset2", -offset2);
		bg1->Draw(shader);
	}
};

#endif // !BACKGROUND_H
