#pragma once
#ifndef MESHES_H
#define MESHES_H

#include <vector>
#include "material.h"
#include "obj_loader.h"
#include <GLFW/glfw3.h>

class Meshes
{
	unsigned int VertexArrays;
	unsigned int PositionBuffer, NormalBuffer, TexCoordBuffer, MaterialBuffer;

	unsigned int vertexnumber;

	void loadToGL();
public:
    //std::vector<Triangle> triangles;

	// 从文件创建网格
	//\param filename 文件名
	//\param mt 外部材质
	//Meshes(const std::string& filename, const Material* mt, const std::vector<std::string>& addtionalfiles = std::vector<std::string>{});

	// 给出自定义顶点
	Meshes(const std::vector<glm::vec3>& Vvertices, const Material* mt, const std::vector<glm::vec3>& Vnormals = std::vector<glm::vec3>{}, const std::vector<glm::vec2>& VtexCoords = std::vector<glm::vec2>{});

	//
	void Draw();

	// 调用结束，销毁
	void Terminate();

	//析构
	~Meshes();
};

//Meshes::Meshes(const std::string& filename, const Material* mt = nullptr, const std::vector<std::string>& addtionalfiles)
//{
//	//std::vector<objl::Loader> addloaders;
//	int loadernum = 1 + addtionalfiles.size();
//	objl::Loader* loaders = new objl::Loader[1 + addtionalfiles.size()];
//	loaders[0].LoadFile(filename);
//
//	for (int i = 0; i < addtionalfiles.size(); i++)
//	{
//		loaders[i + 1].LoadFile(addtionalfiles[i]);
//	}
//
//	assert(loaders[0].LoadedMeshes.size() >= 1);
//
//	//int indices_num = 0, vertex_num = 0;
//	int vertex_num = 0;
//	for (int i = 0; i < loadernum; i++)
//	{
//		for (int j = 0; j < loaders[i].LoadedMeshes.size(); j++)
//		{
//			//indices_num += loader.LoadedMeshes[j].Indices.size();
//			vertex_num += loaders[i].LoadedMeshes[j].Vertices.size();
//			printf("%d\n", vertex_num);
//		}
//	}
//	vertexnumber = vertex_num;
//
//	float* vertices = new float[3 * vertex_num];
//	float* normals = new float[3 * vertex_num];
//	float* texCoords = new float[2 * vertex_num];
//	float* materials = new float[12 * vertex_num];
//
//	vertex_num = 0;
//	for (int k = 0; k < loadernum; k++)
//	{
//		for (auto mesh : loaders[k].LoadedMeshes)
//		{
//
//			for (int i = 0; i < mesh.Vertices.size(); i++)
//			{
//				vertices[3 * vertex_num + i * 3] = mesh.Vertices[i].Position.X;
//				vertices[3 * vertex_num + i * 3 + 1] = mesh.Vertices[i].Position.Y;
//				vertices[3 * vertex_num + i * 3 + 2] = mesh.Vertices[i].Position.Z;
//			}
//
//			for (int i = 0; i < mesh.Vertices.size(); i++)
//			{
//				normals[3 * vertex_num + i * 3] = mesh.Vertices[i].Normal.X;
//				normals[3 * vertex_num + i * 3 + 1] = mesh.Vertices[i].Normal.Y;
//				normals[3 * vertex_num + i * 3 + 2] = mesh.Vertices[i].Normal.Z;
//			}
//
//			for (int i = 0; i < mesh.Vertices.size(); i++)
//			{
//				texCoords[2 * vertex_num + i * 2] = mesh.Vertices[i].TextureCoordinate.X;
//				texCoords[2 * vertex_num + i * 2 + 1] = mesh.Vertices[i].TextureCoordinate.Y;
//			}
//
//			//ka.r, ka.g, ka.b; kd.r, kd.g, kd.b; ks.r, ks.g, ks.b; ns; ior; dissolve;
//			if (mt != nullptr)
//			{
//				for (int i = 0; i < mesh.Vertices.size(); i++)
//				{
//					//ka
//					materials[12 * vertex_num + i * 12] = mt->ka.x;
//					materials[12 * vertex_num + i * 12 + 1] = mt->ka.y;
//					materials[12 * vertex_num + i * 12 + 2] = mt->ka.z;
//					//kd
//					materials[12 * vertex_num + i * 12 + 3] = mt->kd.x;
//					materials[12 * vertex_num + i * 12 + 4] = mt->kd.y;
//					materials[12 * vertex_num + i * 12 + 5] = mt->kd.z;
//					//ks
//					materials[12 * vertex_num + i * 12 + 6] = mt->ks.x;
//					materials[12 * vertex_num + i * 12 + 7] = mt->ks.y;
//					materials[12 * vertex_num + i * 12 + 8] = mt->ks.z;
//					//ns
//					materials[12 * vertex_num + i * 12 + 9] = mt->ns;
//					//ior
//					materials[12 * vertex_num + i * 12 + 10] = mt->ior;
//					//dissolve
//					materials[12 * vertex_num + i * 12 + 11] = mt->dissolve;
//				}
//			}
//
//			//更新已加载数据
//			//indices_num += mesh.Indices.size();
//			vertex_num += mesh.Vertices.size();
//		}
//	}
//
//	delete[] loaders;
//	loadToGL();
//
//	//绑定数组对象并写入数据
//	glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_num, vertices, GL_STATIC_DRAW);
//	//设定顶点属性指针并启用相应属性数组
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	//绑定数组对象并写入数据
//	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_num, normals, GL_STATIC_DRAW);
//	//设定顶点属性指针并启用相应属性数组
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//
//	//绑定数组对象并写入数据
//	glBindBuffer(GL_ARRAY_BUFFER, TexCoordBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertex_num, texCoords, GL_STATIC_DRAW);
//	//设定顶点属性指针并启用相应属性数组
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(2);
//
//	//绑定数组对象并写入数据
//	glBindBuffer(GL_ARRAY_BUFFER, MaterialBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * vertex_num, materials, GL_STATIC_DRAW);
//	//设定顶点属性指针并启用相应属性数组
//	//ka
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(3);
//	//kd
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(4);
//	//ks
//	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(5);
//	//ns, ior, dissolve
//	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
//	glEnableVertexAttribArray(6);
//
//	//delete[] indices;
//	delete[] vertices;
//	delete[] normals;
//	delete[] texCoords;
//	delete[] materials;
//}

Meshes::Meshes(const std::vector<glm::vec3>& Vvertices, const Material* mt, const std::vector<glm::vec3>& Vnormals, const std::vector<glm::vec2>& VtexCoords)
{
	int vertex_num = Vvertices.size();
	vertexnumber = vertex_num;
	float* vertices = new float[3 * vertex_num];
	float* normals = new float[3 * vertex_num];
	float* texCoords = new float[2 * vertex_num];
	float* materials = new float[12 * vertex_num];

	memset(vertices, 0, 3 * vertex_num * sizeof(float));
	memset(normals, 0, 3 * vertex_num * sizeof(float));
	memset(texCoords, 0, 2 * vertex_num * sizeof(float));
	memset(materials, 0, 12 * vertex_num * sizeof(float));

	for (int i = 0; i < vertex_num; i++)
	{
		vertices[i * 3] = Vvertices[i].x;
		vertices[i * 3 + 1] = Vvertices[i].y;
		vertices[i * 3 + 2] = Vvertices[i].z;
	}

	for (int i = 0; i < Vnormals.size(); i++)
	{
		normals[i * 3] = Vnormals[i].x;
		normals[i * 3 + 1] = Vnormals[i].y;
		normals[i * 3 + 2] = Vnormals[i].z;
	}

	for (int i = 0; i < VtexCoords.size(); i++)
	{
		texCoords[i * 2] = VtexCoords[i].x;
		texCoords[i * 2 + 1] = VtexCoords[i].y;
	}

	for (int i = 0; i < vertex_num; i++)
	{
		//ka
		materials[i * 12] = mt->ka.x;
		materials[i * 12 + 1] = mt->ka.y;
		materials[i * 12 + 2] = mt->ka.z;
		//kd
		materials[i * 12 + 3] = mt->kd.x;
		materials[i * 12 + 4] = mt->kd.y;
		materials[i * 12 + 5] = mt->kd.z;
		//ks
		materials[i * 12 + 6] = mt->ks.x;
		materials[i * 12 + 7] = mt->ks.y;
		materials[i * 12 + 8] = mt->ks.z;
		//ns
		materials[i * 12 + 9] = mt->ns;
		//ior
		materials[i * 12 + 10] = mt->ior;
		//dissolve
		materials[i * 12 + 11] = mt->dissolve;
	}

	loadToGL();

	//绑定数组对象并写入数据
	glBindBuffer(GL_ARRAY_BUFFER, PositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_num, vertices, GL_STATIC_DRAW);
	//设定顶点属性指针并启用相应属性数组
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//绑定数组对象并写入数据
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_num, normals, GL_STATIC_DRAW);
	//设定顶点属性指针并启用相应属性数组
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//绑定数组对象并写入数据
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertex_num, texCoords, GL_STATIC_DRAW);
	//设定顶点属性指针并启用相应属性数组
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	//绑定数组对象并写入数据
	glBindBuffer(GL_ARRAY_BUFFER, MaterialBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * vertex_num, materials, GL_STATIC_DRAW);
	//设定顶点属性指针并启用相应属性数组
	//ka
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	//kd
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(4);
	//ks
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(5);
	//ns, ior, dissolve
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(6);

	delete[] vertices;
	delete[] normals;
	delete[] texCoords;
	delete[] materials;
}

void Meshes::loadToGL()
{
	//渲染初始化
	//创建VAO顶点数组对象
	//unsigned int VertexArrays;
	glGenVertexArrays(1, &VertexArrays);
	//绑定顶点数组对象
	glBindVertexArray(VertexArrays);

	//创建VBO顶点缓冲对象，此处包括顶点位置，法线，纹理uv，以及材质属性
	//unsigned int PositionBuffer, NormalBuffer, TexCoordBuffer, MaterialBuffer;
	glGenBuffers(1, &PositionBuffer);
	glGenBuffers(1, &NormalBuffer);
	glGenBuffers(1, &TexCoordBuffer);
	glGenBuffers(1, &MaterialBuffer);
}

void Meshes::Draw()
{
	glBindVertexArray(VertexArrays);
	glDrawArrays(GL_TRIANGLES, 0, vertexnumber);
	glBindVertexArray(0);
}

void Meshes::Terminate()
{
	glDeleteVertexArrays(1, &VertexArrays);
	glDeleteBuffers(1, &PositionBuffer);
	glDeleteBuffers(1, &NormalBuffer);
	glDeleteBuffers(1, &TexCoordBuffer);
	glDeleteBuffers(1, &MaterialBuffer);
}

Meshes::~Meshes()
{
	Terminate();
}

#endif // !MESHES_H
