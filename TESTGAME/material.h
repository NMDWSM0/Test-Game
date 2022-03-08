#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

enum MaterialType { VRAY, PBR };

class Material
{
public:
	// �������
	MaterialType type = VRAY;

	/* VRAY�������� */
	// �����⣬ɢ����ɫ��������ɫ����������ɫ
	glm::vec3 ka = glm::vec3(1.0f), kd = glm::vec3(1.0f), ks = glm::vec3(0.0f);
	// Specular Exponent ����ָ��
	float ns = 100.0f;
	// ior ������
	float ior = 1.0f;
	// d ͸���ȣ�
	float dissolve = 1.0f;

	/* PBR�������� */
	// ����������ɫ
	glm::vec3 albedo = glm::vec3(1.0f);
	//�ֲڶȣ�������
	float roughness = 1.0f, metalness = 0.0f;

	// �Է�������, ����Ϊ����ʾ������
	// Ĭ��vec3(-1.0f)
	glm::vec3 emission = glm::vec3(-1.0f);

	Material();

	Material(glm::vec3 albedo, float roughness = 1.0f, float metalness = 0.0f, glm::vec3 emission = glm::vec3(-1.0f));

	Material(glm::vec3 ka, glm::vec3 kd = glm::vec3(1.0f), glm::vec3 ks = glm::vec3(0.0f), float ns = 100.0f, float ior = 1.0f, float dissolve = 1.0f);
};

Material::Material()
{
	type = VRAY;
}

Material::Material(glm::vec3 albedo, float roughness, float metalness, glm::vec3 emission) :
	albedo(albedo), roughness(roughness), metalness(metalness), emission(emission)
{
	type = PBR;
}

Material::Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ns, float ior, float dissolve) :
	ka(ka), kd(kd), ks(ks), ns(ns), ior(ior), dissolve(dissolve)
{
	type = VRAY;
}

#endif // !MATERIAL_H
