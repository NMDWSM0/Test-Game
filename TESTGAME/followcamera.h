#pragma once
#ifndef FOLLOWCAMERA_H
#define FOLLOWCAMERA_H

#include "camera.h"
#include "entity.h"

class FollowCamera : public Camera
{
	Entity* trackedentity;
	float followdistance = 3.0f;
	glm::vec3 targetposition = glm::vec3(0.0f);

private:
	virtual void updateCameraVectors();

public:
	FollowCamera(Entity* entity = nullptr, float followdistance = 3.0f);

	void SetTracktarget(Entity* entity);

	Entity& GetTrackedEntity();

	void SetFollowDistance(float dist);

	void UpdateCameraPos();

	virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		return;
	}

	virtual void LockToZFront(glm::vec3 defaultpos = glm::vec3(0.0f, 0.0f, 3.0f));
};

FollowCamera::FollowCamera(Entity* entity, float followdistance):
	Camera(glm::vec3(0.0f, 0.0f, -followdistance)), followdistance(followdistance), trackedentity(entity)
{

}

void FollowCamera::SetTracktarget(Entity* entity)
{
	trackedentity = entity;
}

Entity& FollowCamera::GetTrackedEntity()
{
	return *trackedentity;
}

void FollowCamera::SetFollowDistance(float dist)
{
	followdistance = dist;
}

void FollowCamera::UpdateCameraPos()
{
	if (trackedentity != nullptr)
		targetposition = trackedentity->position;
	//摄像机会游动
	Position = targetposition - followdistance * Front;
}

void FollowCamera::updateCameraVectors()
{
	// 计算新的Front向量
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// 计算新的Right和Up向量
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
	//摄像机会游动
	Position = targetposition - followdistance * Front;
}

void FollowCamera::LockToZFront(glm::vec3 defaultpos)
{
	locked = true;

	Yaw = -90.0f;
	Pitch = 0.0f;

	updateCameraVectors();
}

#endif