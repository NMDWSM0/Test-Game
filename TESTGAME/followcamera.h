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

	glm::vec2 viewsize = glm::vec2(24.0f, 18.0f);

private:
	virtual void updateCameraVectors();

public:
	FollowCamera(Entity* entity = nullptr, float followdistance = 3.0f);

	glm::vec2 GetHalfViewSize();

	void SetTracktarget(Entity* entity);

	Entity& GetTrackedEntity();

	void SetFollowDistance(float dist);

	void UpdateCameraPos(glm::vec4 size = glm::vec4(0.0f));

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

glm::vec2 FollowCamera::GetHalfViewSize()
{
	return 0.5f * viewsize;
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

//size: x_min, y_min, x_max, y_max
void FollowCamera::UpdateCameraPos(glm::vec4 size)
{
	if (trackedentity != nullptr)
		targetposition = trackedentity->position;

	float left = targetposition.x - viewsize.x * 0.5f;
	float right = targetposition.x + viewsize.x * 0.5f;
	float top = targetposition.y + viewsize.y * 0.5f;
	float buttom = targetposition.y - viewsize.y * 0.5f;

	if (left < size.r)
		targetposition.x += size.r - left;
	else if (right > size.b)
		targetposition.x -= right - size.b;

	if (buttom < size.g)
		targetposition.y += size.g - buttom;
	else if (top > size.a)
		targetposition.y -= top - size.a;

	if (size.b - size.r < viewsize.x)
		targetposition.x = (size.b + size.r) * 0.5f;
	if (size.a - size.g < viewsize.y)
		targetposition.y = (size.a + size.g) * 0.5f;

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