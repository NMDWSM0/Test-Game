#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#define _g 9.8f
#define MAX_VELOCITY 25.0f
#include "glm/glm.hpp"

//控制重力影响的程度
enum GRAVITYTYPE {
	GRAVITATIONAL,  //完全重力，如玩家（下落速度快）
	HALF_GRAV,      //半重力，g值与现实的9.8相同，比如BOSS发射的炸弹等
	QUAD_GRAV,      //四分之一重力，做这个其实只是为了对应箭矢
	ANTIGRAVITY,    //漂浮，世界中的方块和飞行生物都属于这个
};

enum COLLISIONGROUP {
	WORLD,   //世界的方块，会和所有东西完全碰撞，包括玩家也不能跳过此类墙壁，
			 //本身基本不去主动碰撞别人，也不会受其它东西影响（不动或者强制按照固定的程序运动）
	A_WORLD, //其它和WORLD完全一致，唯一的区别在于如果玩家比它高度低，则不会触发任何碰撞属性
	ARROW,   //射出的箭矢
	MOVABLE, //可移动的方块,一般是机关箱子等，会和PLAYER发生一般的碰撞，但会被推着走；也会和WORLD碰撞而停下；和其它几个有碰撞但不会动
	PLAYER,  //玩家，最特殊的物体，会和WORLD, MOVABLE, PLAYER, ENTITIES, ENEMIES碰撞；当和WORLD碰撞时，特殊在从下往上的情况并不发生垂直碰撞；
			 //和MOVABLE碰撞时正常，且会推着走；和ENTITIES和ENEMIES碰撞时正常，且会触发一些效果（比如死亡眩晕等等）
    INV_PLAYER,//无敌状态的玩家，除了不和怪物以及箭矢碰撞以外，其它和普通玩家一致  
	ENTITIES,//其它实体，诸如各种道具，BOSS发射的炸弹等等，这些实体更加遵循物理规律，
			 //比如弹跳，摩擦；它们与WORLD和MOVABLE碰撞时会遵循一点点物理规律（但不会推动MOVABLE），与PLAYER碰撞会有特殊效果，和ENEMIES碰撞也有（取决于具体是什么）
    ENEMIES, //敌人（除了漂浮的BOSS），和NETITIES的碰撞属性基本一样，区别在于没有遵循什么物理规律
	NOTHING  //完全不碰撞，这个没什么好说的，需要注意的是漂浮的BOSS也属于NONE组
};
//以上碰撞组中，会与同组碰撞的只有WORLD和MOVABLE

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
