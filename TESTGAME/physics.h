#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#define _g 9.8f
#define MAX_VELOCITY 25.0f
#include "glm/glm.hpp"

//��������Ӱ��ĳ̶�
enum GRAVITYTYPE {
	GRAVITATIONAL,  //��ȫ����������ң������ٶȿ죩
	HALF_GRAV,      //��������gֵ����ʵ��9.8��ͬ������BOSS�����ը����
	QUAD_GRAV,      //�ķ�֮һ�������������ʵֻ��Ϊ�˶�Ӧ��ʸ
	ANTIGRAVITY,    //Ư���������еķ���ͷ������ﶼ�������
};

enum COLLISIONGROUP {
	WORLD,   //����ķ��飬������ж�����ȫ��ײ���������Ҳ������������ǽ�ڣ�
			 //���������ȥ������ײ���ˣ�Ҳ��������������Ӱ�죨��������ǿ�ư��չ̶��ĳ����˶���
	A_WORLD, //������WORLD��ȫһ�£�Ψһ���������������ұ����߶ȵͣ��򲻻ᴥ���κ���ײ����
	ARROW,   //����ļ�ʸ
	MOVABLE, //���ƶ��ķ���,һ���ǻ������ӵȣ����PLAYER����һ�����ײ�����ᱻ�����ߣ�Ҳ���WORLD��ײ��ͣ�£���������������ײ�����ᶯ
	PLAYER,  //��ң�����������壬���WORLD, MOVABLE, PLAYER, ENTITIES, ENEMIES��ײ������WORLD��ײʱ�������ڴ������ϵ��������������ֱ��ײ��
			 //��MOVABLE��ײʱ�������һ������ߣ���ENTITIES��ENEMIES��ײʱ�������һᴥ��һЩЧ������������ѣ�εȵȣ�
    INV_PLAYER,//�޵�״̬����ң����˲��͹����Լ���ʸ��ײ���⣬��������ͨ���һ��  
	ENTITIES,//����ʵ�壬������ֵ��ߣ�BOSS�����ը���ȵȣ���Щʵ�������ѭ������ɣ�
			 //���絯����Ħ����������WORLD��MOVABLE��ײʱ����ѭһ���������ɣ��������ƶ�MOVABLE������PLAYER��ײ��������Ч������ENEMIES��ײҲ�У�ȡ���ھ�����ʲô��
    ENEMIES, //���ˣ�����Ư����BOSS������NETITIES����ײ���Ի���һ������������û����ѭʲô�������
	NOTHING  //��ȫ����ײ�����ûʲô��˵�ģ���Ҫע�����Ư����BOSSҲ����NONE��
};
//������ײ���У�����ͬ����ײ��ֻ��WORLD��MOVABLE

enum COLLIDEDIRECTION {
	ALLDIR,
	HORIZONTAL,
	VERTICAL,
	NONE,
};

class Physics
{
public:

	GRAVITYTYPE gravtype = ANTIGRAVITY;
	COLLISIONGROUP collisiongroup = WORLD;

	glm::vec3 boundingbox;

	glm::vec2 velocity = glm::vec2(0.0f);

	bool onground = false;

private:
	void CutVelocity();

public:
	Physics(GRAVITYTYPE t, COLLISIONGROUP c, glm::vec3 bound);

	glm::vec2 SetVelocity(glm::vec2 v);

	glm::vec2 SetHorizontalV(float x);

	glm::vec2 SetVerticalV(float y);

	glm::vec2 GiveVelocity(glm::vec2 v);

	glm::vec2 GetVelocity() const { return velocity; }

	bool IsStatic();

	void UpdateVelocity(float deltaT);
};

Physics::Physics(GRAVITYTYPE t, COLLISIONGROUP c, glm::vec3 bound) :
	gravtype(t), collisiongroup(c), boundingbox(bound)
{

}

void Physics::CutVelocity()
{
	float dist = velocity.x * velocity.x + velocity.y * velocity.y;
	if (dist > MAX_VELOCITY * MAX_VELOCITY)
	{
		velocity *= MAX_VELOCITY / sqrtf(dist);
	}
}

glm::vec2 Physics::SetVelocity(glm::vec2 v)
{
	velocity = v;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::SetHorizontalV(float x)
{
	velocity.x = x;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::SetVerticalV(float y)
{
	velocity.y = y;
	CutVelocity();
	return velocity;
}

glm::vec2 Physics::GiveVelocity(glm::vec2 v)
{
	velocity += v;
	CutVelocity();
	return velocity;
}

bool Physics::IsStatic()
{
	return fabs(velocity.x) < 0.001f && fabs(velocity.y) < 0.001f;
}

void Physics::UpdateVelocity(float deltaT)
{
	if (gravtype == GRAVITATIONAL)
		velocity.y -= deltaT * _g * 2.0f;
	else if (gravtype == HALF_GRAV)
		velocity.y -= deltaT * _g * 1.0f;
	else if (gravtype == QUAD_GRAV)
		velocity.y -= deltaT * _g * 0.2f;
	CutVelocity();
}

#endif // !PHYSICS_H
